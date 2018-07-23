#ifndef SLIMUTILS_H
#define SLIMUTILS_H

#ifndef CPP_COMPILING
#ifdef __cplusplus
extern "C" {
#endif
#endif

const char * CSlim_BuyBuf(char const*, int);
const char * CSlim_BuyString(char const*);

const char* CSlim_CreateEmptyString(void);
void CSlim_ConcatenateString(const char**, const char*);
int CSlim_StringStartsWith(const char* string, const char* prefix);
void CSlim_DestroyString(const char*);

typedef struct MapStringInt
{
	const char* string;
	int n;
} MapStringInt;

int CSlim_MapToIntFrom(MapStringInt*, const char*);
const char* CSlim_MapToStringFrom(MapStringInt*, int);
int CSlim_IsCharacter(unsigned char const* byte);

#ifndef CPP_COMPILING
#ifdef __cplusplus
}
#endif
#endif

#endif
