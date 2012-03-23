#include "Exceptions.h"
#include "Fixtures.h"

#include "SlimList.h"
#include "StatementExecutor.h"

using namespace std;

SLIM_BEGIN_FIXTURE(ExceptionsExample) 
  SLIM_FUNCTION(setTrouble)
SLIM_END_FIXTURE


FixtureIntf* ExceptionsExample::Create(Slim::StatementExecutor* executor, Slim::SlimList* args)
{
  return new ExceptionsExample();
}

ExceptionsExample::~ExceptionsExample()
{

}

void ExceptionsExample::Destroy()
{
  delete this;
}

string ExceptionsExample::setTrouble(Slim::SlimList* args)
{
  return SLIM_EXCEPTION("You stink");
}
