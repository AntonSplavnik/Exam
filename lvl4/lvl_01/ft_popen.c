
/**
Assignment name		: ft_popen
Expected files		: ft_popen.c
Allowed functions:	: pipe, fork, dup2, execvp, close, exit

Write the following function:

int	ft_popen(const char *file, char *const argv[], char type);

The function must launch the executable file with the arguments argv (using execvp).
If type is 'r' the function must return a file descriptor connected to the output of the command.
If type is 'w' the function must return a file descriptor connected to the input of the command.
In case of error or invalid parameter the function must return -1.

For example, the function could be used like that:

int	main() {
	int	fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
	char	*line;
	while ((line = get_next_line(fd)))
		ft_putstr(line);
}

Hints:
Do not leak file descriptors!
*/


#include <unistd.h>		// pipe, fork, dup2, execvp
#include <stdlib.h>		// exit

/**
Execute function in a child process and return a file descriptor connected
to the input or output of the child process.

 @param file:	Name of the executable file to run (e.g. "ls").
 @param argv:	Arguments to pass to the executable (e.g. {"ls", "-l", NULL}).
 @param type: 	'r': parent reads from the child process;
 				'w': parent writes to the child process.

 @return 		File descriptor connected to the input or output of the
				child process;
				'-1' if an error occurred.

*/
int	ft_popen(const char *file, const char *argv[], char type)
{
	pid_t	pid;
	int		fd[2];

	// check arguments for validity
	if (!file || !argv || (type != 'w' && type != 'r'))
		return (-1);

	if (pipe(fd) == -1)
		return (-1);

	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}

	if (pid == 0)	// CHILD PROCESS // -> Redirect standard input/output to pipe's ends
	{
		if (type == 'r')
		{
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);	// anything printed by the child process goes to the pipe
			close(fd[1]);
		}
		else if (type == 'w')
		{
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO); // child's input comes from the pipe
			close(fd[0]);
		}
		execvp(file, (char *const *)argv);
		exit(EXIT_FAILURE);	// in case execvp fails
	}
	else	// PARENT PROCESS //
	{
		if (type == 'r')
		{
			close(fd[1]);
			return (fd[0]);	// return read end for parent to read output
		}
		else if (type == 'w') // if 'write
		{
			close(fd[0]);
			return (fd[1]);	// return write end for parent to write input
		}
	}
	return (-1);
}

//////////
// MAIN //
//////////

#include <stdio.h>	// perror
#include <string.h>	// strlen


int	main(void)
{
	// read from pipe's output
	const char	*argv_r[] = {"ls", "-l", NULL};
	int			fd = ft_popen(argv_r[0], argv_r, 'r');

	char	buffer[1024];
	ssize_t	bytes;

	while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0) // read until there is no more data (or failure), leave space for null-terminator
	{
		buffer[bytes] = '\0';
		printf("%s", buffer);
	}
	close(fd);

	// write to pipe's input
	const char	*argv_w[] = {"cat", NULL};
	fd = ft_popen(argv_w[0], argv_w, 'w');

	const char	*msg = "Hello from the parent process!\n";
	write(fd, msg, strlen(msg));
	close(fd);

	return (0);
}
