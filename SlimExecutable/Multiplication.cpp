#include "Multiplication.h"
#include "Fixtures.h"

#include "SlimList.h"
#include "StatementExecutor.h"

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

SLIM_BEGIN_FIXTURE(Multiplication) 
  SLIM_FUNCTION(setMultiplicand1)
  SLIM_FUNCTION(setMultiplicand2)
  SLIM_FUNCTION(Product)
SLIM_END_FIXTURE


Slim::FixtureIntf* Multiplication::Create(Slim::StatementExecutor* executor, Slim::SlimList* args)
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

std::string Multiplication::setMultiplicand1(Slim::SlimList* args)
{
  m_m1 = boost::lexical_cast<float>(args->GetStringAt(0));
  return std::string();
}

std::string Multiplication::setMultiplicand2(Slim::SlimList* args)
{
  m_m2 = boost::lexical_cast<float>(args->GetStringAt(0));
  return std::string();
}

std::string Multiplication::Product(Slim::SlimList* args)
{
  return (boost::format("%1%") % (m_m1 * m_m2)).str();
}
