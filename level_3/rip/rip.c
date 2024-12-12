#include <unistd.h>
#include <stdio.h>
#define MAX 100

// Function to check if the string has balanced parentheses
int is_balanced(char *s)
{
    int b = 0;
    for (int i = 0; s[i]; i++)
    {
        b += (s[i] == '(') - (s[i] == ')');
        if (b < 0) // If there are unmatched closing parentheses
            return (0);
    }
    return (b == 0); // Balanced if all opening are matched with closing
}

// Recursive function to generate all possibilities
void gen(char *s, int i)
{
    if (!s[i])
    {
        if (is_balanced(s)) // Print only balanced solutions
            puts(s);
        return;
    }
    if (s[i] == '(' || s[i] == ')') // If it's a parenthesis, explore removal
    {
        gen(s, i + 1); // Skip this character
        char c = s[i];
        s[i] = ' '; // Mark as removed by replacing with a space
        gen(s, i + 1);
        s[i] = c; // Restore the original character
    }
    else
        gen(s, i + 1); // For non-parentheses, just move to the next
}

int main()
{
    char s[MAX] = {0};
    int n = read(0, s, MAX); // Read input from standard input
    if (n > 0)
        s[n - 1] = '\0'; // Replace newline character with string terminator
    gen(s, 0); // Start generating solutions
}



/*
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

*/