#pragma once

#include "Fixture.h"

class ExceptionsExample : public FixtureBase<ExceptionsExample>
{
public:
  typedef FixtureBase<ExceptionsExample>  tSuper;

  ~ExceptionsExample();

  virtual void Destroy();

private:
  friend tSuper;

  static FixtureIntf* Create(Slim::StatementExecutor* executor, Slim::SlimList* args);

  // Fixture methods
  std::string setTrouble(Slim::SlimList* args);
};
