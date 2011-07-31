
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------


namespace dt {

WindowClosedEvent::WindowClosedEvent() {}

const std::string WindowClosedEvent::GetType() const {
    return "DT_WINDOWCLOSEDEVENT";
}

std::shared_ptr<Event> WindowClosedEvent::Clone() const {
    std::shared_ptr<Event> ptr(new WindowClosedEvent());
    return ptr;
}

}
