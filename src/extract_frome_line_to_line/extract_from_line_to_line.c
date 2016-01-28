#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

int extract_from_line_to_line(char * path, unsigned int from, unsigned int to)
{
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int number = 1;

	fp = fopen(path, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1) {
		if (number > to)
			break;
		if (number >= from)
			printf("%s", line);
		number++;
	}

	fclose(fp);
	if (line)
		free(line);
	return number - from;
}

int main (int argc, char **argv)
{
	if (argc != 4) {
		fprintf(stderr, "Usage: extract_from_line_to_line <file> <from> <to>\n");
		return -1;
	}
	return extract_from_line_to_line(argv[1], atoi(argv[2]), atoi(argv[3]));
}
