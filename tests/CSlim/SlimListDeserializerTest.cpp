#include "CppUTest/TestHarness.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>

extern "C"
{
#include "SlimList.h"
#include "CppUTest/TestHarness_c.h"
#include "SlimListDeserializer.h"
#include "SlimListSerializer.h"
}

TEST_GROUP(SlimListDeserializer)
{
    SlimList* slimList;
	SlimList* deserializedList;
	char* serializedList;

    void setup()
    {
		slimList  = SlimList_create();
		serializedList = 0;
		deserializedList = 0;
    }
    
    void teardown()
    {
		SlimList_destroy(slimList);

		if (deserializedList)
			SlimList_destroy(deserializedList);
			
		if (serializedList != 0)
			cpputest_free(serializedList);
    }

	void check_lists_equal(SlimList* expected, SlimList* actual) {
		CHECK(SlimList_equals(expected, actual));	
	}
	
};


TEST(SlimListDeserializer, deserializeEmptyList)
{
	deserializedList = SlimList_deserialize("[000000:]");
	CHECK(deserializedList != 0);
	LONGS_EQUAL(0, SlimList_getLength(deserializedList));
}

TEST(SlimListDeserializer, deserializeNull)
{
	SlimList* list = SlimList_deserialize(0);
	POINTERS_EQUAL(0, list);
}

TEST(SlimListDeserializer, deserializeEmptyString)
{
	SlimList* list = SlimList_deserialize("");
	POINTERS_EQUAL(0, list);
}

TEST(SlimListDeserializer, MissingOpenBracketReturnsNull)
{
	SlimList* list = SlimList_deserialize("hello");
	POINTERS_EQUAL(0, list);	
}

TEST(SlimListDeserializer, MissingClosingBracketReturnsNull)
{
	SlimList* list = SlimList_deserialize("[000000:");
	POINTERS_EQUAL(0, list);	
}

TEST(SlimListDeserializer, canDeserializeCanonicalListWithOneElement) 
{
	char* canonicalList = "[000001:000008:Hi doug.:]";
	SlimList* deserializedList = SlimList_deserialize(canonicalList);
	CHECK(deserializedList != NULL);
	LONGS_EQUAL(1, SlimList_getLength(deserializedList));
	STRCMP_EQUAL("Hi doug.", SlimList_getStringAt(deserializedList, 0));
	SlimList_destroy(deserializedList);
}


TEST(SlimListDeserializer, canDeSerializeListWithOneElement)
{
	SlimList_addString(slimList, "hello");
	serializedList = SlimList_serialize(slimList);	
	deserializedList = SlimList_deserialize(serializedList);
	CHECK(deserializedList != 0);
	check_lists_equal(slimList, deserializedList);
}

TEST(SlimListDeserializer, canDeSerializeListWithTwoElements)
{
	SlimList_addString(slimList, "hello");
	SlimList_addString(slimList, "bob");
	serializedList = SlimList_serialize(slimList);	
	deserializedList = SlimList_deserialize(serializedList);
	CHECK(deserializedList != 0);
	check_lists_equal(slimList, deserializedList);
}

TEST(SlimListDeserializer, canAddSubList)
{
	SlimList* embeddedList;
	embeddedList = SlimList_create();
	SlimList_addString(embeddedList, "element");
	SlimList_addList(slimList, embeddedList);
	serializedList = SlimList_serialize(slimList);	
	deserializedList = SlimList_deserialize(serializedList);
	SlimList * subList = SlimList_getListAt(deserializedList, 0);
	subList = SlimList_getListAt(deserializedList, 0);
	check_lists_equal(embeddedList, subList);
	
	SlimList_destroy(embeddedList);	
}

TEST(SlimListDeserializer, getStringWhereThereIsAList)
{
	SlimList* embeddedList;
	embeddedList = SlimList_create();
	SlimList_addString(embeddedList, "element");
	SlimList_addList(slimList, embeddedList);
	serializedList = SlimList_serialize(slimList);	
	deserializedList = SlimList_deserialize(serializedList);
	char * string = SlimList_getStringAt(deserializedList, 0);

	STRCMP_EQUAL("[000001:000007:element:]", string);
	// POINTERS_EQUAL(0, string); ?????????????????????????????????????
	
	SlimList_destroy(embeddedList);	
}
