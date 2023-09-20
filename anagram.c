#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEFAULT_WORDLIST "words.txt"

int count_lines (FILE *file)
{
	int i = 0;
	char *line = NULL;
	size_t dummy;
	while (getline(&line, &dummy, file) != -1) {
		++i;
		free(line);
		line = NULL;
	}
	fseek(file, 0, SEEK_SET);
	return i;
}

int cmp (const void *a, const void *b)
{
	return *(char*)a - *(char*)b;
}

void tolowers (char *s)
{
	int len = strlen(s);
	for (int i = 0; i < len; ++i)
		s[i] = tolower((int)s[i]);
}

int anagram (char *a, char *b)
{
	if (strlen(a) != strlen(b))
		return 0;
	tolowers(a);
	tolowers(b);
	qsort(a, strlen(a), 1, cmp);
	qsort(b, strlen(b), 1, cmp);
	return !strcmp(a, b);
}

void solve (char *word, char **words, int num_words)
{
	for (int i = 0; i < num_words; ++i) {
		char *a = strdup(word);
		char *b = strdup(words[i]);
		if (anagram(a, b))
			puts(words[i]);
		free(a);
		free(b);
	}
	puts("");
}


int main (int argc, char **argv)
{
	size_t dummy; //for getline()
	char **words;
	int num_words;
	char *filename = DEFAULT_WORDLIST;
	FILE *file;

	if (argc >= 3) {
		if (!strcmp(argv[1], "-f")) {
			filename = argv[2];
			argv += 2;
			argc -= 2;
		}
	}

	fprintf(stderr, "Opening %s\n", filename);
	file = fopen(filename, "r");
	if (!file) {
		perror("fopen");
		return -1;
	}

	num_words = count_lines(file);
	fprintf(stderr, "Reading %d words\n", num_words);

	words = malloc(num_words * sizeof(*words));
	memset(words, 0, num_words * sizeof(*words));
	for (int i = 0; i < num_words; ++i) {
		getline(&words[i], &dummy, file);
		words[i][strlen(words[i])-1] = '\0'; //remove newline
	}
	fprintf(stderr, "Loaded\n\n");

	if (argc == 1) {
		//Read puzzles from stdin
		char *word = NULL;
		while (getline(&word, &dummy, stdin) != -1) {
			word[strlen(word) - 1] = '\0';
			solve(word, words, num_words);
			word = NULL;
		}
	} else {
		//Read puzzles from argv
		for (int i = 1; i < argc; ++i)
			solve(argv[i], words, num_words);
	}
	return 0;
}
