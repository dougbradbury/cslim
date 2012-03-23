#pragma once

#include "../../SlimExecutable/Fixture.h"

class TestSlim : public FixtureBase<TestSlim>
{
public:
  typedef FixtureBase<TestSlim>  tSuper;

  TestSlim(Slim::StatementExecutor* executor, Slim::SlimList* args);

  virtual void Destroy();

private:
  friend tSuper;

  static FixtureIntf* Create(Slim::StatementExecutor* executor, Slim::SlimList* args);

  // Fixture methods
  std::string returnValue(Slim::SlimList* args);
  std::string noArgs(Slim::SlimList* args);
  std::string echo(Slim::SlimList* args);
  std::string add(Slim::SlimList* args);
  std::string null(Slim::SlimList* args);
  std::string setArg(Slim::SlimList* args);
  std::string getArg(Slim::SlimList* args);
  std::string getConstructionArg(Slim::SlimList* args);
  std::string returnError(Slim::SlimList* args);

  int noArgsCalled;
  std::string constructionArg;
  std::string arg;
  char echoBuf[50];
  Slim::StatementExecutor* statementExecutor;
};
