#include <stdlib.h>
#include <string.h>
#include <iostream>

extern "C"
{
#include "SlimUtil.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP(SlimUtil)
{

};

TEST(SlimUtil, CanCreateEmptyString)
{
  char* actual = CSlim_CreateEmptyString();
  STRCMP_EQUAL("", actual);
  CSlim_DestroyString(actual);
}

TEST(SlimUtil, CanConcatenateToAnEmptyString)
{
  char* actual = CSlim_CreateEmptyString();
  CSlim_ConcatenateString(&actual, "a");
  STRCMP_EQUAL("a", actual);

  CSlim_DestroyString(actual);
}

TEST(SlimUtil, CanConcatenateToANonEmptyString)
{
  char* actual = CSlim_CreateEmptyString();
  CSlim_ConcatenateString(&actual, "a");
  CSlim_ConcatenateString(&actual, "b");
  STRCMP_EQUAL("ab", actual);

  CSlim_DestroyString(actual);
}
