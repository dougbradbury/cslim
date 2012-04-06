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

#include <boost/format.hpp>

#include <sstream>

namespace
{
  int const ListOverhead = 9;
  int const ElementOverhead = 8;

  std::string nodeStringAt(Slim::SlimList* self, int i)
  {
    std::string nodeString = self->GetStringAt(i);
    if (nodeString.empty())
    {
      nodeString = "null";		
    }

    return nodeString;
  }
}

namespace Slim
{
  int SlimList::SerializedLength(SlimList* self)
  {
    int length = ListOverhead;
    for (int i = 0; i < self->GetLength(); ++i)
    {
      length += nodeStringAt(self, i).size() + ElementOverhead;
    }
    return length;
  }

  std::string SlimList::Serialize(SlimList* self)
  {
    std::ostringstream buf;

    int listLength = self->GetLength();
    buf << (boost::format("[%06d:") % listLength).str();

    for (int i = 0; i < listLength; ++i)
    {
      std::string nodeString = nodeStringAt(self, i);
      buf << (boost::format("%1$06ld:%2%:") % nodeString.size() % nodeString).str();
    }

    buf << "]";
    return buf.str();
  }
}

