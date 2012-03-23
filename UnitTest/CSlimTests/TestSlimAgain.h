#pragma once

#include "Fixture.h"

class TestSlimAgain : public Slim::FixtureBase<TestSlimAgain>
{
public:
  typedef Slim::FixtureBase<TestSlimAgain>  tSuper;

  virtual void Destroy();

private:
  friend tSuper;

  static Slim::FixtureIntf* Create(Slim::StatementExecutor* executor, Slim::SlimList* args);

  // Fixture methods
  std::string setArgAgain(Slim::SlimList* args);
  std::string getArgAgain(Slim::SlimList* args);

  std::string arg;
};
