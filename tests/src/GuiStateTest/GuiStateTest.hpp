
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_GUISTATESTEST
#define DUCTTAPE_ENGINE_TESTS_GUISTATESTEST

#include <Config.hpp>

#include "Test.hpp"

#include <Core/Root.hpp>
//#include <Event/EventListener.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Scene/Scene.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiManager.hpp>
#include <Gui/GuiRootWindow.hpp>
#include <Graphics/TextComponent.hpp>

#include <OgreFontManager.h>

#include <memory>

namespace GuiStateTest {

class GuiStateTest : public Test {
public:
    bool run(int argc, char** argv);
    QString getTestName();
};

////////////////////////////////////////////////////////////////

class SecondState : public dt::State {
    Q_OBJECT
public:
    void onInitialize();

    void onDeinitialize();

    void onClick(MyGUI::Widget* sender);

    void updateStateFrame(double simulation_frame_time);

private:
    dt::GuiButton* mReturnButton;
};

////////////////////////////////////////////////////////////////

class FirstState : public dt::State {
    Q_OBJECT
public:
    void onInitialize();

    void onDeinitialize();

    void onClick(MyGUI::Widget* sender);

    void updateStateFrame(double simulation_frame_time);

private:
    dt::GuiButton* mNextButton;
};

} // namespace test_physics

#endif
