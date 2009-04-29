#include "CppUTest/TestHarness.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>


extern "C"
{
#include "SlimList.h"
#include "CppUTest/TestHarness_c.h"
#include "SlimListSerializer.h"
}

TEST_GROUP(SlimListSerializer)
{
    SlimList* slimList;
	char* serializedList;

    void setup()
    {
		slimList  = SlimList_create();
		serializedList = 0;
    }
    
    void teardown()
    {
		SlimList_destroy(slimList);

		if (serializedList != 0)
			cpputest_free(serializedList);
    }
	
};

TEST(SlimListSerializer, SerializeAListWithNoElements)
{
	serializedList = SlimList_serialize(slimList);
	STRCMP_EQUAL("[000000:]", serializedList);
}

TEST(SlimListSerializer, SerializeAListWithOneElements)
{
	SlimList_addString(slimList, "hello");
	serializedList = SlimList_serialize(slimList);
	STRCMP_EQUAL("[000001:000005:hello:]", serializedList);
}

TEST(SlimListSerializer, SerializeAListWithTwoElements)
{
	SlimList_addString(slimList, "hello");
	SlimList_addString(slimList, "world");
	
	serializedList = SlimList_serialize(slimList);

	STRCMP_EQUAL("[000002:000005:hello:000005:world:]", serializedList);
}

TEST(SlimListSerializer, ListCopysItsString)
{
	char string[12] = "Hello";
	SlimList_addString(slimList, string);
	strcpy(string, "Goodbye");
	serializedList = SlimList_serialize(slimList);
	STRCMP_EQUAL("[000001:000005:Hello:]", serializedList);
}

TEST(SlimListSerializer, canCopyAList)
{
	SlimList_addString(slimList, "123456");
	SlimList_addString(slimList, "987654");

	SlimList* copy = SlimList_create();
	int i;
	for (i=0; i<SlimList_getLength(slimList); i++) {
		char* string = SlimList_getStringAt(slimList, i);
		SlimList_addString(copy, string);
	}
	char * serialCopy = SlimList_serialize(copy);
	char * serialSlimList = SlimList_serialize(slimList);
	STRCMP_EQUAL(serialCopy, serialSlimList);
	
	SlimList_destroy(copy);
	cpputest_free(serialSlimList);
	cpputest_free(serialCopy);
	
}

TEST(SlimListSerializer, SerializeNestedList)
{
	SlimList* embeddedList;
	embeddedList = SlimList_create();
	SlimList_addString(embeddedList, "element");
	SlimList_addList(slimList, embeddedList);
	serializedList = SlimList_serialize(slimList);
	STRCMP_EQUAL("[000001:000024:[000001:000007:element:]:]", serializedList);
	SlimList_destroy(embeddedList);
}

TEST(SlimListSerializer, serializedLength)
{
	SlimList_addString(slimList, "12345");
	LONGS_EQUAL(5 + 17, SlimList_serializedLength(slimList));
	SlimList_addString(slimList, "123456");
	LONGS_EQUAL(9 + (5+8) + (6+8), SlimList_serializedLength(slimList));
	serializedList = SlimList_serialize(slimList);
	LONGS_EQUAL(9 + (5+8) + (6+8), strlen(serializedList));
}

