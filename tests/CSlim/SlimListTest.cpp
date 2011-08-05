#include <stdlib.h>
#include <string.h>
#include <iostream>

extern "C"
{
#include "SlimList.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP(SlimList)
{
    SlimList* slimList;
	SlimList* deserializedList;
	char* serializedList;

    void setup()
    {
		slimList  = SlimList_Create();
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
		CHECK(SlimList_Equals(expected, actual));
	}

};

TEST(SlimList, twoEmptyListsAreEqual)
{
	SlimList* list = SlimList_Create();
	check_lists_equal(slimList, list);
	SlimList_Destroy(list);
}

TEST(SlimList, twoDifferentLenghtListsAreNotEqual)
{
	SlimList* list = SlimList_Create();
	SlimList_AddString(slimList, "hello");
	CHECK(!SlimList_Equals(slimList, list));
	SlimList_Destroy(list);
}

TEST(SlimList, twoSingleElementListsWithDifferentElmementsAreNotEqual)
{
	SlimList* list = SlimList_Create();
	SlimList_AddString(slimList, "hello");
	SlimList_AddString(list, "goodbye");
	CHECK(!SlimList_Equals(slimList, list));
	SlimList_Destroy(list);
}


TEST(SlimList, twoIdenticalMultipleElementListsElmementsAreEqual)
{
	SlimList* list = SlimList_Create();
	SlimList_AddString(slimList, "hello");
	SlimList_AddString(slimList, "goodbye");
	SlimList_AddString(list, "hello");
	SlimList_AddString(list, "goodbye");
	CHECK(SlimList_Equals(slimList, list));
	SlimList_Destroy(list);
}


TEST(SlimList, twoNonIdenticalMultipleElementListsElmementsAreNotEqual)
{
	SlimList* list = SlimList_Create();
	SlimList_AddString(slimList, "hello");
	SlimList_AddString(slimList, "hello");
	SlimList_AddString(list, "hello");
	SlimList_AddString(list, "goodbye");
	CHECK(!SlimList_Equals(slimList, list));
	SlimList_Destroy(list);
}


TEST(SlimList, canGetElements)
{
	SlimList_AddString(slimList, "element1");
	SlimList_AddString(slimList, "element2");
	STRCMP_EQUAL("element1", SlimList_GetStringAt(slimList, 0));
	STRCMP_EQUAL("element2", SlimList_GetStringAt(slimList, 1));
}

TEST(SlimList, canGetHashWithOneElement)
{
	SlimList_AddString(slimList, "<table><tr><td>name</td><td>bob</td></tr></table>");

	SlimList * hash = SlimList_GetHashAt(slimList, 0);
     SlimList * twoElementList = SlimList_GetListAt(hash, 0);

     STRCMP_EQUAL("name", SlimList_GetStringAt(twoElementList, 0));
     STRCMP_EQUAL("bob", SlimList_GetStringAt(twoElementList, 1));
     SlimList_Destroy(hash);
}

TEST(SlimList, canGetHashWithMultipleElements)
{
     SlimList_AddString(slimList, "<table><tr><td>name</td><td>dough</td></tr><tr><td>addr</td><td>here</td></tr></table>");

     SlimList * hash = SlimList_GetHashAt(slimList, 0);
     SlimList * twoElementList = SlimList_GetListAt(hash,1);

     STRCMP_EQUAL("addr", SlimList_GetStringAt(twoElementList, 0));
     STRCMP_EQUAL("here", SlimList_GetStringAt(twoElementList, 1));
     SlimList_Destroy(hash);
}

TEST(SlimList, cannotGetElementThatAreNotThere)
{
	SlimList_AddString(slimList, "element1");
	SlimList_AddString(slimList, "element2");
	POINTERS_EQUAL(0, SlimList_GetStringAt(slimList, 3));
}

TEST(SlimList, canReplaceString)
{
	SlimList_AddString(slimList, "replaceMe");
	SlimList_ReplaceAt(slimList, 0, "WithMe");

	STRCMP_EQUAL("WithMe", SlimList_GetStringAt(slimList, 0));
}

TEST(SlimList, canGetTail)
{
  SlimList_AddString(slimList, "1");
  SlimList_AddString(slimList, "2");
  SlimList_AddString(slimList, "3");
  SlimList_AddString(slimList, "4");

  SlimList* expected = SlimList_Create();
  SlimList_AddString(expected, "3");
  SlimList_AddString(expected, "4");

  SlimList* tail = SlimList_GetTailAt(slimList, 2);
  CHECK(SlimList_Equals(expected, tail));
  SlimList_Destroy(tail);
  SlimList_Destroy(expected);
}

TEST(SlimList, recursiveToString)
{
 	SlimList_AddString(slimList, "a");
	SlimList_AddString(slimList, "b");

  SlimList* sublist = SlimList_Create();
  SlimList_AddString(sublist, "3");
  SlimList_AddString(sublist, "4");

  SlimList_AddList(slimList, sublist);

  STRCMP_EQUAL("[\"a\", \"b\", [\"3\", \"4\"]]", SlimList_ToString(slimList));

  SlimList_Destroy(sublist);
}

TEST(SlimList, getDouble)
{
  SlimList_AddString(slimList, "2.3");
  DOUBLES_EQUAL(2.3, SlimList_GetDoubleAt(slimList, 0), 0.1);
}
