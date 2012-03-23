#pragma once

#include <boost/function.hpp>

#include <map>
#include <string>

///////////////////////////////////////////////////////////////////////////////
//
//  ListExecutor is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

namespace Slim
{
  class StatementExecutor;
  class SlimList;

  class ListExecutor
  {
  public:
    ListExecutor(StatementExecutor* executor);
    SlimList* Execute(SlimList* instructions);

  private:
    std::string Dispatch(SlimList* instruction);
    std::string InvalidCommand(SlimList* instruction);
    std::string MalformedInstruction(SlimList* instruction);
    std::string Import(SlimList* instruction);
    std::string Make(SlimList* instruction);
    std::string Call(SlimList* instruction);
    std::string CallAndAssign(SlimList* instruction);

    typedef boost::function<std::string (SlimList*)>  SlimCommandHandler;
    typedef std::map<std::string, SlimCommandHandler>  CommandLookupTable;
    CommandLookupTable m_cmdTable;

    StatementExecutor* m_executor;
  };
}