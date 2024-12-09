#include <stdio.h>
#include <stdlib.h>

int board_size;
int *queen_pos;

void print_board()
{
	int col = 0;
	while (col < board_size)
	{
		printf("%d%c", queen_pos[col], (col == (board_size - 1)) ? '\n' : ' ');
		col++;
	}
}

int is_pos_safe(int current_col, int proposed_row)
{
	int previous_col = 0;
	while (previous_col < current_col)
	{
		int previous_queen_row = queen_pos[previous_col];
		if (previous_queen_row == proposed_row || previous_queen_row - proposed_row == previous_col - current_col ||
		 proposed_row - previous_queen_row == previous_col - current_col)
		 	return 0;
		previous_col++;
	}
	return 1;
}

void find_solutions (int current_col)
{
	if (current_col == board_size)
	{
		print_board();
		return;
	}
	int proposed_row = 0;
	while (proposed_row < board_size)
	{
		if (is_pos_safe(current_col, proposed_row))
		{
			queen_pos[current_col] = proposed_row;
			find_solutions(current_col + 1);
		}
		proposed_row++;
	}

}

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		board_size = atoi(argv[1]);
		queen_pos = (int *)malloc(board_size * sizeof(int));
		if (!queen_pos)
		{
			printf("Memory alloc failure");
			free(queen_pos);
		}
		find_solutions(0);
	}
	free(queen_pos);
	return 0;
}