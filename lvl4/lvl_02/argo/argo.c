


/**
Assignment name: argo
Expected files: argo.c
>>>>>>> cdc044b4a31a3dc0fc854b7571bf7f533c846808

Allowed functions: getc, ungetc, printf, malloc, calloc, realloc, free, isdigit, fscanf, write
-----------------
Write a function argo that will parse a json file in the structure declared in argo.h:

int	argo(json *dst, FILE *stream);

	dst	- is the pointer to the AST that you will create
	stream	- is the file to parse (man FILE)

Your function will return 1 for success and -1 for failure.
If an unexpected token is found you will print the following message in stdout:
"Unexpected token '%c'\n"
or if the token is EOF:
"Unexpected end of input\n"

In case of doubt how to parse json, read rfc8259. But you won't need it as the format is simple. Tested with the main, the output should be exactly the same as the input (except for errors).
There are some functions in argo.c that might help you.

Example: If the key is missing and only the value is present, the function must return -1.
You will only have to handle numbers, strings and maps.
Numbers will only be basic ints like in scanf("%d").
You must handle escaping in strings only for backslashes and quotations marks (no \n, \u or anything else).
You don't have to handle spaces in the input (so they should be considered as invalid tokens).
For example:
echo -n '"bonjour"' | ./argo /dev/stdin
"bonjour"
echo -n '"unf.string' | ./argo /dev/stdin
Unexpected end of input

"escape ! \"   "

MORE:

Examples that should work:

echo -n input | ./argo /dev/stdin | cat -e		output
--------------------------------------------------------------------------------------
'1'												1$
'"bonjour"'										"bonjour"$
'"escape!\""'									"escape!\""$
'{"tomatoes":42,"potatoes":234}'				{"tomatoes":42,"potatoes:234}$
'{"recursion":{"recursion":"recursion"}}'		{"recursion":{"recursion":"recursion"}}$
'"unfinished string'							Unexpected end of input$
'"unfinished string 2\"'						Unexpected end of input$
'{"no value?":}'								Unexpected token '}'$

TEST WITH BASH, AS ZSH MAY INTERPRETS THE ESCAPE CHARACTERS DIFFERENTLY.
*/

#include "argo.h"

// returns the next character in the stream without consuming it
int	peek(FILE *stream)
{
	int	c = getc(stream);

	ungetc(c, stream);
	return (c);
}

// prints an error message when an unexpected token is found
void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("Unexpected token '%c'\n", peek(stream));
	else
		printf("Unexpected end of input\n");
}

// consumes the next character in the stream if it matches the given character
int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return (1);
	}
	return (0);
}

// ensures that the next character in the stream matches a specific expected
// character. If not, it reports an error and returns 0.
// If it matches, it consumes the character and returns 1
int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return (1);
	unexpected(stream);
	return (0);
}

//////////////
// SOLUTION //
//////////////

int	g_error = 0;

json	parse_int(FILE *stream)
{
	json	j;
	int		num;
	int		ret_fscanf;

	j.type = INTEGER;
	j.integer = -42;

	ret_fscanf = fscanf(stream, "%d", &num);

	if (ret_fscanf != 1) // exactly 1 conversion expected
	{
		g_error = 1;
		return (j);
	}
	j.integer = num;
	return (j);
}

char	*parse_string(FILE *stream)
{
	char	*str = NULL;
	int		c;
	int		str_len = 0;

	str = malloc(sizeof(char));
	str[0] = '\0';

	// skip the opening quote; if not present -> error
	if (!accept(stream, '"'))
	{
		g_error = 1;
		return (str);
	}

	while (peek(stream) != '"' && peek(stream) != EOF && !g_error)
	{
		str_len++;
		str = realloc(str, (str_len + 1) * sizeof(char));
		str[str_len] = '\0';
		c = getc(stream);
		if (c == '\\')
		{
			if (peek(stream) != '"' && peek(stream) != '\\')
			{
				g_error = 1;
				return (str);
			}
			c = getc(stream);
		}
		str[str_len - 1] = c;
	}

	if (!accept(stream, '"'))
		g_error = 1;

	return (str);
}

json	parse_string_json(FILE *stream)
{
	json	j;

	j.type = STRING;
	j.string = parse_string(stream);
	return (j);
}

json	parse_json(FILE *stream);

json	parse_map(FILE *stream)
{
	json	j;

	// initialize the json object
	j.type = MAP;
	j.map.size = 0;
	j.map.data = NULL;

	// Ensure the map starts correctly with '{'
	if (!accept(stream, '{'))
	{
		g_error = 1;
		return (j);
	}

	// parse the key-value pairs
	while (peek(stream) != '}' && peek(stream) != EOF && !g_error)
	{
		pair	new_pair;

		new_pair.key = parse_string(stream);

		// if key parsing failed or missing colon after key, return error
		if (g_error || !accept(stream, ':'))
		{
			g_error = 1;
			return (j);
		}

		// parse the value
		new_pair.value = parse_json(stream);
		if (g_error)
			return (j);

		// store the parsed key-value pair
		j.map.size++;
		j.map.data = realloc(j.map.data, j.map.size * sizeof(pair));
		j.map.data[j.map.size - 1] = new_pair;

		// if no comma, expect to close the map
		if (!accept(stream, ','))
			break ;
	}

	// Ensure the map closes correctly with '{'
	if (!accept(stream, '}'))
		g_error = 1;

	return (j);
}

json	parse_json(FILE *stream)
{
	int		c = peek(stream);

	// parse the json object based on the first character
	if (c == '-' || isdigit(c))
		return (parse_int(stream));
	else if (c == '"')
		return (parse_string_json(stream));
	else if (c == '{')
		return (parse_map(stream));
	else
	{
		g_error = 1;
		return ((json){0});
	}
}

int	argo(json *dst, FILE *stream)
{
	if (!stream)
		return (-1);

	*dst = parse_json(stream);

	// if an error occurred during parsing, print an error message
	if (g_error)
	{
		unexpected(stream);
		return (-1);
	}

	return (1);
}
