// Borrowed from https://github.com/b-k/21st-Century-Examples
#include <stdlib.h> //malloc
#include <string.h>
#include <strings.h> //strcasecmp (from POSIX)
#include "keyval.h"

keyval *keyval_new(char *key, void *value){
	keyval *out = malloc(sizeof(keyval));
	*out = (keyval){.key = strdup(key), .value=strdup(value)};
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
	return !strcasecmp(in->key, key);
}
