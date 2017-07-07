#include <stdio.h>
#include "zip.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: %s <path_to_DOCX>\n", argv[0]);
		printf("Optionaly provide output file as second argument.\n");
	}
		
	if (argc > 2)
		process(argv[1], argv[2]);
	else if (argc > 1)
		process(argv[1], NULL);
}
