

#include <stdio.h>
#include <stdlib.h>

int arequeensok(int *arr, int row, int col)
{
    int i = 0;
    while (i < col)
    {
        if (arr[i] == row)
            return (0);
        if (arr[i] == row - (col - i))
            return (0);
        if (arr[i] == row + (col - i))
            return (0);
        i++;
    }
    return (1);
}
void    printsol(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d", arr[i]);
        if (i < n - 1)
            printf(" ");
    }
    printf("\n");
}
void    n_queens(int *arr, int n, int col)
{
    int row = 0;
    while (row < n)
    {
        arr[col] = row;
        if (arequeensok(arr, row, col))
        {
            if (col == n - 1)
                printsol(arr, n);
            else
                n_queens(arr, n, col + 1);
        }
        row++;
    }
}
int main(int ac, char **av)
{
    if (ac != 2)
        return (0);
    int n = atoi(av[1]);
    int arr[n];
    n_queens(arr, n, 0);
    return (0);
}
