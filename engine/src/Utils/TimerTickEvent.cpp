
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <TimerTickEvent.hpp>

#include <memory>
#include <string>

namespace dt {

class Event;

TimerTickEvent::TimerTickEvent(const std::string& message, double interval)
    : MessageEvent(message),
      mInterval(interval) {}

const std::string TimerTickEvent::GetType() const {
    return "DT_TIMERTICKEVENT";
}

std::shared_ptr<Event> TimerTickEvent::Clone() const {
    std::shared_ptr<Event> ptr(new TimerTickEvent(mMessage, mInterval));
    return ptr;
}

double TimerTickEvent::GetInterval() const {
    return mInterval;
}

} // namespace dt
