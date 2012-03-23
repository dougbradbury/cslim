#include "SlimList.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/range/algorithm/for_each.hpp>

namespace
{
  std::string const hashRowOpenTag = "<tr>";
  std::string const hashRowCloseTag = "</tr>";
  std::string const hashCellOpenTag = "<td>";
  std::string const hashCellCloseTag = "</td>";

  std::string parseHashCell(char const*& cellStart)
  {
    char const* cellValue = cellStart + hashCellOpenTag.size();
    char const* cellStop = strstr(cellValue, hashCellCloseTag.c_str());
    std::string buf(cellValue, cellStop - cellValue);
    cellStart = strstr(cellStop + hashCellOpenTag.size(), hashCellOpenTag.c_str());
    return buf;
  }

  Slim::SlimList* parseHashEntry(char const* row)
  {
    Slim::SlimList* element = new Slim::SlimList();

    char const* cellStart = strstr(row, hashCellOpenTag.c_str());
    std::string hashKey = parseHashCell(cellStart);
    element->AddString(hashKey);

    std::string hashValue = parseHashCell(cellStart);
    element->AddString(hashValue);

    return element;
  }
}

namespace Slim
{
  SlimList::~SlimList()
  {
    boost::for_each(m_nodes,
      [](SlimListNode const& node)
      {
        delete node.sublist;
      });
  }

  void SlimList::AddString(std::string value)
  {
    SlimListNode newNode;
    newNode.value = value;
    m_nodes.push_back(newNode);
  }

  void SlimList::AddList(SlimList* element)
  {
    AddString(Serialize(element));
  }

  int SlimList::GetLength()
  {
    return m_nodes.size();
  }

  bool SlimList::Equals(SlimList const* other) const
  {
    return boost::equals(m_nodes, other->m_nodes, 
      [](SlimListNode const& lhs, SlimListNode const& rhs)->bool
      {
        return lhs.value == rhs.value;
      });
  }

  SlimList* SlimList::GetListAt(int index)
  {
    SlimListNode* node = GetNodeAt(index);
    if (!node)
    {
      return 0;
    }

    if (!node->sublist)
    {
      node->sublist = Deserialize(node->value);	
    }
    return node->sublist;
  }

  std::string SlimList::GetStringAt(int index)
  {
    SlimListNode* node = GetNodeAt(index);
    if (!node)
    {
      return std::string();
    }

    return node->value;
  }

  SlimList* SlimList::GetHashAt(int index)
  {
    SlimList* hash = new SlimList();
    SlimList* element = 0;

    std::string value = GetStringAt(0);
    char const* row = strstr(value.c_str(), hashRowOpenTag.c_str());
    while (row != NULL)
    {
      element = parseHashEntry(row);
      hash->AddList(element);
      delete element;
      row = strstr(row + hashRowOpenTag.size(), hashRowOpenTag.c_str());
    }

    return hash;
  }

  SlimListNode* SlimList::GetNodeAt(int index)
  {
    if (index >= GetLength())
    {
      return 0;
    }

    return &m_nodes[index];
  }

  void SlimList::ReplaceAt(int index, char const* replacementString)
  {
    SlimListNode* node = GetNodeAt(index);
    node->value = replacementString;
    if (node->sublist)
    {
      delete node->sublist;
      node->sublist = 0;
    }
  }

  SlimList* SlimList::GetTailAt(int index)
  {
    SlimList* tail = new SlimList();
    for(int length = GetLength(); index < length; ++index) 
    {
      tail->AddString(GetStringAt(index).c_str());
    }
    return tail;
  }

  std::string SlimList::ToString()
  {
    std::string buf("[");
    for (int i = 0; i < GetLength(); ++i) 
    {
      SlimList* sublist = GetListAt(i);
      if (sublist) 
      {
        buf += sublist->ToString();
      } 
      else 
      {
        buf += '"';
        buf += GetStringAt(i);
        buf += '"';
      }

      if (i != (GetLength() - 1)) 
      {
        buf += ", ";
      }	
    }

    buf += ']';
    return buf;	
  }
}