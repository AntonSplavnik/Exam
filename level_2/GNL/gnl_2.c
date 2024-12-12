
// Audrey

#include "gnl.h"

char	*ft_strchr(char *s, int c)
{
	int i = 0;
		while (s[i] != (char)c && s[i]) // changed
			i++;
		if (s[i] == (char)c)
			return &s[i];
		else
			return NULL;
}

size_t	ft_strlen(char *s)
{
	size_t ret = 0;
		while (*s)
		{
			s++;
			ret++;
		}
	return (ret);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	while (n-- > 0) //changed
		((char*)dest)[n] = ((char*)src)[n]; //changed
	return dest;
}

int	str_append_mem(char **s1, char *s2, size_t size2)
{
	size_t size1;
	if (*s1) //added
		size1 = ft_strlen(*s1);
	else
		size1 = 0;
	char *tmp = malloc(size2 + size1 + 1);
	if (!tmp)
		return 0;
	ft_memcpy (tmp, *s1, size1);
	ft_memcpy (tmp + size1, s2, size2); //added missing comma
	tmp[size1 + size2] = 0;
	free(*s1);
	*s1 = tmp;
	return (1);
}

int	str_append_str(char **s1, char *s2)
{
	return str_append_mem(s1, s2, ft_strlen(s2));
}

void	*ft_memmove (void *dest, const void *src, size_t n)
{
	if (dest < src) //reverse comparison sign
		return ft_memcpy(dest, src, n);
	else if (dest == src)
		return dest;
	while (n-- > 0) //changed
		{
			((char *)dest)[n] = ((char *)src)[n]; //changed
		}
	return dest;
}

char	*get_next_line(int fd)
{
	static char b[BUFFER_SIZE + 1] = "";
	char *ret = NULL;
	char *tmp = ft_strchr(b, '\n');
	while (!tmp)
	{
		if (!str_append_str(&ret, b))
			return (NULL);
		int read_ret = read(fd, b, BUFFER_SIZE);
		if (read_ret == -1)
		{
			free(ret);  //added
			return (NULL);
		}
		b[read_ret] = 0;
		if (read_ret == 0)  //added
		{
			if (ret && *ret) //check if the pointer ret exist and if it is not empty
				return (ret);
			free(ret);
			return (NULL);
		}
	}
	tmp = ft_strchr(b, '\n'); //added
	if (!str_append_mem(&ret, b, tmp - b + 1))
	{
		free(ret);
		return NULL;
	}
	ft_memmove(b, tmp + 1, ft_strlen(tmp + 1) + 1); //added
	return (ret);
}

int main()
{
	int fd = open("txt.txt", O_RDONLY);
	char *line;
	while ((line = get_next_line(fd)))
	{
		printf("%s", line);
		free(line);
	}
	close (fd);
}
