#pragma once

#include "Fixture.h"

class Count : public FixtureBase<Count>
{
public:
  typedef FixtureBase<Count>  tSuper;

  Count();
  ~Count();

  virtual void Destroy();

private:
  friend tSuper;

  static FixtureIntf* Create(Slim::StatementExecutor* executor, Slim::SlimList* args);

  // Fixture methods
  std::string count(Slim::SlimList* args);
  std::string counter(Slim::SlimList* args);

  size_t m_count;
};
