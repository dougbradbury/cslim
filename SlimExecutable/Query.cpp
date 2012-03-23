#include "Query.h"
#include "Fixtures.h"

#include "SlimList.h"
#include "StatementExecutor.h"

using namespace std;

SLIM_BEGIN_FIXTURE(EmployeePayRecordsRow) 
  SLIM_FUNCTION(query)
SLIM_END_FIXTURE


FixtureIntf* EmployeePayRecordsRow::Create(Slim::StatementExecutor* executor, Slim::SlimList* args)
{
  return new EmployeePayRecordsRow();
}

EmployeePayRecordsRow::~EmployeePayRecordsRow()
{

}

void EmployeePayRecordsRow::Destroy()
{
  delete this;
}

string EmployeePayRecordsRow::query(Slim::SlimList* args)
{
  Slim::SlimList* id = new Slim::SlimList();
  id->AddString("id");
  id->AddString("1");

  Slim::SlimList* pay = new Slim::SlimList();
  pay->AddString("pay");
  pay->AddString("1000");

  Slim::SlimList* record1 = new Slim::SlimList();
  record1->AddList(id);
  record1->AddList(pay);

  Slim::SlimList* records = new Slim::SlimList();
  records->AddList(record1);

  string result = Slim::SlimList::Serialize(records);

  delete id;
  delete pay;
  delete record1;
  delete records;

  return result;
}
