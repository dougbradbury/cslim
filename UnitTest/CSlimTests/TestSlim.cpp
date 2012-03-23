#include "stdafx.h"

#include "TestSlim.h"
#include "../../SlimExecutable/Fixtures.h"

#include <SlimList.h>
#include <StatementExecutor.h>

using namespace std;
using namespace Slim;

SLIM_BEGIN_FIXTURE(TestSlim) 
  SLIM_FUNCTION(returnValue)
  SLIM_FUNCTION(noArgs)
  SLIM_FUNCTION(echo)
  SLIM_FUNCTION(add)
  SLIM_FUNCTION(null)
  SLIM_FUNCTION(setArg)
  SLIM_FUNCTION(getArg)
  SLIM_FUNCTION(getConstructionArg)
  SLIM_FUNCTION(returnError)
SLIM_END_FIXTURE


FixtureIntf* TestSlim::Create(Slim::StatementExecutor* executor, Slim::SlimList* args)
{
  if (args->GetLength() > 1) 
  {
    executor->ConstructorError("xxx");
    return 0;
  }

  return new TestSlim(executor, args);
}

TestSlim::TestSlim(Slim::StatementExecutor* executor, SlimList* args)
  : statementExecutor(executor)
  , constructionArg(args->GetStringAt(0))
{
}

void TestSlim::Destroy()
{
  delete this;
}


std::string TestSlim::noArgs(SlimList* args) 
{
  noArgsCalled = 1;
  return "/__VOID__/";
}

std::string TestSlim::returnValue(SlimList* args) 
{
  return "value";
}

std::string TestSlim::echo(SlimList* args) 
{
  return args->GetStringAt(0);
}

std::string TestSlim::add(SlimList* args) 
{
  return (boost::format("%1%%2%") % args->GetStringAt(0) % args->GetStringAt(1)).str();
}

std::string TestSlim::null(SlimList* args)
{
  return string();
}

std::string TestSlim::setArg(SlimList* args) 
{
  arg = args->GetStringAt(0);
  return "/__VOID__/";
}

std::string TestSlim::getArg(SlimList* args) 
{
  return arg;
}

std::string TestSlim::getConstructionArg(SlimList* args) 
{
  return constructionArg;
}

std::string TestSlim::returnError(SlimList* args) 
{
  return statementExecutor->FixtureError("my exception");
}
