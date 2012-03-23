#include "Count.h"
#include "Fixtures.h"

#include "SlimList.h"
#include "StatementExecutor.h"

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

using namespace std;

SLIM_BEGIN_FIXTURE(Count) 
  SLIM_FUNCTION(count)
  SLIM_FUNCTION(counter)
SLIM_END_FIXTURE


FixtureIntf* Count::Create(Slim::StatementExecutor* executor, Slim::SlimList* args)
{
  return new Count();
}

Count::Count()
  : m_count(0)
{

}

Count::~Count()
{

}

void Count::Destroy()
{
  delete this;
}

string Count::count(Slim::SlimList* args) 
{
  ++m_count;
  return string();
}

string Count::counter(Slim::SlimList* args) 
{
  return (boost::format("%1%") % m_count).str();
}
