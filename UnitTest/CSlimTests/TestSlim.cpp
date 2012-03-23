#include "stdafx.h"

#include "TestSlim.h"
#include "Fixtures.h"

#include <SlimList.h>
#include <StatementExecutor.h>

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


Slim::FixtureIntf* TestSlim::Create(Slim::StatementExecutor* executor, Slim::SlimList* args)
{
  if (args->GetLength() > 1) 
  {
    executor->ConstructorError("xxx");
    return 0;
  }

  return new TestSlim(executor, args);
}

TestSlim::TestSlim(Slim::StatementExecutor* executor, Slim::SlimList* args)
  : statementExecutor(executor)
  , constructionArg(args->GetStringAt(0))
{
}

void TestSlim::Destroy()
{
  delete this;
}


std::string TestSlim::noArgs(Slim::SlimList* args) 
{
  noArgsCalled = 1;
  return "/__VOID__/";
}

std::string TestSlim::returnValue(Slim::SlimList* args) 
{
  return "value";
}

std::string TestSlim::echo(Slim::SlimList* args) 
{
  return args->GetStringAt(0);
}

std::string TestSlim::add(Slim::SlimList* args) 
{
  return (boost::format("%1%%2%") % args->GetStringAt(0) % args->GetStringAt(1)).str();
}

std::string TestSlim::null(Slim::SlimList* args)
{
  return std::string();
}

std::string TestSlim::setArg(Slim::SlimList* args) 
{
  arg = args->GetStringAt(0);
  return "/__VOID__/";
}

std::string TestSlim::getArg(Slim::SlimList* args) 
{
  return arg;
}

std::string TestSlim::getConstructionArg(Slim::SlimList* args) 
{
  return constructionArg;
}

std::string TestSlim::returnError(Slim::SlimList* args) 
{
  return statementExecutor->FixtureError("my exception");
}
