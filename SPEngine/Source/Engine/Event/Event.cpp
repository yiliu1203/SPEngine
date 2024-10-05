
#include "Event.h"
#include "PCH.h"


namespace SP {

std::ostream& operator<<(std::ostream& os, const Event& event)
{
    return os << event.ToString();
}



}   // namespace SP
