#include <stdio.h>
#include "zip.h"

int deanonymize;

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: %s <path_to_DOCX>\n", argv[0]);
		printf("Optionaly provide output file as second argument.\n");
		printf("-d as second argument will deanonymize given file. You can optionaly provide output file as third argument.\n");
		return 0;
	}

	if (argc > 2) {
		if (strcmp(argv[2], "-d") == 0) {
			deanonymize = 1;

			if (argc > 3) {
				process(argv[1], argv[3]);
				return 0;
			}

			process(argv[1], NULL);
			return 0;
		}
		
		process(argv[1], argv[2]);
	}
	else
		process(argv[1], NULL);
}
