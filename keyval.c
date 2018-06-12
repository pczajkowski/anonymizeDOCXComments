// Borrowed from https://github.com/b-k/21st-Century-Examples
#include <stdlib.h> //malloc
#include <string.h>
#include "keyval.h"

keyval *keyval_new(char *key, char *value){
	keyval *out = malloc(sizeof(keyval));
	out->key = malloc(strlen(key)+1);
	out->value = malloc(strlen(value)+1);

	strcpy(out->key, key);
	strcpy(out->value, value);

	return out;
}

/** Copy a key/value pair. The new pair has pointers to
  the values in the old pair, not copies of their data.  */
keyval *keyval_copy(keyval const *in){
	keyval *out = malloc(sizeof(keyval));
	*out = *in;
	return out;
}

void keyval_free(keyval *in){
	free(in->key);
	free(in->value);
	free(in);
}

int keyval_matches(keyval const *in, char const *key){
	return !strcmp(in->key, key);
}
