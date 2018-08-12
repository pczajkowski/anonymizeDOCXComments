#include <stdio.h>
#include <archive.h>
#include <archive_entry.h>
#include <string.h>
#include "comments.h"

extern char binnFile[256];

int process(char const *infile, char *outfile);
