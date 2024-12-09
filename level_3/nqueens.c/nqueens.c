#include <stdio.h>
#include <stdlib.h>

int n;
int pos[20];

int is_safe(int col, int row)
{
    for(int i = 0; i < col; i++)
	{
        if(pos[i] == row || pos[i] - row == i - col || row - pos[i] == i - col)
            return 0;
    }
    return 1;
}

void solve(int col)
{
    if(col == n)
	{
        for(int i = 0; i < n; i++)
            printf("%d%c", pos[i], (i == n-1) ? '\n' : ' ');
        return;
    }
    for(int row = 0; row < n; row++)
	{
        if(is_safe(col, row))
		{
            pos[col] = row;
            solve(col + 1);
        }
    }
}

int main(int argc, char **argv)
{
    if(argc == 2)
	{
        n = atoi(argv[1]);
        solve(0);
    }
    return 0;
}