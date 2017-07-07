#define _GNU_SOURCE //asks stdio.h to include asprintf
#include <stdio.h>
#include <libxml2/libxml/xpath.h>
#include <libxml2/libxml/xpathInternals.h>
#include "stopif.h"
#include "xmlbuff.h"
#include "dict.h"

static char* anonymizeAuthor(dictionary *authors, xmlChar const *authorName) {
	char *name = (char*)authorName;
	char *newName = (char*)dictionary_find(authors, name);

	if (newName)
		return newName;

	asprintf(&newName, "Author%d", authors->length+1);
	dictionary_add(authors, name, newName);
	return newName;
}

static void printAuthors(dictionary *authors) {
	for (int i=0; i<authors->length; i++)
		printf("\"%s\" is now \"%s\"\n", authors->pairs[i]->key, (char*)authors->pairs[i]->value);
}

int anonymizeComments(XMLBuff *infile) {
	dictionary *anonAuthors = dictionary_new();
	const xmlChar *authorPath = (xmlChar*)"//w:comment/@w:author";

	xmlDocPtr doc = xmlReadMemory(infile->data, infile->size, infile->name, NULL, 0);
	Stopif(!doc, return -1, "Unable to parse file %s!\n", infile->name);

	xmlXPathContextPtr context = xmlXPathNewContext(doc);
	Stopif(!context, return -1, "Unable to create new XPath context!\n");

	const xmlChar* prefix = (xmlChar*)"w";
	const xmlChar* ns = (xmlChar*)"http://schemas.openxmlformats.org/wordprocessingml/2006/main";
	Stopif(xmlXPathRegisterNs(context, prefix, ns), return -1, "Can't add namespace!\n");

	xmlXPathObjectPtr authors = xmlXPathEvalExpression(authorPath, context);
	Stopif(!authors, return -1, "Something is wrong with XPATH %s!\n", authorPath);

	xmlChar *authorName = (xmlChar*)"";
	for (int i=0; i < authors->nodesetval->nodeNr; i++){
		authorName = xmlNodeGetContent(authors->nodesetval->nodeTab[i]);
		char *anonAuthor = anonymizeAuthor(anonAuthors, authorName);
		xmlNodeSetContent(authors->nodesetval->nodeTab[i], (xmlChar*)anonAuthor);
	}

	xmlChar *buf;
	xmlDocDumpMemoryEnc(doc, &buf, &infile->size, "UTF-8");
	infile->data = (char*)buf;

	Stopif(!infile->size, return -1, "Unable to save file %s!\n", infile->name);

	xmlXPathFreeObject(authors);
	xmlXPathFreeContext(context);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	printAuthors(anonAuthors);
	xmlFree(authorName);
	dictionary_free(anonAuthors);
	return 0;
}
