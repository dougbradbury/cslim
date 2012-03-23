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

