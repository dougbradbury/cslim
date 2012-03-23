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

#include <string>

namespace Slim
{
  namespace Detail
  {
    class Fixtures;
    class Instances;
  }

  class FixtureIntf;
  class SlimList;
  class SymbolTable;

  class StatementExecutor;
  typedef boost::function<FixtureIntf* (StatementExecutor*, SlimList*)> Constructor;

  class StatementExecutor
  {
  public:
    StatementExecutor();
    ~StatementExecutor();

    typedef boost::function<void (StatementExecutor*)>  AddFixtureFunc;
    void AddFixture(AddFixtureFunc const& addFixture);

    void RegisterFixture(char const* className, Constructor const& constructor);

    std::string Make(char const* instanceName, char const* className, SlimList* args);
    std::string Call(char const* instanceName, char const* methodName, SlimList* args);
    void SetSymbol(char const* symbol, char const* value);

    void ConstructorError(char const* message);
    std::string FixtureError(char const* message);

  private:
    std::string message;
    char const* userMessage;

    SymbolTable* symbolTable;
    Detail::Fixtures* fixtures;
    Detail::Instances* instances;
  };
}

