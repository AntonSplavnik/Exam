
/**
Assignment name:	picoshell
Expected files:		picoshell.c
Allowed functions:	close, fork, wait, exit, execvp, dup2, pipe
___________________________________________________________________

Write the following function:

int    picoshell(char **cmds[]);

The goal of this function is to execute a pipeline. It must execute each
commands [sic] of cmds and connect the output of one to the input of the
next command (just like a shell).

Cmds contains a null-terminated list of valid commands. Each rows [sic]
of cmds are an argv array directly usable for a call to execvp. The first
arguments [sic] of each command is the command name or path and can be passed
directly as the first argument of execvp.

If any error occur [sic], The function must return 1 (you must of course
close all the open fds before). otherwise the function must wait all child
processes and return 0. You will find in this directory a file main.c which
contain [sic] something to help you test your function.


Examples:
./picoshell /bin/ls "|" /usr/bin/grep picoshell
picoshell
./picoshell echo 'squalala' "|" cat "|" sed 's/a/b/g'
squblblb

Hints:
Not more than 30 fds should be opened at the same time.
Do not leak file descriptors
___________________________________________________________________

You are given this main function

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int	picoshell(char **cmds[]);

int	main(int argc, char **argv)
{
	int	cmds_size = 1;
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "|"))
			cmds_size++;
	}
	char ***cmds = calloc(cmds_size + 1, sizeof(char **));
	if (!cmds)
	{
		dprintf(2, "Malloc error: %m\n");
		return 1;
	}
	cmds[0] = argv + 1;
	int cmds_i = 1;
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "|"))
		{
			cmds[cmds_i] = argv + i + 1;
			argv[i] = NULL;
			cmds_i++;
		}
	}
	int ret = picoshell(cmds);
	if (ret)
		perror("picoshell");
	free(cmds);
	return ret;
}

___________________________________________________________________

Old summary by a student:
You are given a main function. It converts received arguments into cmds array
of strings. When there is a pipe the commands after the pipe are in the next
array of strings. You have to create a pipeline using the cmds you receive from
the main, and execute them. If there is any error the function should return 1.
Close all fds before returning. If the cmds executed successfully wait all
child processes and return 0.
*/

#include <unistd.h>		// for fork, pipe, dup2, execvp
#include <stdlib.h>		// for exit
#include <sys/wait.h>	// for wait

int picoshell(char **cmds[])
{
	int		i = 0;	//	index of the current command.
	int		prev_fd = STDIN_FILENO;	// keeps track of the read end of the previous pipe
	pid_t	pid;	// process id
	int		fd[2];	// file descriptors for the pipe

	while(cmds[i]) // while there are commands to execute
	{
		if(cmds[i + 1])	// if not the last command
			pipe(fd);

		pid = fork();
		if(pid == 0)	// Child
		{
			if(prev_fd != STDIN_FILENO)	// if not the first command
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if(cmds[i + 1])	// if not the last command
			{
				close(fd[0]);	// close the read end of the pipe
				dup2(fd[1], STDOUT_FILENO);	// set the write end of the pipe as the output
				close(fd[1]);	// close the write end of the pipe
			}
			execvp(cmds[i][0], cmds[i]);	// execute the command
			exit(1);	// if execvp fails, exit with 1
		}
		else	// Parent
		{
			if(prev_fd != STDIN_FILENO)	// if not the first command
				close(prev_fd);
			if(cmds[i + 1])	// if not the last command
			{
				close(fd[1]);	// close the write end of the pipe
				prev_fd = fd[0];	// store the read end of the current pipe for the next iteration
			}
		}
		i++;
	}
	while(wait(NULL) > 0) {} // The parent process waits for all child processes to terminate

	return (0);
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int	picoshell(char **cmds[]);

int	main(int argc, char **argv)
{
	int	cmds_size = 1;
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "|"))
			cmds_size++;
	}
	char ***cmds = calloc(cmds_size + 1, sizeof(char **));
	if (!cmds)
	{
		dprintf(2, "Malloc error: %m\n");
		return 1;
	}
	cmds[0] = argv + 1;
	int cmds_i = 1;
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "|"))
		{
			cmds[cmds_i] = argv + i + 1;
			argv[i] = NULL;
			cmds_i++;
		}
	}
	int ret = picoshell(cmds);
	if (ret)
		perror("picoshell");
	free(cmds);
	return ret;
}