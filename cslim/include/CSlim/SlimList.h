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

#include <string>
#include <vector>

namespace Slim
{
  class SlimList;

  struct SlimListNode
  {
    SlimListNode()
      : sublist(0)
    {
    }

    std::string value;
    SlimList* sublist;
  };

  class SlimList
  {
  public:
    ~SlimList();

    void AddString(std::string value);
    void AddList(SlimList* element);
    int GetLength();
    bool Equals(SlimList const* other) const;
    SlimList *GetListAt(int index);
    std::string GetStringAt(int index);
    SlimList* GetHashAt(int index);
    void ReplaceAt(int index, char const * replacementString);
    SlimList* GetTailAt(int index);
    std::string ToString();

    // Serialization...
    static std::string Serialize(Slim::SlimList*);
    static int SerializedLength(Slim::SlimList*);

    // Deserialization...
    static Slim::SlimList* Deserialize(std::string const& serializedList);

  private:
    SlimListNode* GetNodeAt(int index);
    std::vector<SlimListNode> m_nodes;
  };

  inline bool operator==(SlimList const& rhs, SlimList const& lhs)
  {
    return rhs.Equals(&lhs);
  }
}