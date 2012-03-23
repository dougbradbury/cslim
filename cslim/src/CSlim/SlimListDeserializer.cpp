#include "SlimList.h"

#include <boost/lexical_cast.hpp>

#include <sstream>

namespace
{
  inline bool Skip(std::istringstream& is, char const c)
  {
    char ic;
    is >> ic;
    return ic == c;
  }

  int readLength(std::istringstream& is)
  {
    std::string length(6, 0);
    is.read(&length[0], 6);
    return boost::lexical_cast<int>(length);
  }
}

namespace Slim
{
  SlimList* SlimList::Deserialize(std::string const& serializedList)
  {
    if (serializedList.empty())
    {
      return 0;
    }

    std::istringstream is(serializedList);
    is.exceptions(std::ios_base::eofbit|std::ios_base::failbit|std::ios_base::badbit);
    std::unique_ptr<SlimList> list(new SlimList());

    try
    {
      if (!Skip(is, '['))
      {
        return nullptr;
      }

      int listLength = readLength(is);
      if (!Skip(is, ':'))
      {
        return nullptr;
      }

      while (listLength--)
      {
        int elementLength = readLength(is);
        if (!Skip(is, ':'))
        {
          return nullptr;
        }

        std::string value(elementLength, 0);
        is.read(&value[0], elementLength);
        list->AddString(value);

        if (!Skip(is, ':'))
        {
          return nullptr;
        }
      }

      if (!Skip(is, ']'))
      {
        return nullptr;
      }

      return list.release();
    }
    catch (std::ios_base::failure const&)
    {
      return nullptr;
    }
  }
}
