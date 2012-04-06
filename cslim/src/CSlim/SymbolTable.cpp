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

#include "SymbolTable.h"

#include "SlimList.h"

#include <boost/algorithm/string/replace.hpp>

namespace
{
  int lengthOfSymbol(char const* start)
  {
    int length = 0;
    while(isalnum(*start))
    {
      start++;
      length ++;
    }
    return length;
  }
}

namespace Slim
{
  bool SymbolTable::Find(std::string const& symbol) const
  {
    return m_symbols.find(symbol) != m_symbols.end();
  }

  bool SymbolTable::Find(char const* symbol, int length) const 
  {
    return Find(std::string(symbol, length));
  }

  std::string SymbolTable::GetValue(std::string const& symbol) const 
  {
    auto it = m_symbols.find(symbol);
    if (it == m_symbols.end())
    {
      throw std::exception("Can't find symbol in symbol table.");
    }

    return it->second;
  }

  std::string SymbolTable::GetValue(char const* symbol, int length) const
  {
    return GetValue(std::string(symbol, length));
  }

  // This function will create a new entry if symbol doesn't exist yet.
  void SymbolTable::SetValue(char const* symbol, char const* value) 
  {
    m_symbols[symbol] = value;
  }

  void SymbolTable::ReplaceSymbols(SlimList* list) 
  {
    for (int i = 0; i < list->GetLength(); ++i) 
    {
      std::string src = list->GetStringAt(i);
      std::unique_ptr<SlimList> embeddedList(SlimList::Deserialize(src));
      if (!embeddedList) 
      {
        std::string replacedString = ReplaceStringFrom(src, src);
        list->ReplaceAt(i, replacedString.c_str());
      } 
      else 
      {
        ReplaceSymbols(embeddedList.get());
        std::string serializedReplacedList = SlimList::Serialize(embeddedList.get());
        list->ReplaceAt(i, serializedReplacedList.c_str());
      }
    }
  }

  std::string SymbolTable::ReplaceStringFrom(std::string src, std::string const& from)
  {
    size_t pos = from.find('$');
    if (std::string::npos == pos)
    {
      return src;
    }

    char const* dollarSign = from.c_str() + pos;
    int length = lengthOfSymbol(dollarSign + 1);
    std::string symbol(dollarSign, length + 1);
    try
    {
      std::string symbolValue = GetValue(dollarSign + 1, length);
      boost::algorithm::replace_all(src
        , symbol
        , symbolValue);
      return ReplaceStringFrom(src, src);
    }
    catch (std::exception const&)
    {
      // eat exception.
    }

    // We are running out of data.
    if (from.size() == (pos + 1))
    {
      return src;
    }

    return ReplaceStringFrom(src, dollarSign + 1);
  }

}