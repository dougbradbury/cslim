#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "SlimUtil.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP(SlimUtil)
{

};

TEST(SlimUtil, CanCreateEmptyString)
{
  const char* actual = CSlim_CreateEmptyString();
  STRCMP_EQUAL("", actual);
  CSlim_DestroyString(actual);
}

TEST(SlimUtil, CanConcatenateToAnEmptyString)
{
  const char* actual = CSlim_CreateEmptyString();
  CSlim_ConcatenateString(&actual, "a");
  STRCMP_EQUAL("a", actual);

  CSlim_DestroyString(actual);
}

TEST(SlimUtil, CanConcatenateToANonEmptyString)
{
  const char* actual = CSlim_CreateEmptyString();
  CSlim_ConcatenateString(&actual, "a");
  CSlim_ConcatenateString(&actual, "b");
  STRCMP_EQUAL("ab", actual);

  CSlim_DestroyString(actual);
}

TEST(SlimUtil, StringStartsWith)
{
    CHECK(CSlim_StringStartsWith("", ""));
    CHECK_FALSE(CSlim_StringStartsWith("", "a"));
    CHECK_FALSE(CSlim_StringStartsWith("a", "ab"));
    CHECK(CSlim_StringStartsWith("a", ""));
    CHECK(CSlim_StringStartsWith("a", "a"));
    CHECK(CSlim_StringStartsWith("ab", "a"));
    CHECK_FALSE(CSlim_StringStartsWith("a", "b"));
    CHECK(CSlim_StringStartsWith("abc", "ab"));
    CHECK_FALSE(CSlim_StringStartsWith("abc", "ac"));
}
