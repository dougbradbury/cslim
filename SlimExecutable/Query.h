#pragma once

#include "Fixture.h"

class EmployeePayRecordsRow : public Slim::FixtureBase<EmployeePayRecordsRow>
{
public:
  typedef Slim::FixtureBase<EmployeePayRecordsRow>  tSuper;

  ~EmployeePayRecordsRow();

  virtual void Destroy();

private:
  friend tSuper;

  static Slim::FixtureIntf* Create(Slim::StatementExecutor* executor, Slim::SlimList* args);

  // Fixture methods
  std::string query(Slim::SlimList* args);
};
