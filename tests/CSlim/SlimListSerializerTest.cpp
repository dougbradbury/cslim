#include <stdlib.h>
#include <string.h>
#include <iostream>


extern "C"
{
#include "SlimList.h"
#include "SlimListSerializer.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP(SlimListSerializer)
{
  SlimList* slimList;
  char* serializedList;

  void setup()
  {
    slimList  = SlimList_Create();
    serializedList = 0;
  }

  void teardown()
  {
    SlimList_Destroy(slimList);

    SlimList_Release(serializedList);
  }
};

TEST(SlimListSerializer, SerializeAListWithNoElements)
{
	serializedList = SlimList_Serialize(slimList);
	STRCMP_EQUAL("[000000:]", serializedList);
}

TEST(SlimListSerializer, SerializeAListWithOneElements)
{
	SlimList_AddString(slimList, "hello");
	serializedList = SlimList_Serialize(slimList);
	STRCMP_EQUAL("[000001:000005:hello:]", serializedList);
}

TEST(SlimListSerializer, SerializeAListWithTwoElements)
{
	SlimList_AddString(slimList, "hello");
	SlimList_AddString(slimList, "world");

	serializedList = SlimList_Serialize(slimList);

	STRCMP_EQUAL("[000002:000005:hello:000005:world:]", serializedList);
}

TEST(SlimListSerializer, ListCopysItsString)
{
	char string[12] = "Hello";
	SlimList_AddString(slimList, string);
	strcpy(string, "Goodbye");
	serializedList = SlimList_Serialize(slimList);
	STRCMP_EQUAL("[000001:000005:Hello:]", serializedList);
}

TEST(SlimListSerializer, canCopyAList)
{
	SlimList_AddString(slimList, "123456");
	SlimList_AddString(slimList, "987654");

	SlimList* copy = SlimList_Create();
	int i;
	for (i=0; i<SlimList_GetLength(slimList); i++) {
		char* string = SlimList_GetStringAt(slimList, i);
		SlimList_AddString(copy, string);
	}
	char * serialCopy = SlimList_Serialize(copy);
	char * serialSlimList = SlimList_Serialize(slimList);
	STRCMP_EQUAL(serialCopy, serialSlimList);

	SlimList_Destroy(copy);
	cpputest_free(serialSlimList);
	cpputest_free(serialCopy);

}

TEST(SlimListSerializer, SerializeNestedList)
{
	SlimList* embeddedList;
	embeddedList = SlimList_Create();
	SlimList_AddString(embeddedList, "element");
	SlimList_AddList(slimList, embeddedList);
	serializedList = SlimList_Serialize(slimList);
	STRCMP_EQUAL("[000001:000024:[000001:000007:element:]:]", serializedList);
	SlimList_Destroy(embeddedList);
}

TEST(SlimListSerializer, serializedLength)
{
	SlimList_AddString(slimList, "12345");
	LONGS_EQUAL(5 + 17, SlimList_SerializedLength(slimList));
	SlimList_AddString(slimList, "123456");
	LONGS_EQUAL(9 + (5+8) + (6+8), SlimList_SerializedLength(slimList));
	serializedList = SlimList_Serialize(slimList);
	LONGS_EQUAL(9 + (5+8) + (6+8), strlen(serializedList));
}

TEST(SlimListSerializer, serializeNull)
{
  SlimList_AddString(slimList, NULL);
	serializedList = SlimList_Serialize(slimList);
	STRCMP_EQUAL("[000001:000004:null:]", serializedList);

}

TEST(SlimListSerializer, serializeMultibyteCharacters)
{
  SlimList_AddString(slimList, "Ü€©phewÜ€©");
  serializedList = SlimList_Serialize(slimList);
  STRCMP_EQUAL("[000001:000010:Ü€©phewÜ€©:]", serializedList);
}

