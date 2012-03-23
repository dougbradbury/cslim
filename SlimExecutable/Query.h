#pragma once

#include "Fixture.h"

class EmployeePayRecordsRow : public FixtureBase<EmployeePayRecordsRow>
{
public:
  typedef FixtureBase<EmployeePayRecordsRow>  tSuper;

  ~EmployeePayRecordsRow();

  virtual void Destroy();

private:
  friend tSuper;

  static FixtureIntf* Create(Slim::StatementExecutor* executor, Slim::SlimList* args);

  // Fixture methods
  std::string query(Slim::SlimList* args);
};
