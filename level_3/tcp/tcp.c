#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float compute_distance(float **dist_matrix, int *sol, int count)
{
	float total_dist = 0;
	for (int i = 0; i < count; i++)
	{
		if (i == count - 1)
			total_dist += dist_matrix[sol[i]][sol[0]];
		else
			total_dist += dist_matrix[sol[i]][sol[i + 1]];
	}
	return (total_dist);
}

float calculate_distance(float *p1, float *p2)
{
	float distance;
	if (p1 == p2) return 0;

	distance = sqrtf((p1[0] - p2[0])*(p1[0] - p2[0]) + (p1[1] - p2[1])*(p1[1] - p2[1]));
	return (distance);
}

float	solve_tsp(float **dist_matrix, int *sol, int start, int count, float min_dist)
{
	/* STEP 1:
		check if we are at start = count, one permutation is complete
		if so, compute distance and return it if less than min distance*/
	if (start == count)
	{
		float curr_dist = compute_distance(dist_matrix, sol, count);
		if (curr_dist < min_dist)
			return (curr_dist);
		else
			return (min_dist);
	}
	else
	{
		/* STEP 2:
			Loop over solution array
swap current  index with start
call function recursively with start = start + 1
swap back*/
		for (int j = start; j < count; j++)
		{
			int swap = sol[j];
			sol[j] = sol[start];
			sol[start] = swap;
			min_dist = solve_tsp(dist_matrix, sol, start + 1, count, min_dist);
			swap = sol[j];
			sol[j] = sol[start];
			sol[start] = swap;
		}
	}
}

int	main(void)
{
	float x, y;
	int count = 0;
	float **coords = NULL; // coordinates

	/* STEP 1:
		Read coordinates and save in a 2-day float array
		with size n by 2, n is the number of points
		arr[i][0] = x, arr[i][1] = y, for each point i.*/
	while(fscanf(stdin, "%f, %f", &x, &y) == 2)
	{
		coords = realloc(coords, sizeof(float *) * (count + 1));
		coords[count] = malloc(sizeof(float) * 2);
		coords[count][0] = x;
		coords[count][1] = y;
		count++;
	}

	/* STEP 2:
		Create a distance matrix - a 2D float array of size n*n,
		where dist_matrix[i][j] = distance between points i and j
		i = 0 -> n - 1;
		j = 0 -> n - 1;*/
	float **dist_matrix = malloc(sizeof(float *) * count); // distance matrix

	for (int j = 0; j < count; j++)
	{
		dist_matrix[j] = malloc (sizeof(float) * count);
		for (int k = 0; k < count; k++)
		{
			dist_matrix[j][k] = calculate_distance(coords[j], coords[k]);
		}
	}

	/* STEP 3:
		Create a solution array sol[count] and fill it from i = 0 to n - 1*/
	int sol[count];
	for (int i = 0; i < count; i++)
		sol[i] = i;
	/*STEP 4:
		call a recursive function solve_tsp with min_dist = INFINITY
		Pass the distance matrix, the solution array
		Start function call from start = 0 and end = count*/
	float min_dist = solve_tsp(dist_matrix, sol, 0, count, INFINITY);

	/*STEP 5:
		Print result and free mallocs*/

	// Print Result
	printf("%.2f\n", min_dist);

	// NEED TO FREE coords + dist_matrix
}
