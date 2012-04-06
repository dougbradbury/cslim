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
