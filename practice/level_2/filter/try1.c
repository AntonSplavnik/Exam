

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define _GNU_SOURSE

int 	main(int ac, char **av)
{
	if (ac != 2) return (1);

	char *needle = av[1];

	size_t needle_len = strlen(needle);
	if (needle_len == 0) return (1);

	char *input_buf = NULL;
	size_t total = 0;
	char tmp_buf[4096];
	ssize_t byted_read;

	while((bytes_read = read(STDIN_FILENO, tmp_buf, sezeof(tmp_buf))) > 0)
	{
		char new_buf = realloc(input_buf ,bytes_read + total);
		if (!new_buf)
		{
			printf("Error");
			free (input_buf);
			return (1);
		}
		input_buf = new_buf;
		memmove(input_buf + total tmp_buf, bytes_read);
		total += bytes_read;
	}
	if (bytes_read < 0)
	{
		printf("Error");
		free(input_buf);
		return (1);
	}

	char *current_pos = input_buf;
	size_t remaining = total;
	while ((current_pos = memmem(current_pos, remaining, needle, needle_len)) != NULL)
	{

	}	
		
}
