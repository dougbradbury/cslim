#include <string.h>
#include <stdlib.h>
#include "SlimUtil.h"
#include <assert.h>

char * CSlim_BuyBuf(char const* buffer, int length)
{
	if (buffer == NULL)
		return NULL;

	char * purchase = (char*)malloc(length+1);
	strncpy(purchase, buffer, length);
	purchase[length] = 0;
	return purchase;
}
char * CSlim_BuyString(char const* string)
{
	if (string == NULL)
		return NULL;
	return CSlim_BuyBuf(string, (int)strlen(string));
}

char* CSlim_CreateEmptyString(void) {
	char* result = malloc(sizeof(char));
	assert(result != NULL);

	result[0] = '\0';
	return result;
}

void CSlim_DestroyString(char* string) {
	free(string);
}

//Note: the location of toAppendTo may change as a result of calling this function. The old location should no longer be used.
void CSlim_ConcatenateString(char** toAppendTo, const char* toAppend) {
	size_t requiredLength = strlen(*toAppendTo) + strlen(toAppend) + 1;

	char *temp = realloc(*toAppendTo, requiredLength * sizeof(char));
	assert(temp != NULL);

	strcat(temp, toAppend);

	*toAppendTo = temp;
}

int CSlim_MapToIntFrom(MapStringInt* map, const char* name)
{
	MapStringInt* p = map;
	while (p->string != NULL && 0 != strcmp(name, p->string))
	{
		p++;
	}

	return p->n;
}

const char* CSlim_MapToStringFrom(MapStringInt* map, int n)
{
	MapStringInt* p = map;
	while (p->string != NULL && n != p->n)
	{
		p++;
	}

	return p->string;
}

int CSlim_IsCharacter(unsigned char const* byte)
{
  if ((*byte < (unsigned char const)0x80) || (*byte > (unsigned char const)0xBF))
    return 1;
  return 0;
}
