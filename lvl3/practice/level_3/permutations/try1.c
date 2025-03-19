
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

void	sort(char *str, int len)
{
	for (int i = 0; i < len - 1; i++)
	{
		//int flag = 0;
		for (int j = 0; j < len - 1; j++)
		{
			if (str[j] > str[j + 1])
			{
				ft_swap(&str[j], &str[j + 1]);
				//flag = 1;
			}
		//if (flag == 0) break;
		}
	}
}

void	reverse(char *str, int start, int end)
{
	while (start < end)
	{
		ft_swap(&str[start], &str[end]);
		end--;
		start++;
	}
}

int	next_perm(char *str, int len)
{
	int i = len - 2;
	int j = len - 1;
	while (str[i] && str[i] >= str[i + 1])
	i--;
	if (i < 0) return 0;
	while (str[j] <= str[i])
	j--;
	ft_swap(&str[i], &str[j]);
	reverse(str, i + 1, len - 1);
	return (1);
}

int main(int ac, char **av)
{
	if (ac != 2)
		return(1);

	int len = ft_strlen(av[1]);
	if (len == 0) return (1);

	char *buf = malloc((len + 1) * sizeof(char));
	if (!buf) return (1);

	for (int i = 0; i < len; i++)
		buf[i] = av[1][i];
	buf[len] = '\0';

	sort(buf, len);
	puts(buf);
	while(next_perm(buf, len))
		puts(buf);

	free(buf);
	return (0);
}
