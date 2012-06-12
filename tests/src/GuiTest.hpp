
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_GUITEST
#define DUCTTAPE_ENGINE_TESTS_GUITEST

#include <Config.hpp>

#include "Test.hpp"

#include <Core/Root.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiLabel.hpp>
#include <Gui/GuiImageBox.hpp>
#include <Gui/GuiProgressBar.hpp>
#include <Gui/GuiScrollBar.hpp>
#include <Gui/GuiCheckBox.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

namespace GuiTest {

class GuiTest : public Test {
public:
    bool run(int argc, char** argv);
    QString getTestName();
};

////////////////////////////////////////////////////////////////

class Main: public dt::State {
    Q_OBJECT
public:
    Main();
    void click(MyGUI::Widget* _sender);
    void scrollChangePosition(MyGUI::ScrollBar* _sender, size_t _position);
    void onInitialize();
    void updateStateFrame(double simulation_frame_time);

private:
    double mRuntime;
    std::tr1::shared_ptr<dt::GuiProgressBar> progressBar1;
};

} // namespace GuiTest

#endif
