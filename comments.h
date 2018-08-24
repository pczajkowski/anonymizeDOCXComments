#include <libxml2/libxml/xpath.h>
#include <libxml2/libxml/xpathInternals.h>
#include <binn.h>
#include "xmlbuff.h"

extern char binnFile[256];
extern int deanonymize;

int anonymizeComments(XMLBuff *infile);
