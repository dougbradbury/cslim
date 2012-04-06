#pragma once

#include "Fixture.h"

class Count : public Slim::FixtureBase<Count>
{
public:
  typedef Slim::FixtureBase<Count>  tSuper;

  Count();
  ~Count();

  virtual void Destroy();

private:
  friend tSuper;

  static Slim::FixtureIntf* Create(Slim::StatementExecutor* executor, Slim::SlimList* args);

  // Fixture methods
  std::string count(Slim::SlimList* args);
  std::string counter(Slim::SlimList* args);

  size_t m_count;
};
