
//#include "permutaions.c"
#include "stdio.h"
#include "stdlib.h"

int	ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

void	swap(char *a, char *b)
{
	char buf = *a;
	*a = *b;
	*b = buf;
}

void	bouble_sort(char *str, int len)
{
	for (int i = 0; i < len -1; i++)\
	{
		int swapped = 0;
		for (int j = 0; j < len - 1; j++)
		{
			if (str[j] > str[j + 1])
			{
				swap(&str[j], &str[j + 1]);
				swapped = 1;
			}
		}
		if (swapped == 0) break;
	}	
}

int	main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	int len = ft_strlen(av[1]);
	if (len == 0) return (1);

	char *buf = malloc((len + 1) * sizeof(char));
	if (!buf) return (1);

	for (int i = 0; i < len; i++)
		buf[i] = av[1][i];
	buf[len] = '\0';
	
	puts(buf);
	bouble_sort(buf, len);
	puts(buf);
}

