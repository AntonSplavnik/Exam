
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

void skip_input_space()
{
    int c;
    while ((c = fgetc(stdin)) != '\n' && isspace(c));
    ungetc(c, stdin);  // Push the last non-space back into the input
}

int scan_char(va_list ap)
{
    char *c = va_arg(ap, char *);
    *c = fgetc(stdin);
    return (*c != '\n');  // Indicate success if it's not a newline
}

int scan_int(va_list ap)
{
    int *num = va_arg(ap, int *);
    int result = 0;
    int c = fgetc(stdin);

    // Check if it's a digit before processing
    if (!isdigit(c))
        return 0;  // Return failure if it's not a digit
    ungetc(c, stdin);  // Put the first character back to start parsing

    while (isdigit(c = fgetc(stdin)))
    {
        result = result * 10 + (c - '0');
    }
    ungetc(c, stdin);  // Put back the first non-digit

    *num = result;
    return 1;
}

int scan_string(va_list ap)
{
    char *str = va_arg(ap, char *);
    int i = 0;
    int c;
    skip_input_space();

    while ((c = fgetc(stdin)) != '\n' && !isspace(c))
    {
        str[i++] = c;
    }
    str[i] = '\0';  // Null-terminate the string
    ungetc(c, stdin);  // Push back the last space or newline

    return i > 0;  // Return success if at least one character was read
}

int receive_input(const char **format, va_list ap)
{
    switch (**format)
    {
        case 'c':
            return scan_char(ap);
        case 'd':
            return scan_int(ap);
        case 's':
            skip_input_space();
            return scan_string(ap);
        default:
            return 0;
    }
}

int ft_scanf(const char *format, ...)
{
    int i = 0, result = 0;
    va_list ap;

    va_start(ap, format);
    while (format[i] != '\0')
    {
        if (format[i] == '%')
        {
            i++;
            if (receive_input(&(format[i]), ap))
                result++;
            else
                break;
        }
        i++;
    }
    va_end(ap);
    return result;
}
