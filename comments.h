#include <libxml2/libxml/xpath.h>
#include <libxml2/libxml/xpathInternals.h>
#include "dict.h"
#include "xmlbuff.h"

char* anonymizeAuthor(dictionary *authors, xmlChar const *authorName);
void printAuthors(dictionary *authors);
int processAuthors(xmlXPathObjectPtr authors);
int anonymizeComments(XMLBuff *infile);