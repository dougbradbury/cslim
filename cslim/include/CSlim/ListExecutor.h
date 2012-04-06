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