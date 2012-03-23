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