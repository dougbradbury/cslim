#ifndef SLIMUTILS_H
#define SLIMUTILS_H
char * CSlim_BuyBuf(char*, int);
char * CSlim_BuyString(char*);

typedef struct MapStringInt
{
	const char* string;
	int n;
} MapStringInt;

int CSlim_MapToIntFrom(MapStringInt*, const char*);
const char* CSlim_MapToStringFrom(MapStringInt*, int);
#endif
