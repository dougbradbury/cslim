#pragma once

#include "Fixture.h"

class Multiplication : public FixtureBase<Multiplication>
{
public:
  typedef FixtureBase<Multiplication>  tSuper;

  ~Multiplication();

  virtual void Destroy();

private:
  friend tSuper;

  static FixtureIntf* Create(Slim::StatementExecutor* executor, Slim::SlimList* args);

  // Fixture methods
  std::string setMultiplicand1(Slim::SlimList* args);
  std::string setMultiplicand2(Slim::SlimList* args); 
  std::string Product(Slim::SlimList* args);

  float m_m1;
  float m_m2;
};
