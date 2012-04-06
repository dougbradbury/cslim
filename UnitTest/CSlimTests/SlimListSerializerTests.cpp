#include "stdafx.h"
#include <SlimList.h>

using namespace Slim;

BOOST_TEST_DONT_PRINT_LOG_VALUE(SlimList);

struct InitSlimListSerializerTest
{
  InitSlimListSerializerTest()
  {
  }

  ~InitSlimListSerializerTest()
  {
  }

  SlimList slimList;
  std::string serializedList;
};

BOOST_FIXTURE_TEST_SUITE(SlimListSerializerTestSuite, InitSlimListSerializerTest)

BOOST_AUTO_TEST_CASE(SerializeAListWithNoElements)
{
  serializedList = SlimList::Serialize(&slimList);
  BOOST_CHECK_EQUAL("[000000:]", serializedList);
}

BOOST_AUTO_TEST_CASE(SerializeAListWithOneElements)
{
  slimList.AddString("hello");
  serializedList = SlimList::Serialize(&slimList);
  BOOST_CHECK_EQUAL("[000001:000005:hello:]", serializedList);
}

BOOST_AUTO_TEST_CASE(SerializeAListWithTwoElements)
{
  slimList.AddString("hello");
  slimList.AddString("world");

  serializedList = SlimList::Serialize(&slimList);
  BOOST_CHECK_EQUAL("[000002:000005:hello:000005:world:]", serializedList);
}

BOOST_AUTO_TEST_CASE(ListCopysItsString)
{
  char string[12] = "Hello";
  slimList.AddString(string);
  strcpy_s(string, "Goodbye");
  serializedList = SlimList::Serialize(&slimList);
  BOOST_CHECK_EQUAL("[000001:000005:Hello:]", serializedList);
}

BOOST_AUTO_TEST_CASE(CanCopyAList)
{
  slimList.AddString("123456");
  slimList.AddString("987654");

  SlimList* copy = new SlimList();
  for (int i = 0; i < slimList.GetLength(); ++i) 
  {
    std::string string = slimList.GetStringAt(i);
    copy->AddString(string);
  }
  std::string serialCopy = SlimList::Serialize(copy);
  std::string serialSlimList = SlimList::Serialize(&slimList);
  BOOST_CHECK_EQUAL(serialCopy, serialSlimList);

  delete copy;
}

BOOST_AUTO_TEST_CASE(SerializeNestedList)
{
  SlimList* embeddedList = new SlimList();
  embeddedList->AddString("element");
  slimList.AddList(embeddedList);
  serializedList = SlimList::Serialize(&slimList);
  BOOST_CHECK_EQUAL("[000001:000024:[000001:000007:element:]:]", serializedList);
  delete embeddedList;
}

BOOST_AUTO_TEST_CASE(SerializedLength)
{
  slimList.AddString("12345");
  BOOST_CHECK_EQUAL(5 + 17, SlimList::SerializedLength(&slimList));
  slimList.AddString("123456");
  BOOST_CHECK_EQUAL(9 + (5+8) + (6+8), SlimList::SerializedLength(&slimList));
  serializedList = SlimList::Serialize(&slimList);
  BOOST_CHECK_EQUAL(9 + (5+8) + (6+8), serializedList.size());
}

BOOST_AUTO_TEST_CASE(SerializeNull)
{
  slimList.AddString("");
  serializedList = SlimList::Serialize(&slimList);
  BOOST_CHECK_EQUAL("[000001:000004:null:]", serializedList);

}

BOOST_AUTO_TEST_SUITE_END()