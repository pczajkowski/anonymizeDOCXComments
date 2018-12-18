#include <stdlib.h>
#include <stdio.h>
#include "xmlbuff.h"

XMLBuff *XMLBuffNew(void) {
	XMLBuff *out = malloc(sizeof(XMLBuff));
	if (out == NULL) {
		puts("Couldn't allocate memory for XMLBuff!");
		return NULL;
	}
	
	*out = (XMLBuff){ .data=NULL };                          
	return out;
}

void XMLBuffFree(XMLBuff *in) {
	free(in->data);
	free(in);
}
