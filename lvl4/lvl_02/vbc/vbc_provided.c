#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
	enum { ADD, MULT, VAL } type;
	int val;
	struct node *l;
	struct node *r;
}	node;

node*	new_node(node n)
{
	node* ret = calloc(1, sizeof(node));
	if (!ret)
		return NULL;
	*ret = n;
	return ret;
}

void	unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return 1;
	}
	return 0;
}

int	expect(char **s, char c)
{
	if (accept(s, c))
		return 1;
	unexpected(**s);
	return 0;
}

int	eval_tree(node* tree)
{
	if (tree->type == ADD)
		return eval_tree(tree->l) + eval_tree(tree->r);
	else if (tree->type == MULT)
		return eval_tree(tree->l) * eval_tree(tree->r);
	else
		return tree->val;
}

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

////// YOUR SOLUTION BELOW!
node	*parse_expr(char *s);

/////

int	main (int argc, char** argv)
{
	if (argc!= 2)
		return 1;
	node *tree = parse_expr(argv[1]);
	if (!tree)
		return 1;
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
	return (0);
}