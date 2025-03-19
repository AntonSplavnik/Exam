
/**
Assignment name		:	vbc
Expected files		:	*.c, *.h
Allowed functions	:	malloc, calloc, recalloc, free, write, printf, isdigit
===============================================================================

Write a program that will print the result of a math expression given as
an argument.
You must handle the operations '+', '*' and parentheses.
You don't have to handle whitespaces in the expression.
All values in the expression will be between 0 and 9 (including).

In case of an unexpected symbol, you will print "Unexpected token '%c'\n" and
exit with code 1.
If the unexpected token is at the end of the input, print:
"Unexpected end of input\n" and exit with code 1.
This rule also applies in cases of an unexpected '(' or ')'.

The beginning if the code is provided in the subject folder (vbc.c).

Examples:

$> ./vbc '1' | cat -e
1$
$> ./vbc '3*4+5'
17$
$> ./vbc '(3+4)*5'
35$
$> ./vbc '(((((((1)))))))'
1$
$> ./vbc '(((((2+2)*2+2)*2+2)*2+2)*2+2)*2'
188$
$> ./vbc '1+' | cat -e
Unexpected end of input$
$> ./vbc '1+2)' | cat -e
Unexpected token ')'$

NOTE (NOT PART OF SUBJECT):
This is 'Recursive Descent Parsing':
- expression	--> term ("+" term)
- term			--> factor ("*" factor)
- factor		--> number | "(" expression ")"
*/

#include <stdio.h>	// for printf
#include <stdlib.h>	// for calloc
#include <ctype.h>	// for isdigit

void	unexpected(char c);

// Defines a node in the binary expression tree
typedef struct node {
	enum { ADD, MULT, VAL } type;
	int val;
	struct node *l;
	struct node *r;
}	node;

// Allocates and initializes a new node dynamically
node*	new_node(node n)
{
	node* ret = calloc(1, sizeof(node));
	if (!ret)
		return NULL;
	*ret = n;
	return ret;
}

// // Handles unexpected characters or end of input in the expression
// void	unexpected(char c)
// {
// 	if (c)
// 		printf("Unexpected token '%c'\n", c);
// 	else
// 		printf("Unexpected end of input\n");
// }

// Checks if the current character matches the expected character
// and advances the input string pointer if it does
int accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return 1;
	}
	return 0;
}

void	unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
}

// Ensures the current character matches the expected character;
// otherwise, prints an error message.
int	expect(char **s, char c)
{
	if (accept(s, c))
		return 1;
	unexpected(**s);
	return 0;
}

// Recursively evaluates the expression tree
int	eval_tree(node* tree)
{
	if (tree->type == ADD)
		return eval_tree(tree->l) + eval_tree(tree->r);
	else if (tree->type == MULT)
		return eval_tree(tree->l) * eval_tree(tree->r);
	else
		return tree->val;
}

// Recursively frees the memory allocated for the expression tree
void	destroy_tree(node* tree)
{
	if (tree)
	{
		if (tree->type != VAL)
		{
			destroy_tree(tree->l);
			destroy_tree(tree->r);
		}
		free(tree);
	}
}

//////////////
// SOLUTION //
//////////////

// If current character is a digit (0-9) create a 'VAL' node.
// Otherwise, print an unexpected token error
node *parse_number(char **s)
{
	if (isdigit(**s))
	{
		node n = { .type = VAL, .val = **s - '0', .r = NULL, .l = NULL };
		(*s)++;
		return new_node(n);
	}
	unexpected(**s);
	return (NULL);
}

node *parse_expr(char **s);

// Handles parantheses and numbers
node *parse_factor(char **s)
{
	if (accept(s, '('))	// If current char is '(' parse a full expression recursively
	{
		node *expr = parse_expr(s);
		if (!expect(s, ')')) // make sure expression ends with ')'
		{
			destroy_tree(expr);
			return (NULL);
		}
		return (expr);
	}
	return parse_number(s); // if no parantheses -> parse a single-digit number
}

// Handles multiplication (higher precedence than addition)
node *parse_term(char **s)
{
	node *left = parse_factor(s); // starts with factor
	while (accept(s, '*'))
	{
		node *right = parse_factor(s); // if '*' is found, parse another factor
		if (!right)
		{
			destroy_tree(left);
			return (NULL);
		}
		node n = { .type = MULT, .l = left, .r = right };
		left = new_node(n);
	}
	return (left);
}

// Handles addition '+'
node *parse_expr(char **s)
{
	node *left = parse_term(s); // start with a term
	while (accept(s, '+')) // if '+' is found, parse another term
	{
		node *right = parse_term(s);
		if (!right)
		{
			destroy_tree(left);
			return (NULL);
		}
		node n = { .type = ADD, .l = left, .r = right };
		left = new_node(n);
	}
	return (left);
}

///////////////
// /SOLUTION //
///////////////

// Takes expression as command-line argument
// Calls 'parse_expr' to build the expression tree
// Prints the result and frees the memory
int	main (int argc, char** argv)
{
	if (argc!= 2)	// Ensure exactly one argument
		return 1;

	// Parse the input into an expression tree
	node *tree = parse_expr(&argv[1]); // CHANGED PROTOTYPE TO ACCEPT A POINTER TO THE STRING POINTER
	if (!tree)
		return 1;

	if (*argv[1]) // if extra characters remain, print an error
	{
		unexpected(*argv[1]);
		destroy_tree(tree);
		return 1;
	}
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
	return (0);
}
