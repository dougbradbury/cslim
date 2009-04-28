#include <string.h>
#include <stdlib.h>
char * buyBuf(char* buffer, int length)
{
	char * purchase = malloc(length+1);
	strncpy(purchase, buffer, length);
	purchase[length] = 0;
	return purchase;
}
char * buyString(char* string)
{
	return buyBuf(string, strlen(string));
}
