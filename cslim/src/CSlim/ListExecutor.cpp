// Fitnesse Slim plugin port for VC++ 2010
// 
// Copyright (C) 2012  Chih C. Von
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "ListExecutor.h"

#include "SlimList.h"
#include "StatementExecutor.h"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>

namespace
{
  void AddResult(Slim::SlimList* list, std::string const& id, char const* result)
  {
    Slim::SlimList* pair = new Slim::SlimList();	
    pair->AddString(id);
    pair->AddString(result);
    list->AddList(pair);
    delete pair;	
  }
}

namespace Slim
{
  ListExecutor::ListExecutor(StatementExecutor* executor)
    : m_executor(executor)
  {
    m_cmdTable["import"] = boost::bind(&ListExecutor::Import, this, _1);
    m_cmdTable["make"] = boost::bind(&ListExecutor::Make, this, _1);
    m_cmdTable["call"] = boost::bind(&ListExecutor::Call, this, _1);
    m_cmdTable["callandassign"] = boost::bind(&ListExecutor::CallAndAssign, this, _1);
  }

  SlimList* ListExecutor::Execute(SlimList* instructions)
  {
    SlimList* results = new SlimList();

    int numberOfInstructions = instructions->GetLength();
    for (int n = 0; n < numberOfInstructions; ++n) 
    {
      SlimList* instruction = instructions->GetListAt(n);
      std::string id = instruction->GetStringAt(0);
      std::string result = Dispatch(instruction);
      AddResult(results, id, result.c_str());		
    }

    return results;	
  }

  std::string ListExecutor::Dispatch(SlimList* instruction) 
  {
    std::string command = instruction->GetStringAt(1);
    boost::algorithm::to_lower(command);
    auto it = m_cmdTable.find(command);
    if (it != m_cmdTable.end())
    {
      return it->second(instruction);
    }

    return InvalidCommand(instruction);
  }

  std::string ListExecutor::InvalidCommand(SlimList* instruction)
  {
    std::string id = instruction->GetStringAt(0);
    std::string command = instruction->GetStringAt(1);
    return (boost::format("__EXCEPTION__:message:<<INVALID_STATEMENT [\"%1%\", \"%2%\"].>>") % id % command).str();
  }

  std::string ListExecutor::MalformedInstruction(SlimList* instruction)
  {
    return (boost::format("__EXCEPTION__:message:<<MALFORMED_INSTRUCTION %1%.>>") % instruction->ToString()).str();
  }

  std::string ListExecutor::Import(SlimList* instruction)
  {
    return "OK";
  }

  std::string ListExecutor::Make(SlimList* instruction)
  {
    std::string instanceName = instruction->GetStringAt(2);
    std::string className = instruction->GetStringAt(3);
    SlimList* args = instruction->GetTailAt(4);
    std::string result = m_executor->Make(instanceName.c_str(), className.c_str(), args);
    delete args;
    return result;
  }

  std::string ListExecutor::Call(SlimList* instruction)
  {
    if (instruction->GetLength() < 4)
    {
      return MalformedInstruction(instruction);
    }

    std::string instanceName = instruction->GetStringAt(2);
    std::string methodName = instruction->GetStringAt(3);
    SlimList* args = instruction->GetTailAt(4);

    std::string result = m_executor->Call(instanceName.c_str(), methodName.c_str(), args);
    delete args;
    return result;
  }

  std::string ListExecutor::CallAndAssign(SlimList* instruction)
  {
    if (instruction->GetLength() < 5)
    {
      return MalformedInstruction(instruction);
    }

    std::string symbolName = instruction->GetStringAt(2);
    std::string instanceName = instruction->GetStringAt(3);
    std::string methodName = instruction->GetStringAt(4);
    SlimList* args = instruction->GetTailAt(5);

    std::string result = m_executor->Call(instanceName.c_str(), methodName.c_str(), args);
    m_executor->SetSymbol(symbolName.c_str(), result.c_str());
    delete args;
    return result;
  }

}