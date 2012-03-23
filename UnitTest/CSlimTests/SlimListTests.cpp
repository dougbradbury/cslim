#include "stdafx.h"
#include <SlimList.h>

using namespace Slim;

BOOST_TEST_DONT_PRINT_LOG_VALUE(SlimList);

struct InitSlimListTest
{
  InitSlimListTest()
  {
  }

  ~InitSlimListTest()
  {
  }

  SlimList slimList;
};

BOOST_FIXTURE_TEST_SUITE(SlimListTestSuite, InitSlimListTest)

BOOST_AUTO_TEST_CASE(TwoEmptyListsAreEqual)
{
  SlimList* list = new SlimList();
  BOOST_CHECK_EQUAL(slimList, *list);
  delete list;
}

BOOST_AUTO_TEST_CASE(TwoDifferentLenghtListsAreNotEqual)
{
  SlimList* list = new SlimList();
  slimList.AddString("hello");
  BOOST_CHECK_NE(slimList, *list);
  delete list;
}

BOOST_AUTO_TEST_CASE(TwoSingleElementListsWithDifferentElmementsAreNotEqual)
{
  SlimList* list = new SlimList();
  slimList.AddString("hello");
  list->AddString("goodbye");
  BOOST_CHECK_NE(slimList, *list);
  delete list;
}

BOOST_AUTO_TEST_CASE(TwoIdenticalMultipleElementListsElmementsAreEqual)
{
  SlimList* list = new SlimList();
  slimList.AddString("hello");
  slimList.AddString("goodbye");
  list->AddString("hello");
  list->AddString("goodbye");
  BOOST_CHECK_EQUAL(slimList, *list);
  delete list;
}

BOOST_AUTO_TEST_CASE(TwoNonIdenticalMultipleElementListsElmementsAreNotEqual)
{
  SlimList* list = new SlimList();
  slimList.AddString("hello");
  slimList.AddString("hello");
  list->AddString("goodbye");
  list->AddString("goodbye");
  BOOST_CHECK_NE(slimList, *list);
  delete list;
}

BOOST_AUTO_TEST_CASE(CanGetElements)
{
  slimList.AddString("element1");
  slimList.AddString("element2");
  BOOST_CHECK_EQUAL("element1", slimList.GetStringAt(0));
  BOOST_CHECK_EQUAL("element2", slimList.GetStringAt(1));
}

BOOST_AUTO_TEST_CASE(CanGetHashWithOneElement)
{
  slimList.AddString("<table><tr><td>name</td><td>bob</td></tr></table>");

  SlimList* hash = slimList.GetHashAt(0);
  SlimList* twoElementList = hash->GetListAt(0);

  BOOST_CHECK_EQUAL("name", twoElementList->GetStringAt(0));
  BOOST_CHECK_EQUAL("bob", twoElementList->GetStringAt(1));

  delete hash;
}

BOOST_AUTO_TEST_CASE(CanGetHashWithMultipleElements)
{
  slimList.AddString("<table><tr><td>name</td><td>dough</td></tr><tr><td>addr</td><td>here</td></tr></table>");

  SlimList* hash = slimList.GetHashAt(0);
  SlimList* twoElementList = hash->GetListAt(1);

  BOOST_CHECK_EQUAL("addr", twoElementList->GetStringAt(0));
  BOOST_CHECK_EQUAL("here", twoElementList->GetStringAt(1));

  delete hash;
}

BOOST_AUTO_TEST_CASE(CannotGetElementThatAreNotThere)
{
  slimList.AddString("element1");
  slimList.AddString("element2");
  BOOST_CHECK(slimList.GetStringAt(3).empty());
}

BOOST_AUTO_TEST_CASE(CanReplaceString)
{
  slimList.AddString("replaceMe");
  slimList.ReplaceAt(0, "WithMe");
  BOOST_CHECK_EQUAL("WithMe", slimList.GetStringAt(0));
}

BOOST_AUTO_TEST_CASE(CanGetTail)
{
  slimList.AddString("1");
  slimList.AddString("2");
  slimList.AddString("3");
  slimList.AddString("4");

  SlimList* expected = new SlimList();
  expected->AddString("3");
  expected->AddString("4");

  SlimList* tail = slimList.GetTailAt(2);
  BOOST_CHECK_EQUAL(*expected, *tail);

  delete tail;
  delete expected;
}

BOOST_AUTO_TEST_CASE(RecursiveToString)
{
  slimList.AddString("a");
  slimList.AddString("b");

  SlimList* sublist = new SlimList();
  sublist->AddString("3");
  sublist->AddString("4");

  slimList.AddList(sublist);

  BOOST_CHECK_EQUAL("[\"a\", \"b\", [\"3\", \"4\"]]", slimList.ToString());

  delete sublist;
}

BOOST_AUTO_TEST_SUITE_END()