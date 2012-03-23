#pragma once

#include "../../SlimExecutable/Fixture.h"

class TestSlimAgain : public FixtureBase<TestSlimAgain>
{
public:
  typedef FixtureBase<TestSlimAgain>  tSuper;

  virtual void Destroy();

private:
  friend tSuper;

  static FixtureIntf* Create(Slim::StatementExecutor* executor, Slim::SlimList* args);

  // Fixture methods
  std::string setArgAgain(Slim::SlimList* args);
  std::string getArgAgain(Slim::SlimList* args);

  std::string arg;
};
