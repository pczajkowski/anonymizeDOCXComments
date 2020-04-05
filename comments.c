#include <stdio.h>
#include "comments.h"
#include "stopif.h"

#define NAME_LENGTH 20

void printAuthors(const char *authorName, const char *anonName) {
	printf("\"%s\" is now \"%s\"\n", authorName, anonName);
}

char* anonymizeAuthor(binn *anonAuthors, const xmlChar *authorName) {
	static int authorsCount = 0;

	char *name = (char*)authorName;
	char *newName = binn_object_str(anonAuthors, name);

	if (newName)
		return newName;
	
	newName = malloc(NAME_LENGTH);
	if (newName == NULL) {
		printf("Couldn't allocate memory for %s!\n", name);
		return NULL;
	}

	snprintf(newName, NAME_LENGTH,"Author%d", ++authorsCount);
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

int anonymizeAuthors(const xmlXPathObjectPtr authors) {
	binn *anonAuthors = binn_object();

	for (int i=0; i < authors->nodesetval->nodeNr; i++){
		xmlChar *authorName = (xmlChar*)"";		
		authorName = xmlNodeGetContent(authors->nodesetval->nodeTab[i]);
		char *anonAuthor = anonymizeAuthor(anonAuthors, authorName);
		if (anonAuthor == NULL) {
			printf("Couldn't anonymize %s!\n", authorName);
			return 0;
		}
		
		xmlNodeSetContent(authors->nodesetval->nodeTab[i], (xmlChar*)anonAuthor);
		xmlFree(authorName);
	}
	
	saveAuthors(anonAuthors);
	binn_free(anonAuthors);
	return 1;
}

char *data;

binn *readAuthors() {
	FILE *fp = fopen(binnFile, "rb");
	if (fp == NULL) {
		printf("Couldn't open bin file (%s)!\n", binnFile);
		return NULL;
	}

        fseek(fp, 0, SEEK_END);
        long fsize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        data = malloc(fsize + 1);
	if (data == NULL) {
		puts("Couldn't allocate memory for data!");
		return NULL;
	}
	
        size_t result = fread(data, fsize, 1, fp);
	if (result != 1) {
		printf("Couldn't read bin file (%s)!\n", binnFile);
		return NULL;
	}

        fclose(fp);

        data[fsize] = 0;

        binn *obj = binn_open(data);
	
	return obj;
}

int deanonymizeAuthors(const xmlXPathObjectPtr authors) {
	binn *anonAuthors = readAuthors();
	if (anonAuthors == NULL) return 0;
	
	for (int i=0; i < authors->nodesetval->nodeNr; i++){
		xmlChar *anonName = (xmlChar*)"";		
		anonName = xmlNodeGetContent(authors->nodesetval->nodeTab[i]);
		
		char *author = binn_object_str(anonAuthors, (char*)anonName);
		if (author != NULL) {
			xmlNodeSetContent(authors->nodesetval->nodeTab[i], (xmlChar*)author);
			printAuthors((char*)anonName, author);
		}
		
		xmlFree(anonName);
	}
	
	free(data);
	binn_free(anonAuthors);
	return 1;
}

int processAuthors(const xmlXPathObjectPtr authors) {
	if (deanonymize)
		return deanonymizeAuthors(authors);
	
	return anonymizeAuthors(authors);
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
