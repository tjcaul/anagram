#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_WORDS 1516999
#define PATH "words.txt"

int cmp (const void *a, const void *b) {
	return *(char*)a - *(char*)b;
}

void tolowers (char *s) {
	int len = strlen(s);
	for (int i = 0; i < len; ++i)
		s[i] = tolower((int)s[i]);
}

int anagram (char *a, char *b) {
	if (strlen(a) != strlen(b))
		return 0;
	tolowers(a);
	tolowers(b);
	qsort(a, strlen(a), 1, cmp);
	qsort(b, strlen(b), 1, cmp);
	return !strcmp(a, b);
}

void solve (char *word, char **words) {
	for (int i = 0; i < NUM_WORDS; ++i) {
		char *a = strdup(word);
		char *b = strdup(words[i]);
		if (anagram(a, b))
			puts(words[i]);
		free(a);
		free(b);
	}
	puts("");
}


int main (int argc, char **argv) {
	size_t dummy;
	char **words = malloc(NUM_WORDS * sizeof(*words));
	memset(words, 0, sizeof(*words)*NUM_WORDS);
	FILE *file = fopen(PATH, "r");
	if(!file)
		return -1;
	for (int i = 0; i < NUM_WORDS; ++i) {
		getline(&words[i], &dummy, file);
		words[i][strlen(words[i])-1] = '\0'; //remove newline
	}
	for (int i = 1; i < argc; ++i)
		solve(argv[i], words);
	if (argc == 1) {
		char *word = NULL;
		while (getline(&word, &dummy, stdin) != -1) {
			word[strlen(word) - 1] = '\0';
			solve(word, words);
			word = NULL;
		}
	}
	return 0;
}
