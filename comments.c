#define _GNU_SOURCE //asks stdio.h to include asprintf
#include <stdio.h>
#include "comments.h"
#include "stopif.h"

char* anonymizeAuthor(dictionary *authors, const xmlChar *authorName) {
	char *name = (char*)authorName;
	char *newName = (char*)dictionary_find(authors, name);

	if (newName)
		return newName;

	asprintf(&newName, "Author%d", authors->length+1);
	dictionary_add(authors, name, newName);
	free(newName);
	return (char*)dictionary_find(authors, name);
}

void printAuthors(const dictionary *authors) {
	for (int i=0; i<authors->length; i++)
		printf("\"%s\" is now \"%s\"\n", authors->pairs[i]->key, (char*)authors->pairs[i]->value);
}

int processAuthors(const xmlXPathObjectPtr authors) {
	dictionary *anonAuthors = dictionary_new();

	for (int i=0; i < authors->nodesetval->nodeNr; i++){
		xmlChar *authorName = (xmlChar*)"";		
		authorName = xmlNodeGetContent(authors->nodesetval->nodeTab[i]);
		char *anonAuthor = anonymizeAuthor(anonAuthors, authorName);
		xmlNodeSetContent(authors->nodesetval->nodeTab[i], (xmlChar*)anonAuthor);
		xmlFree(authorName);
	}

	printAuthors(anonAuthors);
	dictionary_free(anonAuthors);
	return 1;
}

int anonymizeComments(XMLBuff *infile) {
	const xmlChar *authorPath = (xmlChar*)"//w:comment/@w:author";

	xmlDocPtr doc = xmlReadMemory(infile->data, infile->size, infile->name, NULL, 0);
	Stopif(!doc, return 0, "Unable to parse file %s!\n", infile->name);

	xmlXPathContextPtr context = xmlXPathNewContext(doc);
	Stopif(!context, return 0, "Unable to create new XPath context!\n");

	const xmlChar* prefix = (xmlChar*)"w";
	const xmlChar* ns = (xmlChar*)"http://schemas.openxmlformats.org/wordprocessingml/2006/main";
	Stopif(xmlXPathRegisterNs(context, prefix, ns), return 0, "Can't add namespace!\n");

	xmlXPathObjectPtr authors = xmlXPathEvalExpression(authorPath, context);
	Stopif(!authors, return 0, "Something is wrong with XPATH %s!\n", authorPath);

	Stopif(!processAuthors(authors), return 0, "Can't process authors!\n");

	xmlChar *buf;
	xmlDocDumpMemoryEnc(doc, &buf, &infile->size, "UTF-8");
	infile->data = (char*)buf;

	Stopif(!infile->size, return 0, "Unable to save file %s!\n", infile->name);

	xmlXPathFreeObject(authors);
	xmlXPathFreeContext(context);
	xmlFreeDoc(doc);
	xmlCleanupParser();

	return 1;
}
