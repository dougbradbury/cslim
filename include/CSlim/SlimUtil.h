#ifndef SLIMUTILS_H
#define SLIMUTILS_H

#ifdef __cplusplus
extern "C" {
#endif

char * CSlim_BuyBuf(char const*, int);
char * CSlim_BuyString(char const*);

char* CSlim_CreateEmptyString(void);
void CSlim_ConcatenateString(char**, const char*);
void CSlim_DestroyString(char*);

typedef struct MapStringInt
{
	const char* string;
	int n;
} MapStringInt;

int CSlim_MapToIntFrom(MapStringInt*, const char*);
const char* CSlim_MapToStringFrom(MapStringInt*, int);
int CSlim_IsCharacter(unsigned char const* byte);

#ifdef __cplusplus
}
#endif

#endif
