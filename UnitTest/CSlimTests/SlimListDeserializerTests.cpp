#include "stdafx.h"
#include <SlimList.h>

using namespace Slim;

BOOST_TEST_DONT_PRINT_LOG_VALUE(SlimList);

struct InitSlimListDeserializerTest
{
  InitSlimListDeserializerTest()
    : deserializedList(0)
  {
  }

  ~InitSlimListDeserializerTest()
  {
    delete deserializedList;
  }

  SlimList slimList;
  SlimList* deserializedList;
  std::string serializedList;
};

BOOST_FIXTURE_TEST_SUITE(SlimListDeserializerTestSuite, InitSlimListDeserializerTest)


BOOST_AUTO_TEST_CASE(DeserializeEmptyList)
{
  deserializedList = SlimList::Deserialize("[000000:]");
  BOOST_CHECK(deserializedList);
  BOOST_CHECK_EQUAL(0, deserializedList->GetLength());
}

BOOST_AUTO_TEST_CASE(DeserializeEmptyString)
{
  SlimList* list = SlimList::Deserialize("");
  BOOST_CHECK(!list);
}

BOOST_AUTO_TEST_CASE(MissingOpenBracketReturnsNull)
{
  SlimList* list = SlimList::Deserialize("hello");
  BOOST_CHECK(!list);
}

BOOST_AUTO_TEST_CASE(MissingClosingBracketReturnsNull)
{
  SlimList* list = SlimList::Deserialize("[000000:");
  BOOST_CHECK(!list);
}

BOOST_AUTO_TEST_CASE(CanDeserializeCanonicalListWithOneElement)
{
  char const* canonicalList = "[000001:000008:Hi doug.:]";
  deserializedList = SlimList::Deserialize(canonicalList);
  BOOST_CHECK(deserializedList);
  BOOST_CHECK_EQUAL(1, deserializedList->GetLength());
  BOOST_CHECK_EQUAL("Hi doug.", deserializedList->GetStringAt(0));
}

BOOST_AUTO_TEST_CASE(CanDeSerializeListWithOneElement)
{
  slimList.AddString("hello");
  serializedList = SlimList::Serialize(&slimList);
  deserializedList = SlimList::Deserialize(serializedList);
  BOOST_CHECK(deserializedList);
  BOOST_CHECK_EQUAL(slimList, *deserializedList);
}

BOOST_AUTO_TEST_CASE(CanDeSerializeListWithTwoElements)
{
  slimList.AddString("hello");
  slimList.AddString("bob");
  serializedList = SlimList::Serialize(&slimList);
  deserializedList = SlimList::Deserialize(serializedList);
  BOOST_CHECK(deserializedList);
  BOOST_CHECK_EQUAL(slimList, *deserializedList);
}

BOOST_AUTO_TEST_CASE(CanAddSubList)
{
  SlimList* embeddedList = new SlimList();
  embeddedList->AddString("element");
  slimList.AddList(embeddedList);
  serializedList = SlimList::Serialize(&slimList);
  deserializedList = SlimList::Deserialize(serializedList);
  SlimList* subList = deserializedList->GetListAt(0);
  subList = deserializedList->GetListAt(0);
  BOOST_CHECK_EQUAL(*embeddedList, *subList);

  delete embeddedList;
}

BOOST_AUTO_TEST_CASE(GetStringWhereThereIsAList)
{
  SlimList* embeddedList = new SlimList();
  embeddedList->AddString("element");
  slimList.AddList(embeddedList);
  serializedList = SlimList::Serialize(&slimList);
  deserializedList = SlimList::Deserialize(serializedList);
  std::string string = deserializedList->GetStringAt(0);
  BOOST_CHECK_EQUAL("[000001:000007:element:]", string);

  delete embeddedList;
}

BOOST_AUTO_TEST_SUITE_END()