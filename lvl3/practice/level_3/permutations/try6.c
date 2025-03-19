
#include <stdio.h>
#include <stdlib.h>

int	ft_strlen(char *s)
{
	int i = 0;
	while (s[i]) i++;
	return (i);
}

void	ft_swap(char *a, char *b)
{
	char buf = *a;
	*a = *b;
	*b = buf;
}

void	ft_sort(char *s, int len)
{
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = 0; j < len - 1; j++)
		{
			if (s[j] > s[j + 1])
				ft_swap (&s[j], &s[j + 1]);
		}
	}
}

void	ft_reverse(char *s, int start, int end)
{
	while (start < end)
	{
		ft_swap(&s[start], &s[end]);
		end--;
		start++;
	}
}

int	next_perm(char *s, int len)
{
	int	i = len - 2;
	int j = len - 1;

	while (s[i] >= 0 && s[i] >= s[i + 1]) i--;
	if (i < 0) return (0);
	while (s[j] <= s[i]) j--;
	ft_swap (&s[i], &s[j]);
	ft_reverse(s, i + 1, len - 1);
	return (1);
}

int	main(int ac, char **av)
{
	if (ac != 2) return (1);

	int buf_len = ft_strlen(av[1]);
	if (buf_len == 0) return (1);

	char	*buf = malloc((buf_len + 1) * sizeof(char));
	if (!buf) return (1);

	for (int i = 0; i < buf_len; i++)
		buf[i] = av[1][i];
	buf[buf_len] = '\0';

	ft_sort(buf, buf_len);
	puts(buf);

	while (next_perm(buf, buf_len)) puts (buf);
	free (buf);
	return (0);
}
