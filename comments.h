#include <libxml2/libxml/xpath.h>
#include <libxml2/libxml/xpathInternals.h>
#include <binn.h>
#include "xmlbuff.h"

#define ANONYMIZE 0
#define DEANONYMIZE 1

extern char binnFile[256];
extern int action;

int anonymizeComments(XMLBuff *infile);
