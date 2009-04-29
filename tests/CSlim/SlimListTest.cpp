#include "CppUTest/TestHarness.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>

extern "C"
{
#include "SlimList.h"
#include "CppUTest/TestHarness_c.h"
}

TEST_GROUP(SlimList)
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

TEST(SlimList, twoEmptyListsAreEqual)
{
	SlimList* list = SlimList_create();
	check_lists_equal(slimList, list);
	SlimList_destroy(list);	
}

TEST(SlimList, twoDifferentLenghtListsAreNotEqual)
{
	SlimList* list = SlimList_create();
	SlimList_addString(slimList, "hello");
	CHECK(!SlimList_equals(slimList, list));
	SlimList_destroy(list);	
}

TEST(SlimList, twoSingleElementListsWithDifferentElmementsAreNotEqual)
{
	SlimList* list = SlimList_create();
	SlimList_addString(slimList, "hello");
	SlimList_addString(list, "goodbye");
	CHECK(!SlimList_equals(slimList, list));
	SlimList_destroy(list);	
}


TEST(SlimList, twoIdenticalMultipleElementListsElmementsAreEqual)
{
	SlimList* list = SlimList_create();
	SlimList_addString(slimList, "hello");
	SlimList_addString(slimList, "goodbye");
	SlimList_addString(list, "hello");
	SlimList_addString(list, "goodbye");
	CHECK(SlimList_equals(slimList, list));
	SlimList_destroy(list);	
}


TEST(SlimList, twoNonIdenticalMultipleElementListsElmementsAreNotEqual)
{
	SlimList* list = SlimList_create();
	SlimList_addString(slimList, "hello");
	SlimList_addString(slimList, "hello");
	SlimList_addString(list, "hello");
	SlimList_addString(list, "goodbye");
	CHECK(!SlimList_equals(slimList, list));
	SlimList_destroy(list);	
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

TEST(SlimList, canReplaceString)
{
	SlimList_addString(slimList, "replaceMe");
	SlimList_replaceAt(slimList, 0, "WithMe");
	
	STRCMP_EQUAL("WithMe", SlimList_getStringAt(slimList, 0));
}
