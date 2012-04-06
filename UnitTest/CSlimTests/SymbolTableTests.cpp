#include "stdafx.h"
#include <SymbolTable.h>

struct InitSymbolTableTest
{
  InitSymbolTableTest()
  {
  }

  ~InitSymbolTableTest()
  {
  }

  Slim::SymbolTable symbolTable;
};

BOOST_FIXTURE_TEST_SUITE(SymbolTableTestSuite, InitSymbolTableTest)

BOOST_AUTO_TEST_CASE(FindNonExistentSymbolShouldReturnFalse)
{
  BOOST_CHECK(!symbolTable.Find("Hey"));
}

BOOST_AUTO_TEST_CASE(GetNonExistentSymbolShouldThrow)
{
  BOOST_CHECK_THROW(symbolTable.GetValue("Hey"), std::exception);
}

BOOST_AUTO_TEST_CASE(GetExistingSymbolShouldNotThrow)
{
  symbolTable.SetValue("Hey", "You");
  BOOST_CHECK(symbolTable.Find("Hey"));
  BOOST_CHECK_NO_THROW(symbolTable.GetValue("Hey"));
  BOOST_CHECK_EQUAL("You", symbolTable.GetValue("Hey"));
}

BOOST_AUTO_TEST_CASE(CanReplaceSymbolsWithTheirValue)
{
	symbolTable.SetValue("v", "bob");
  std::string input("hi $v.");
  std::string result = symbolTable.ReplaceStringFrom(input, input);
  BOOST_CHECK_EQUAL(result, "hi bob.");
}

BOOST_AUTO_TEST_CASE(CanReplaceSymbolsInTheMiddle)
{
  symbolTable.SetValue("v", "bob");
  std::string input("hi $v whats up.");
  std::string result = symbolTable.ReplaceStringFrom(input, input);
  BOOST_CHECK_EQUAL(result, "hi bob whats up.");
}

BOOST_AUTO_TEST_CASE(CanReplaceSymbolsWithOtherNonAlphaNumeric)
{
  symbolTable.SetValue("v2", "bob");
  std::string input("$v2=why");
  std::string result = symbolTable.ReplaceStringFrom(input, input);
  BOOST_CHECK_EQUAL(result, "bob=why");
}

BOOST_AUTO_TEST_CASE(CanReplaceMultipleSymbolsWithTheirValue)
{
  symbolTable.SetValue("v", "bob");
  symbolTable.SetValue("e", "doug");
  std::string input("hi $v. Cost:  $12.32 from $e.");
  std::string result = symbolTable.ReplaceStringFrom(input, input);
  BOOST_CHECK_EQUAL(result, "hi bob. Cost:  $12.32 from doug.");
}

BOOST_AUTO_TEST_CASE(CanHandleStringWithJustADollarSign)
{
  symbolTable.SetValue("v2", "bob");
  std::string input("$");
  std::string result = symbolTable.ReplaceStringFrom(input, input);
  BOOST_CHECK_EQUAL(result, "$");
}

BOOST_AUTO_TEST_CASE(CanHandleDollarSignAtTheEndOfTheString)
{
  symbolTable.SetValue("v2", "doug");
  std::string input("hi $v2$");
  std::string result = symbolTable.ReplaceStringFrom(input, input);
  BOOST_CHECK_EQUAL(result, "hi doug$");
}

BOOST_AUTO_TEST_SUITE_END()