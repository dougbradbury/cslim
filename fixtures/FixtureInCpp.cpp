#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SlimList.h"
#include "Fixtures.h"

class cMultiplication 
{
public:
	cMultiplication(){};
	~cMultiplication(){};
	float product()
	{
		return m1*m2;
	}
	float m1;
	float m2;
	char result[32];	
};

#ifndef CPP_COMPILING
extern "C" {
#endif
typedef struct Multiplication
{
	cMultiplication multiplication;
	char result[32];
} Multiplication;

void* Multiplication_Create(StatementExecutor* errorHandler, SlimList* args)
{
	Multiplication* self = (Multiplication*)malloc(sizeof(Multiplication));
	self->result[0] = 0;
  // self->multiplication = new cMultiplication();
	return self;
}

void Multiplication_Destroy(void* void_self)
{
	Multiplication* self = (Multiplication*)void_self;
  // delete self->multiplication;
	free(self);
}

static char* setMultiplicand1(void* void_self, SlimList* args) {
	Multiplication* self = (Multiplication*)void_self;
	self->multiplication.m1 = atof(SlimList_GetStringAt(args, 0));
	return self->result;
}

static char* setMultiplicand2(void* void_self, SlimList* args) {
	Multiplication* self = (Multiplication*)void_self;
	self->multiplication.m2 = atof(SlimList_GetStringAt(args, 0));
	return self->result;
}

static char* Product(void* void_self, SlimList* args) {
	Multiplication* self = (Multiplication*)void_self;
	float product = self->multiplication.product();
	snprintf(self->result, 32, "%g", product);
	return self->result;
}


SLIM_CREATE_FIXTURE(Multiplication) 
	SLIM_FUNCTION(setMultiplicand1)
	SLIM_FUNCTION(setMultiplicand2)
	SLIM_FUNCTION(Product)
SLIM_END

#ifndef CPP_COMPILING
}
#endif  
