
#include <stdio.h>
#include <stdlib.h>

int	ft_strlen(char *str)
{
	int i = 0;

	while (str[i])
		i++;
	return (i);
}

void	ft_swap(char *a, char *b)
{
	char buf = *a;
	*a = *b;
	*b = buf;
}

void	ft_sort(char *str, int len)
{
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = 0; j < len - 1; j++)
		{
			if (str[j] > str[j + 1])
				ft_swap(&str[j], &str[j + 1]);
		}

	}
}
void	ft_reverse(char *s, int start, int end)
{
	while (start < end)
	{
		ft_swap(&s[start], &s[end]);
		start++;
		end--;
	}
}

int	next_perm(char *str, int len)
{
	int i = len - 2;
	int j = len - 1;

	while (i >= 0 && str[i] >= str[i + 1]) i--;
	if (i < 0) return (0);
	while (str[j] <= str[i]) j--;
	ft_swap (&str[i], &str[j]);
	ft_reverse(str, i + 1, len - 1);
	return (1);
}

int	main(int ac, char **av)
{
	if (ac != 2) return (1);

	int buf_len = ft_strlen(av[1]);
	if (buf_len == 0) return (1);

	char *buf = malloc((buf_len + 1) * sizeof(char));
	if (!buf) return 1;

	for (int i = 0; i < buf_len; i++)
		buf[i] = av[1][i];
	buf[buf_len] = '\0';

	ft_sort(buf, buf_len);
	puts(buf);

	while (next_perm(buf, buf_len))
		puts(buf);

	free (buf);
	return (0);
}
