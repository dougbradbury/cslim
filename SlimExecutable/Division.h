#pragma once

#include "Fixture.h"

class Division : public FixtureBase<Division>
{
public:
  typedef FixtureBase<Division>  tSuper;

  ~Division();

  virtual void Destroy();

private:
  friend tSuper;

  static FixtureIntf* Create(Slim::StatementExecutor* executor, Slim::SlimList* args);

  // Fixture methods
  std::string setNumerator(Slim::SlimList* args);
  std::string setDenominator(Slim::SlimList* args); 
  std::string Quotient(Slim::SlimList* args);
  std::string execute(Slim::SlimList* args);
  std::string reset(Slim::SlimList* args);
  std::string table(Slim::SlimList* args);

  float m_numerator;
  float m_denominator;
};
