#include <unistd.h>
#define MAX 100

int is_balanced(char *s) 
{
    int b = 0;
    for (int i = 0; s[i]; i++) 
    {
        b += (s[i] == '(') - (s[i] == ')');
        if (b < 0)
        	return (0);
    }
    return (b == 0);
}

void gen(char *s, int i) 
{
    if (!s[i]) 
    	return (void)(is_balanced(s) && puts(s));
    if (s[i] == '(' || s[i] == ')')
    {
        gen(s, i + 1);
        char c = s[i];
        s[i] = ' ';
        gen(s, i + 1);
        s[i] = c;
    }
    else gen(s, i + 1);
}

int main() 
{
    char s[MAX] = {0};
    int n = read(0, s, MAX);
    if (n > 0) s[n - 1] = '\0';
    	gen(s, 0);
}

