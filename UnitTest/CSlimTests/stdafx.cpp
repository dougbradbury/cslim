//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CSlim Unit Test

#include "stdafx.h"

//
struct GlobalInit
{
  GlobalInit()
  {
  }

  ~GlobalInit()
  {
  }
};

BOOST_GLOBAL_FIXTURE(GlobalInit);