
// Emir

#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	char *needle = av[1];
	if (needle[0] == '\0') return 1;
	size_t needle_len = strlen(av[1]);
	char *input_buf = NULL;
	size_t total_size = 0;
	char tmp_buf[4096];
	ssize_t bytes_read;
	while ((bytes_read = read(STDIN_FILENO, tmp_buf, sizeof(tmp_buf))) > 0)
	{
		char *new_buf = realloc(input_buf, total_size + bytes_read);
		if (!new_buf)
		{
			free(input_buf);
			return 1;
		}
		input_buf = new_buf;
		memmove(input_buf + total_size, tmp_buf, bytes_read);
		total_size += bytes_read;
	}
	if (bytes_read < 0)
	{
		free(input_buf);
		return 1;
	}
	char *current_pos = input_buf;
	size_t remaining_bytes = total_size;
	while ((current_pos = memmem(current_pos, remaining_bytes, needle, needle_len)) != NULL)
	{
		for (int i = 0; i < needle_len; i++) current_pos[i] = '*';
		size_t bytes_processed = (current_pos - input_buf) + needle_len;
		remaining_bytes = total_size - bytes_processed;
		current_pos = input_buf + bytes_processed;
	}
	ssize_t written_bytes = write(STDOUT_FILENO, input_buf, total_size);
	if (written_bytes < 0)
	{
		free(input_buf);
		return 1;
	}
	free(input_buf);
	return 1;
}
