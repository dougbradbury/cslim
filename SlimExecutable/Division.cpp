#include "Division.h"
#include "Fixtures.h"

#include "SlimList.h"
#include "StatementExecutor.h"

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

SLIM_BEGIN_FIXTURE(Division) 
  SLIM_FUNCTION(setNumerator)
  SLIM_FUNCTION(setDenominator)
  SLIM_FUNCTION(Quotient)
  SLIM_FUNCTION(execute)
  SLIM_FUNCTION(reset)
  SLIM_FUNCTION(table)
SLIM_END_FIXTURE


Slim::FixtureIntf* Division::Create(Slim::StatementExecutor* executor, Slim::SlimList* args)
{
  return new Division();
}

Division::~Division()
{

}

void Division::Destroy()
{
  delete this;
}

std::string Division::setNumerator(Slim::SlimList* args)
{
  m_numerator = boost::lexical_cast<float>(args->GetStringAt(0));
  return std::string();
}

std::string Division::setDenominator(Slim::SlimList* args) 
{
  m_denominator = boost::lexical_cast<float>(args->GetStringAt(0));
  return m_denominator == 0.0 ? SLIM_EXCEPTION("You shouldn't divide by zero now should ya?") : std::string();
}

std::string Division::Quotient(Slim::SlimList* args) 
{
  float quotient = m_numerator / m_denominator;
  return (boost::format("%1%") % quotient).str();
}

//These are optional.  If they aren't declared, they are ignored
std::string Division::execute(Slim::SlimList* args) 
{
  return std::string();
}

std::string Division::reset(Slim::SlimList* args) 
{
  m_denominator = 0.0f;
  m_numerator = 0.0f;
  return std::string();
}

std::string Division::table(Slim::SlimList* args) 
{
  return std::string();
}
