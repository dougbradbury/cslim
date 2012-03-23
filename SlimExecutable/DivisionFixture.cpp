#include "Division.h"
#include "Fixtures.h"

#include "SlimList.h"

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

using namespace std;


// Initialize method look up table for DivisionFixture.
DivisionFixture::MethodLookupTable DivisionFixture::tSuper::m_lookupTable[] = 
{
  { "setNumerator", &DivisionFixture::setNumerator },
  { "setDenominator", &DivisionFixture::setDenominator },
  { "Quotient", &DivisionFixture::Quotient },
  { "execute", &DivisionFixture::execute },
  { "reset", &DivisionFixture::reset },
  { "table", &DivisionFixture::table },
};
size_t DivisionFixture::tSuper::m_methodCount = _countof(DivisionFixture::tSuper::m_lookupTable);
std::string DivisionFixture::tSuper::m_fixtureName = "Division";

SLIM_CREATE_CPPFIXTURE(DivisionFixture) 


FixtureIntf* DivisionFixture::Create(StatementExecutor* executor, SlimList* args)
{
  return new DivisionFixture();
}

DivisionFixture::DivisionFixture()
{

}

DivisionFixture::~DivisionFixture()
{

}

void DivisionFixture::Destroy()
{
  delete this;
}

string DivisionFixture::setNumerator(SlimList* args)
{
  m_numerator = boost::lexical_cast<float>(SlimList_GetStringAt(args, 0));
  return string();
}

string DivisionFixture::setDenominator(SlimList* args) 
{
  m_denominator = boost::lexical_cast<float>(SlimList_GetStringAt(args, 0));
  return m_denominator == 0.0 ? SLIM_EXCEPTION("You shouldn't divide by zero now should ya?") : string();
}

string DivisionFixture::Quotient(SlimList* args) 
{
  float quotient = m_numerator / m_denominator;
  //snprintf(self->result, 32, "%g", quotient);
  //return self->result;
  return (boost::format("%1%") % quotient).str();
}

//These are optional.  If they aren't declared, they are ignored
string DivisionFixture::execute(SlimList* args) 
{
  return string();
}

string DivisionFixture::reset(SlimList* args) 
{
  m_denominator = 0.0f;
  m_numerator = 0.0f;
  return string();
}

string DivisionFixture::table(SlimList* args) 
{
  return string();
}
