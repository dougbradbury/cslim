#include <stdlib.h>
#include <string.h>
#include <iostream>

extern "C"
{
#include "SlimList.h"
#include "SlimUtil.h"
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

TEST(SlimList, getDouble)
{
  SlimList_AddString(slimList, "2.3");
  DOUBLES_EQUAL(2.3, SlimList_GetDoubleAt(slimList, 0), 0.1);
}

TEST(SlimList, ToStringForEmptyList)
{
	char* listAsAString = SlimList_ToString(slimList);
	STRCMP_EQUAL("[]", listAsAString);
	CSlim_DestroyString(listAsAString);
}

TEST(SlimList, toStringForSimpleList)
{
	SlimList_AddString(slimList, "a");
	SlimList_AddString(slimList, "b");
	char* stringVersionOfList = SlimList_ToString(slimList);

	STRCMP_EQUAL("[\"a\", \"b\"]", stringVersionOfList);

	CSlim_DestroyString(stringVersionOfList);
}

TEST(SlimList, toStringDoesNotHaveASideEffectWhichChangesResultsFromPriorCalls)
{
	char* priorString = SlimList_ToString(slimList);

	SlimList_AddString(slimList, "a");
	char* listWithAnElementAsASting = SlimList_ToString(slimList);

	int areStringsTheSame = (strcmp(priorString, listWithAnElementAsASting) == 0);
	CHECK_FALSE(areStringsTheSame);

	CSlim_DestroyString(priorString);
	CSlim_DestroyString(listWithAnElementAsASting);
}

TEST(SlimList, recursiveToString)
{
	SlimList_AddString(slimList, "a");
	SlimList_AddString(slimList, "b");

	SlimList* sublist = SlimList_Create();
	SlimList_AddString(sublist, "3");
	SlimList_AddString(sublist, "4");

	SlimList_AddList(slimList, sublist);

	char* listAsAString = SlimList_ToString(slimList);
	STRCMP_EQUAL("[\"a\", \"b\", [\"3\", \"4\"]]", listAsAString);
	CSlim_DestroyString(listAsAString);

	SlimList_Destroy(sublist);
}

TEST(SlimList, toStringForLongList)
{
	const int entries = 128; //TODO: consider updating the size given its no longer as relevant
	int i;

	for (i = 0; i < entries; i++)
	{
		SlimList_AddString(slimList, "a");
	}

	char* listAsAString = SlimList_ToString(slimList);
	CSlim_DestroyString(listAsAString);
}

TEST(SlimList, CanPopHeadOnListWithOneEntry)
{
	SlimList_AddString(slimList, "a");
	SlimList_PopHead(slimList);
	CHECK_EQUAL(0, SlimList_GetLength(slimList));
}

TEST(SlimList, CanInsertAfterPoppingListWithEntries)
{
	SlimList_AddString(slimList, "a");
	SlimList_AddString(slimList, "a");

	SlimList_PopHead(slimList);
	SlimList_PopHead(slimList);

	SlimList_AddString(slimList, "a");
	SlimList_AddString(slimList, "a");
	CHECK_EQUAL(2, SlimList_GetLength(slimList));
}

TEST(SlimList, iteratorDoesNotHaveAnItemWhenEmpty)
{
	SlimListIterator* iterator = SlimList_CreateIterator(slimList);
	CHECK_FALSE(SlimList_Iterator_HasItem(iterator));
}

TEST(SlimList, iteratorHasItem)
{
	SlimList_AddString(slimList, "a");
	SlimListIterator* iterator = SlimList_CreateIterator(slimList);

	CHECK_TRUE(SlimList_Iterator_HasItem(iterator));
}

TEST(SlimList, iteratorNext)
{
	SlimList_AddString(slimList, "a");
	SlimListIterator* iterator = SlimList_CreateIterator(slimList);
	SlimList_Iterator_Advance(&iterator);

	CHECK_FALSE(SlimList_Iterator_HasItem(iterator));
}


TEST(SlimList, iteratorGetString)
{
	const char* contents = "a";
	SlimList_AddString(slimList, contents);
	SlimListIterator* iterator = SlimList_CreateIterator(slimList);

	char *actual =  SlimList_Iterator_GetString(iterator);
	STRCMP_EQUAL(contents, actual);
}



