#define _GNU_SOURCE //asks stdio.h to include asprintf
#include <stdio.h>
#include "comments.h"
#include "stopif.h"

void printAuthors(const char *authorName, const char *anonName) {
	printf("\"%s\" is now \"%s\"\n", authorName, anonName);
}

char* anonymizeAuthor(binn *anonAuthors, const xmlChar *authorName) {
	static int authorsCount = 0;

	char *name = (char*)authorName;
	char *newName = binn_object_str(anonAuthors, name);

	if (newName)
		return newName;

	asprintf(&newName, "Author%d", ++authorsCount);
	binn_object_set_str(anonAuthors, name, newName);
	binn_object_set_str(anonAuthors, newName, name);
	printAuthors(name, newName);
	free(newName);

	return binn_object_str(anonAuthors, name);
}

void saveAuthors(binn *anonAuthors) {
	FILE *fp = fopen(binnFile, "w");
	fwrite(binn_ptr(anonAuthors), binn_size(anonAuthors), 1, fp);
	fclose(fp);
}

int processAuthors(const xmlXPathObjectPtr authors) {
	binn *anonAuthors = binn_object();

	for (int i=0; i < authors->nodesetval->nodeNr; i++){
		xmlChar *authorName = (xmlChar*)"";		
		authorName = xmlNodeGetContent(authors->nodesetval->nodeTab[i]);
		char *anonAuthor = anonymizeAuthor(anonAuthors, authorName);
		xmlNodeSetContent(authors->nodesetval->nodeTab[i], (xmlChar*)anonAuthor);
		xmlFree(authorName);
	}
	
	saveAuthors(anonAuthors);
	binn_free(anonAuthors);
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
