#include <stdlib.h>
#include <string.h>
#include <iostream>

extern "C"
{
#include "SlimList.h"
#include "SlimListDeserializer.h"
#include "SlimListSerializer.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP(SlimListDeserializer)
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
			SlimList_Release(serializedList);
    }

	void check_lists_equal(SlimList* expected, SlimList* actual) {
		CHECK(SlimList_Equals(expected, actual));
	}

};


TEST(SlimListDeserializer, deserializeEmptyList)
{
	deserializedList = SlimList_Deserialize("[000000:]");
	CHECK(deserializedList != 0);
	LONGS_EQUAL(0, SlimList_GetLength(deserializedList));
}

TEST(SlimListDeserializer, deserializeNull)
{
	SlimList* list = SlimList_Deserialize(0);
	POINTERS_EQUAL(0, list);
}

TEST(SlimListDeserializer, deserializeEmptyString)
{
	SlimList* list = SlimList_Deserialize("");
	POINTERS_EQUAL(0, list);
}

TEST(SlimListDeserializer, MissingOpenBracketReturnsNull)
{
	SlimList* list = SlimList_Deserialize("hello");
	POINTERS_EQUAL(0, list);
}

TEST(SlimListDeserializer, MissingClosingBracketReturnsNull)
{
	SlimList* list = SlimList_Deserialize("[000000:");
	POINTERS_EQUAL(0, list);
}

TEST(SlimListDeserializer, canDeserializeCanonicalListWithOneElement)
{
	char const* canonicalList = "[000001:000008:Hi doug.:]";
	SlimList* deserializedList = SlimList_Deserialize(canonicalList);
	CHECK(deserializedList != NULL);
	LONGS_EQUAL(1, SlimList_GetLength(deserializedList));
	STRCMP_EQUAL("Hi doug.", SlimList_GetStringAt(deserializedList, 0));
	SlimList_Destroy(deserializedList);
}

TEST(SlimListDeserializer, canDeserializeWithMultibyteCharacters)
{
  char const* canonicalList = "[000001:000008:Hi JRÜ€©:]";
  SlimList* deserializedList = SlimList_Deserialize(canonicalList);
  CHECK(deserializedList != NULL);
  LONGS_EQUAL(1, SlimList_GetLength(deserializedList));
  STRCMP_EQUAL("Hi JRÜ€©", SlimList_GetStringAt(deserializedList, 0));
  SlimList_Destroy(deserializedList);
}

TEST(SlimListDeserializer, canDeSerializeListWithOneElement)
{
	SlimList_AddString(slimList, "hello");
	serializedList = SlimList_Serialize(slimList);
	deserializedList = SlimList_Deserialize(serializedList);
	CHECK(deserializedList != 0);
	check_lists_equal(slimList, deserializedList);
}

TEST(SlimListDeserializer, canDeSerializeListWithTwoElements)
{
	SlimList_AddString(slimList, "hello");
	SlimList_AddString(slimList, "bob");
	serializedList = SlimList_Serialize(slimList);
	deserializedList = SlimList_Deserialize(serializedList);
	CHECK(deserializedList != 0);
	check_lists_equal(slimList, deserializedList);
}

TEST(SlimListDeserializer, canAddSubList)
{
	SlimList* embeddedList;
	embeddedList = SlimList_Create();
	SlimList_AddString(embeddedList, "element");
	SlimList_AddList(slimList, embeddedList);
	serializedList = SlimList_Serialize(slimList);
	deserializedList = SlimList_Deserialize(serializedList);
	SlimList * subList = SlimList_GetListAt(deserializedList, 0);
	subList = SlimList_GetListAt(deserializedList, 0);
	check_lists_equal(embeddedList, subList);

	SlimList_Destroy(embeddedList);
}

TEST(SlimListDeserializer, getStringWhereThereIsAList)
{
	SlimList* embeddedList;
	embeddedList = SlimList_Create();
	SlimList_AddString(embeddedList, "element");
	SlimList_AddList(slimList, embeddedList);
	serializedList = SlimList_Serialize(slimList);
	deserializedList = SlimList_Deserialize(serializedList);
	char * string = SlimList_GetStringAt(deserializedList, 0);

	STRCMP_EQUAL("[000001:000007:element:]", string);
	// POINTERS_EQUAL(0, string); ?????????????????????????????????????

	SlimList_Destroy(embeddedList);
}
