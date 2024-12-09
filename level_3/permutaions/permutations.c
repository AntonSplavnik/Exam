// malloc puts

#include <stdio.h>
#include <stdlib.h>

int ft_strlen(char *s)
{
	int i = 0;

	while (s[i])
		i++;
	return i;
}
void swap(char *a, char *b)
{
	char tmp = *a;

	*a = *b;
	*b = tmp;
}

void bubble_sort(char *s, int len)
{
	for (int i = 0; i < len -1; i++)
	{
		for (int j = 0; j < len - 1; j++)
		{
			if (s[j] > s[j + 1])
				swap(&s[j], &s[j + 1]);
		}
	}
}

void reverse(char *s, int start, int end)
{
	while (start < end)
	{
		swap(&s[start], &s[end]);
		end--;
		start++;
	}
}

int next_perm(char *s, int len)
{
	int i = len - 2;
	int j = len - 1;

	while (i >= 0 && s[i] >= s[i + 1])
		i--;
	if (i < 0)
		return 0;
	while (s[j] <= s[i])
		j--;
	swap(&s[i], &s[j]);
	reverse(s, i + 1, len - 1);
	return 1;
}

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	int len = ft_strlen(av[1]);
	if (len == 0) return 1;

	char *buf = malloc(len + 1);
	if (!buf) return 1;

	for (int i = 0; i < len; i++)
		buf[i] = av[1][i];
	buf[len] = '\0';

	bubble_sort(buf, len);
	puts(buf);
	while (next_perm(buf, len))
		puts(buf);
	free(buf);
	return 0;
}
