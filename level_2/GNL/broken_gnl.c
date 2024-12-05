#include "get_next_line.h"
char *ft_strchr(char *s, int c)
{
    int i = 0;
    while (s[i] && s[i] != c) // add s[i] &&
        i++;
    if (s[i] == c)
        return s + i;
    else
        return NULL;
}
void *ft_memcpy(void *dest, const void *src, size_t n)
{
    while (n-- > 0) // change to n--
        ((char*)dest)[n] = ((char*)src)[n]; // remove - 1
    return dest;
}
size_t ft_strlen(char *s)
{
    size_t ret = 0;
    while (*s)
    {
        s++;
        ret++;
    }
    return (ret);
}
int str_append_mem(char **s1, char *s2, size_t size2) // X
{
    size_t size1 = ft_strlen(*s1);
    char *tmp = malloc(size2 + size1 + 1);
    if (!tmp)
        return 0;
    ft_memcpy(tmp, *s1, size1);
    ft_memcpy(tmp + size1, s2, size2);
    tmp[size1 + size2] = 0;
    free(*s1);
    *s1 = tmp;
    return 1;
}
int str_append_str(char **s1, char *s2) // X
{
    return str_append_mem(s1, s2, ft_strlen(s2));
}
void    *ft_memmove (void *dest, const void *src, size_t n)
{
    if (dest < src) // reverse the comparison sign
        return ft_memcpy(dest, src, n);
    else if (dest == src)
        return dest;
    while (n > 0) // changed all things with i to n  and do n > 0
    {
        ((char *)dest)[n] = ((char *)src)[n]; // changed
        n--; // changed
    }
    return dest;
}
char *get_next_line(int fd)
{
    static char b[BUFFER_SIZE + 1] = "";
    char *ret = NULL;
    char *tmp = ft_strchr(b, '\n');
    while (!tmp) {
        if (!str_append_str(&ret, b))
            return NULL;
        int read_ret = read(fd, b, BUFFER_SIZE);
        if (read_ret == -1)
        {
            free(ret); // add
            return NULL;
        }
        b[read_ret] = 0;
        if (read_ret == 0) // add
            break;  // add
        tmp = ft_strchr(b, '\n');  // copy the init of tmp
    }
    if (tmp && !str_append_mem(&ret, b, tmp - b + 1)) // add tmp &&
    {
        free(ret);
        return NULL;
    }
    if (tmp) // add
        ft_memmove(b, tmp + 1, ft_strlen(tmp + 1) + 1);  // add
    else // add
        b[0] = '\0'; // add
    return ret;
}
