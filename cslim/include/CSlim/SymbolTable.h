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