
// Audrey

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

void	skip_input_space()
{
	int c;

	while ((c = fgetc(stdin)) != EOF && isspace(c))
		continue;
	if (c != EOF)
		ungetc(c, stdin);
}

int	scan_char(va_list ap)
{
	int c;
	char *ptr = va_arg(ap, char*);

	if ((c = fgetc(stdin)) == '\n')
		c = fgetc(stdin);
	if (c == EOF)
		return (0);
	*ptr = (char)c;
	return (1);
}

int	scan_int(va_list ap)
{
	int c;
	int *ptr = va_arg(ap, int*);
	int sign = 1;
	int value = 0;
	int has_digit = 0;

	c = fgetc(stdin);
	if (c == '-')
	{
		sign = -1;
		c = fgetc(stdin);
	}
	else if (c == '+')
		c = fgetc(stdin);

	while (c != EOF && isdigit(c))
	{
		has_digit = 1;
		value = value * 10 + (c - '0');
		c = fgetc(stdin);
	}
	if (c != EOF)
		ungetc(c, stdin);
	if (!has_digit)
		return 0;
	*ptr = value * sign;
	return (1);
}

int	scan_string(va_list ap)
{
	int c;
	char *ptr = va_arg(ap, char*);
	int	char_read = 0;

	while ((c = fgetc(stdin)) != EOF && !isspace(c))
		ptr[char_read++] = (char)c;
	if (char_read > 0)
	{
		ptr[char_read] = '\0';
		return 1;
	}
	return (0);
}

int receive_input(const char *format, va_list ap)
{
	switch (*format)
	{
		case 'c':
			return (scan_char(ap));
		case 'd':
			skip_input_space();
			return (scan_int(ap));
		case 's':
			skip_input_space();
			return (scan_string(ap));
		default:
			return (-1);
	}
}

int ft_scanf(const char *format, ...)
{
	int	i = 0;
	int	result = 0;
	va_list	ap;

	va_start(ap, format);
	while (format[i] != '\0')
	{
		if (format[i] == '%')
		{
			i++;
			if (receive_input(&(format[i]), ap) <= 0)
				break;
			result++;
		}
		i++;
	}
	va_end(ap);
	return (result);
}

int main ()
{
	char name[20];
	int age = 0;
	char initial = 0;

	printf("Please enter your name:\n");
	ft_scanf("%s", name);
	printf("how old are you %s?\n", name);
	ft_scanf("%d", &age);
	printf("Initial? \n");
	ft_scanf("%c", &initial);
	printf("Name : %s \nAge : %d \nInitial : %c\n", name, age, initial);
}
