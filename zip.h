#include <stdio.h>
#include <archive.h>
#include <archive_entry.h>
#include <string.h>
#include "comments.h"

char binnFile[256];
int action;

int anonymize(char const *infile, char *outfile);
int deanonymize(char const *infile, char *outfile);
