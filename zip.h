#include <stdio.h>
#include <archive.h>
#include <archive_entry.h>
#include <string.h>
#include "comments.h"

int processComments(struct archive *archiveOut, XMLBuff *comments);
int rewriteZIP(struct archive *archiveIn, struct archive *archiveOut);
int processDOCX(char const *infile, char const *outfile);
int process(char const *infile, char *outfile);