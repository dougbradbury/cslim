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

#include <map>
#include <string>

///////////////////////////////////////////////////////////////////////////////
//
//  SymbolTable is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

namespace Slim
{
  class SlimList;

  class SymbolTable
  {
  public:
    bool Find(std::string const& symbol) const;
    bool Find(char const* symbol, int length) const;

    std::string GetValue(std::string const& symbol) const;
    std::string GetValue(char const* symbol, int length) const;

    void SetValue(char const* symbol, char const* value);

    void ReplaceSymbols(SlimList* list);

    std::string ReplaceStringFrom(std::string src, std::string const& from);

  private:
    std::map<std::string, std::string> m_symbols;
  };
}