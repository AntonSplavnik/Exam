#include <stdio.h>
#include <stdlib.h>

typedef struct s_number_set
{
	int	*numbers;
	int	current_size;
	int	max_capacity;
}	t_number_set;

t_number_set	*initialize_number_set(int initial_capacity)
{
	t_number_set	*number_set;

	number_set = malloc(sizeof(t_number_set));
	if (!number_set)
		exit(1);
	number_set->numbers = malloc(initial_capacity * sizeof(int));
	if (!number_set->numbers)
	{
		free(number_set);
		exit(1);
	}
	number_set->current_size = 0;
	number_set->max_capacity = initial_capacity;
	return (number_set);
}

void	add_number_to_set(t_number_set *number_set, int new_number)
{
	int	*temp;

	if (number_set->current_size == number_set->max_capacity)
	{
		number_set->max_capacity *= 2;
		temp = realloc(number_set->numbers,
			number_set->max_capacity * sizeof(int));
		if (!temp)
		{
			free(number_set->numbers);
			free(number_set);
			exit(1);
		}
		number_set->numbers = temp;
	}
	number_set->numbers[number_set->current_size++] = new_number;
}

void	print_number_set(t_number_set *number_set)
{
	int	i;

	printf("{");
	i = 0;
	while (i < number_set->current_size)
	{
		if (i > 0)
			printf(" ");
		printf("%d", number_set->numbers[i]);
		i++;
	}
	printf("}\n");
}

void	find_target_subsets(int *numbers, int array_size, int target_sum,
		int start_index, t_number_set *current_set, int running_sum, int *is_found)
{
	int	i;

	if (running_sum == target_sum)
	{
		print_number_set(current_set);
		*is_found = 1;
	}
	if (start_index == array_size)
		return ;
	if (running_sum > target_sum)
        return ;
	i = start_index;
	while (i < array_size)
	{
		if (i > start_index && numbers[i] == numbers[i - 1])
		{
			i++;
			continue ;
		}
		add_number_to_set(current_set, numbers[i]);
		find_target_subsets(numbers, array_size, target_sum, i + 1,
			current_set, running_sum + numbers[i], is_found);
		current_set->current_size--;
		i++;
	}
}

void	bubble_sort_array(int *array, int size)
{
	int	i;
	int	j;
	int	temp;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (array[i] > array[j])
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
			j++;
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_number_set	*current_set;
	int			*input_numbers;
	int			target_sum;
	int			array_size;
	int			is_found;
	int			i;

	if (argc < 2)
	{
		printf("\n");
		return (0);
	}
	target_sum = atoi(argv[1]);
	array_size = argc - 2;
	input_numbers = malloc(array_size * sizeof(int));
	if (!input_numbers)
		exit(1);
	i = 0;
	while (i < array_size)
	{
		input_numbers[i] = atoi(argv[i + 2]);
		i++;
	}
	bubble_sort_array(input_numbers, array_size);
	current_set = initialize_number_set(10);
	is_found = 0;
	find_target_subsets(input_numbers, array_size, target_sum,
		0, current_set, 0, &is_found);
	// if (target_sum == 0 && array_size >= 0)
	// {
	// 	print_number_set(current_set);
	// 	is_found = 1;
	// }
	if (!is_found)
		printf("\n");
	free(current_set->numbers);
	free(current_set);
	free(input_numbers);
	return (0);
}