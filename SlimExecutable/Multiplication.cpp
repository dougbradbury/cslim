#include "Multiplication.h"
#include "Fixtures.h"

#include "SlimList.h"
#include "StatementExecutor.h"

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

using namespace std;

SLIM_BEGIN_FIXTURE(Multiplication) 
  SLIM_FUNCTION(setMultiplicand1)
  SLIM_FUNCTION(setMultiplicand2)
  SLIM_FUNCTION(Product)
SLIM_END_FIXTURE


FixtureIntf* Multiplication::Create(Slim::StatementExecutor* executor, Slim::SlimList* args)
{
  return new Multiplication();
}

Multiplication::~Multiplication()
{

}

void Multiplication::Destroy()
{
  delete this;
}

string Multiplication::setMultiplicand1(Slim::SlimList* args)
{
  m_m1 = boost::lexical_cast<float>(args->GetStringAt(0));
  return string();
}

string Multiplication::setMultiplicand2(Slim::SlimList* args)
{
  m_m2 = boost::lexical_cast<float>(args->GetStringAt(0));
  return string();
}

string Multiplication::Product(Slim::SlimList* args)
{
  return (boost::format("%1%") % (m_m1 * m_m2)).str();
}
