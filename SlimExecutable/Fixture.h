#pragma once

#include <SlimList.h>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>

#include <algorithm>
#include <xstring>

//
namespace Slim
{
  class StatementExecutor;
  class SlimList;
}

// Fixture interface.
class FixtureIntf
{
public:
  virtual ~FixtureIntf() {}

  virtual void Destroy() = 0;
  virtual std::string Execute(std::string const& method, Slim::SlimList* args) = 0;
};

// Fixture base class serves as factory class.
template<typename ConcreteFixture>
class FixtureBase : public FixtureIntf
{
public:
  static FixtureIntf* CreateInstance(Slim::StatementExecutor* executor, Slim::SlimList* args);

  virtual std::string Execute(std::string const& method, Slim::SlimList* args);

  struct FixtureMethod
  {
    char const* methodName;
    std::string (ConcreteFixture::*funcPtr)(Slim::SlimList* args);
  };
  static FixtureMethod m_lookupTable[];
  static std::string m_fixtureName;
  static size_t m_methodCount;

private:
  friend ConcreteFixture;
  static bool IsMatchedMethod(FixtureMethod const& fixtureMethod, std::string const& method);
};

template<typename ConcreteFixture>
FixtureIntf* FixtureBase<ConcreteFixture>::CreateInstance(Slim::StatementExecutor* executor, Slim::SlimList* args)
{
  return ConcreteFixture::Create(executor, args);
}

template<typename ConcreteFixture>
bool FixtureBase<ConcreteFixture>::IsMatchedMethod(FixtureMethod const& fixtureMethod, std::string const& method)
{
  return boost::algorithm::iequals(method.substr(0, 32), std::string(fixtureMethod.methodName));
}

template<typename ConcreteFixture>
std::string FixtureBase<ConcreteFixture>::Execute(std::string const& method, Slim::SlimList* args)
{
  // SLIM protocol only allows method name up to 32 characters.
  std::string legitName = method.substr(0, 32);
  auto it = std::find_if(m_lookupTable, m_lookupTable + m_methodCount, boost::bind(&FixtureBase<ConcreteFixture>::IsMatchedMethod, _1, legitName));
  if (it == m_lookupTable + m_methodCount)
  {
    throw std::exception((boost::format("%1%[%2%] %3%") % legitName % args->GetLength() % m_fixtureName).str().c_str());
  }

  return (static_cast<ConcreteFixture*>(this)->*(it->funcPtr))(args);
}

