#pragma once

#include "Fixture.h"

class ExceptionsExample : public Slim::FixtureBase<ExceptionsExample>
{
public:
  typedef Slim::FixtureBase<ExceptionsExample>  tSuper;

  ~ExceptionsExample();

  virtual void Destroy();

private:
  friend tSuper;

  static Slim::FixtureIntf* Create(Slim::StatementExecutor* executor, Slim::SlimList* args);

  // Fixture methods
  std::string setTrouble(Slim::SlimList* args);
};
