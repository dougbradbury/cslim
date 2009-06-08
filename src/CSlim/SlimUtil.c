#include <string.h>
#include <stdlib.h>
char * buyBuf(char* buffer, int length)
{
	if (buffer == NULL)
		return NULL;
		
	char * purchase = (char*)malloc(length+1);
	strncpy(purchase, buffer, length);
	purchase[length] = 0;
	return purchase;
}
char * buyString(char* string)
{
	if (string == NULL)
		return NULL;
	return buyBuf(string, strlen(string));
}
