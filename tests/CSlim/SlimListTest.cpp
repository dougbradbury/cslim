#include "CppUTest/TestHarness.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>

static int fakeRan = 0;

extern "C"
{
#include "SlimList.h"
#include "CppUTest/TestHarness_c.h"
    void virtualFunction_renameThis_fake(SlimList*)
    {
        fakeRan = 1;
    }
}

TEST_GROUP(SlimList)
{
    SlimList* slimList;
	SlimList* deserializedList;
	char* serializedList;

    void setup()
    {
		slimList  = SlimList_Create();
		fakeRan   = 0;
		serializedList = 0;
		deserializedList = 0;
    }
    
    void teardown()
    {
		SlimList_Destroy(slimList);

		if (deserializedList)
			SlimList_Destroy(deserializedList);
			
		if (serializedList != 0)
			cpputest_free(serializedList);
    }

	void check_lists_equal(SlimList* expected, SlimList* actual) {
		CHECK(SlimList_equals(expected, actual));	
	}
	
};

TEST(SlimList, SerializeAListWithNoElements)
{
	serializedList = SlimList_serialize(slimList);
	STRCMP_EQUAL("[000000:]", serializedList);
}

TEST(SlimList, SerializeAListWithOneElements)
{
	SlimList_addString(slimList, "hello");
	serializedList = SlimList_serialize(slimList);
	STRCMP_EQUAL("[000001:000005:hello:]", serializedList);
}

TEST(SlimList, SerializeAListWithTwoElements)
{
	SlimList_addString(slimList, "hello");
	SlimList_addString(slimList, "world");
	
	serializedList = SlimList_serialize(slimList);

	STRCMP_EQUAL("[000002:000005:hello:000005:world:]", serializedList);
}

TEST(SlimList, ListCopysItsString)
{
	char string[12] = "Hello";
	SlimList_addString(slimList, string);
	strcpy(string, "Goodbye");
	serializedList = SlimList_serialize(slimList);
	STRCMP_EQUAL("[000001:000005:Hello:]", serializedList);
}

TEST(SlimList, SerializeNestedList)
{
	SlimList* embeddedList;
	embeddedList = SlimList_Create();
	SlimList_addString(embeddedList, "element");
	SlimList_addList(slimList, embeddedList);
	serializedList = SlimList_serialize(slimList);
	STRCMP_EQUAL("[000001:000024:[000001:000007:element:]:]", serializedList);
	SlimList_Destroy(embeddedList);
}

TEST(SlimList, serializedLength)
{
	SlimList_addString(slimList, "12345");
	LONGS_EQUAL(5 + 17, SlimList_serializedLength(slimList));
	SlimList_addString(slimList, "123456");
	LONGS_EQUAL(9 + (5+8) + (6+8), SlimList_serializedLength(slimList));
	serializedList = SlimList_serialize(slimList);
	LONGS_EQUAL(9 + (5+8) + (6+8), strlen(serializedList));
}

TEST(SlimList, twoEmptyListsAreEqual)
{
	SlimList* list = SlimList_Create();
	check_lists_equal(slimList, list);
	SlimList_Destroy(list);	
}

TEST(SlimList, twoDifferentLenghtListsAreNotEqual)
{
	SlimList* list = SlimList_Create();
	SlimList_addString(slimList, "hello");
	CHECK(!SlimList_equals(slimList, list));
	SlimList_Destroy(list);	
}

TEST(SlimList, twoSingleElementListsWithDifferentElmementsAreNotEqual)
{
	SlimList* list = SlimList_Create();
	SlimList_addString(slimList, "hello");
	SlimList_addString(list, "goodbye");
	CHECK(!SlimList_equals(slimList, list));
	SlimList_Destroy(list);	
}


TEST(SlimList, twoIdenticalMultipleElementListsElmementsAreEqual)
{
	SlimList* list = SlimList_Create();
	SlimList_addString(slimList, "hello");
	SlimList_addString(slimList, "goodbye");
	SlimList_addString(list, "hello");
	SlimList_addString(list, "goodbye");
	CHECK(SlimList_equals(slimList, list));
	SlimList_Destroy(list);	
}


TEST(SlimList, twoNonIdenticalMultipleElementListsElmementsAreNotEqual)
{
	SlimList* list = SlimList_Create();
	SlimList_addString(slimList, "hello");
	SlimList_addString(slimList, "hello");
	SlimList_addString(list, "hello");
	SlimList_addString(list, "goodbye");
	CHECK(!SlimList_equals(slimList, list));
	SlimList_Destroy(list);	
}


TEST(SlimList, deserializeEmptyList)
{
	deserializedList = SlimList_deserialize("[000000:]");
	CHECK(deserializedList != 0);
	LONGS_EQUAL(0, SlimList_getLength(deserializedList));
}

TEST(SlimList, deserializeNull)
{
	SlimList* list = SlimList_deserialize(0);
	POINTERS_EQUAL(0, list);
}

TEST(SlimList, deserializeEmptyString)
{
	SlimList* list = SlimList_deserialize("");
	POINTERS_EQUAL(0, list);
}

TEST(SlimList, MissingOpenBracketReturnsNull)
{
	SlimList* list = SlimList_deserialize("hello");
	POINTERS_EQUAL(0, list);	
}

TEST(SlimList, MissingClosingBracketReturnsNull)
{
	SlimList* list = SlimList_deserialize("[000000:");
	POINTERS_EQUAL(0, list);	
}


TEST(SlimList, canDeSerializeListWithOneElement)
{
	SlimList_addString(slimList, "hello");
	serializedList = SlimList_serialize(slimList);	
	deserializedList = SlimList_deserialize(serializedList);
	CHECK(deserializedList != 0);
	check_lists_equal(slimList, deserializedList);
}

TEST(SlimList, canDeSerializeListWithTwoElements)
{
	SlimList_addString(slimList, "hello");
	SlimList_addString(slimList, "bob");
	serializedList = SlimList_serialize(slimList);	
	deserializedList = SlimList_deserialize(serializedList);
	CHECK(deserializedList != 0);
	check_lists_equal(slimList, deserializedList);
}

TEST(SlimList, canAddSubList)
{
	SlimList* embeddedList;
	embeddedList = SlimList_Create();
	SlimList_addString(embeddedList, "element");
	SlimList_addList(slimList, embeddedList);
	serializedList = SlimList_serialize(slimList);	
	deserializedList = SlimList_deserialize(serializedList);
	SlimList * subList = SlimList_getListAt(deserializedList, 0);
	subList = SlimList_getListAt(deserializedList, 0);
	check_lists_equal(embeddedList, subList);
	
	SlimList_Destroy(embeddedList);	
}

TEST(SlimList, canGetElements)
{
	SlimList_addString(slimList, "element1");
	SlimList_addString(slimList, "element2");
	STRCMP_EQUAL("element1", SlimList_getStringAt(slimList, 0));
	STRCMP_EQUAL("element2", SlimList_getStringAt(slimList, 1));
}

TEST(SlimList, cannotGetElementThatAreNotThere)
{
	SlimList_addString(slimList, "element1");
	SlimList_addString(slimList, "element2");
	POINTERS_EQUAL(0, SlimList_getStringAt(slimList, 3));
}

TEST(SlimList, getStringWhereThereIsAList)
{
	SlimList* embeddedList;
	embeddedList = SlimList_Create();
	SlimList_addString(embeddedList, "element");
	SlimList_addList(slimList, embeddedList);
	serializedList = SlimList_serialize(slimList);	
	deserializedList = SlimList_deserialize(serializedList);
	char * string = SlimList_getStringAt(deserializedList, 0);

	STRCMP_EQUAL("[000001:000007:element:]", string);
	// POINTERS_EQUAL(0, string); ?????????????????????????????????????
	
	SlimList_Destroy(embeddedList);	
}
