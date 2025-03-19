
/**
Assignment name		:	sandbox
Expected files		:	sandbox.c
Allowed functions	:	fork, waitpid, exit, alarm, sigaction, kill,
						printf, strsignal, errno
===============================================================================

Write the following function:

#include <stdbool.h>
int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)

This function must test if the function f is a nice function or a bad function,
you will return 1 if f is nice , 0 if f is bad or -1 in case of an error in
your function.

A function is considered bad if it is terminated or stopped by a signal
(segfault, abort...), if it exit with any other exit code than 0 or if it
times out.

If verbose is true, you must write the appropriate message among the following:

"Nice function!\n"
"Bad function: exited with code <exit_code>\n"
"Bad function: <signal description>\n"
"Bad function: timed out after <timeout> seconds\n"

You must not leak processes (even in zombie state, this will be checked using
wait).

We will test your code with very bad functions.
*/

#include <signal.h>		// for sigaction, SIGALRM, SIGKILL
#include <stdbool.h>	// for bool, true, false
#include <stdio.h>		// for printf
#include <string.h> 	// for strsignal
#include <unistd.h>		// for fork, alarm
#include <sys/wait.h>	// for waitpid
#include <stdlib.h>		// for exit

// Global variables for signal handler (you cannot pass arguments to signal handlers)
pid_t	g_child_pid;				// Stores the child process ID
bool	g_kill_failed = false;		// Flag for kill failure in timeout handler
bool	g_timeout_killed = false;	// Flag to indicate if child was terminated by timeout

// Handler for SIGALRM / timeout
// Triggerd when alarm goes off (SIGALRM): Forcefully kill child process via SIGKILL
static void	timeout_handler(int signal)
{
	(void)signal;	// Would be SIGALRM, but not used

	if (kill(g_child_pid, SIGKILL) == -1)
	{
		g_kill_failed = true;	// Kill failed
		return ;
	}
	g_timeout_killed = true;	// Child successfully terminated by timeout
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	g_child_pid = fork();
	if (g_child_pid == -1)
		return (-1);	// Fork failed
	else if (g_child_pid == 0)	// CHILD PROCESS //
	{
		f();		// Call function in child process
		exit(0);	// Exit child process (if function doesn't exit on its own or doesn't terminate abnormally (signal))
	}

	// PARENT PROCESS //

	// Initialize signal action struct
	// SA_RESTART flag is set to restart interrupted system calls, it's more of a convention / good practice;
	// would work without it as well (leave it out or set to 0)
	struct sigaction	sa;
	sa.sa_handler = timeout_handler;
	sa.sa_flags = SA_RESTART;	// optional, but good practice

	// Clear / init the signal set (otherwise errors when valgrinding - not necessarily leaks though).
	// This is a bit of a hack, but it's the only way to clear sa_mask without using 'sigemptyset(&sa.sa_mask)'
	// prob not needed for passing the exam, but good to know
	int	i = 0;
	while (i < (int)(sizeof(sa.sa_mask.__val) / sizeof(sa.sa_mask.__val[0])))
	{
		sa.sa_mask.__val[i] = 0;
		i++;
	}

	// Set up signal handler for SIGALRM, invoke 'timeout_handler' when alarm goes off
	if (sigaction(SIGALRM, &sa, NULL))
		return (-1);
	alarm(timeout);	// Set alarm to go off after 'timeout' seconds

	// Wait and check exit status of child process
	int	w_status;
	if (waitpid(g_child_pid, &w_status, 0) == -1)
		return (-1);	// waitpid failed

	if (g_kill_failed)
		return (-1);	// timeout handler failed (kill failed)

	if (WIFEXITED(w_status))	// Child process exited normally (not by signal)
	{
		int	exit_code = WEXITSTATUS(w_status);
		if (exit_code == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (1);
		}
		else
		{
			if (verbose)
				printf("Bad function: exited with code %d\n", exit_code);
			return (0);
		}
	}
	else if (WIFSIGNALED(w_status))	// Child process terminated by signal
	{
		int signal_code = WTERMSIG(w_status);
		if (verbose)
		{
			if (g_timeout_killed)	// Child was terminated by timeout
				printf("Bad function: timed out after %d seconds\n", timeout);
			else	// Child was terminated by some other signal
				printf("Bad function: %s\n", strsignal(signal_code));
		}
		return (0);
	}
	else	// Something else went wrong
		return (-1);
}

/////////////////////
// TEST FUNCTIONS ///
/////////////////////


void	f1(void)
{
	printf("-> I'm a nice function\n");
}

void	f2(void)
{
	printf("-> I am exiting with code > 0\n");
	exit(42);
}

void	f3(void)
{
	printf("-> I'm about to segfault\n");

	int	*ptr = NULL;
	*ptr = 42;
}

void	f4(void)
{
	printf("-> I'm about to terminate abnormally\n");
	abort();
}

void	f5(void)
{
	printf("-> I will run forever!\n");
	while (true) {}
}

void	f6(void)
{
	printf("-> I will run forever and ignore all signales!\n");
	sigaction(SIGALRM, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);
	sigaction(SIGINT, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);
	sigaction(SIGTERM, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);
	sigaction(SIGSEGV, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);
	sigaction(SIGPIPE, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);
	sigaction(SIGBUS, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);
	sigaction(SIGCHLD, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);
	sigaction(SIGHUP, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);
	sigaction(SIGTSTP, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);
	sigaction(SIGTTIN, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);
	sigaction(SIGTTOU, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);
	sigaction(SIGURG, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);
	sigaction(SIGQUIT, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);
	while (true) {}
}

//////////
// MAIN //
//////////

int	main(void)
{
	void	(*funcs[])(void) = {f1, f2, f3, f4, f5, f6};

	for (int i = 0; i < (int)(sizeof(funcs)/sizeof(funcs[0])); ++i)
		printf("-> sandbox ret: %d\n\n", sandbox(funcs[i], 2, true));

	return (0);
}
