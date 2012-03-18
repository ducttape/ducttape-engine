
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include <Config.hpp>

#include <Core/Root.hpp>
#include <Scene/Game.hpp>
#include <Scene/Scene.hpp>
#include <Scene/Node.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Gui/GuiManager.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiEditBox.hpp>
#include <Utils/Utils.hpp>
#include <Graphics/DisplayManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Logic/ScriptManager.hpp>
#include <Scene/StateManager.hpp>

#include <QList>
#include <QString>

#include <sstream>
#include <ios>

QString INFO_COLOR("#FFFF88");
QString CMD_COLOR("#AACCFF");
QString HELP_COLOR(INFO_COLOR);
QString ERROR_COLOR("#AA0000");
//QString INPUT_COLOR("#FFCC33");
QString INPUT_COLOR("#888888");

class Main;

Main* THE_MAIN;

class Main: public dt::State {
public:
    void keyPressed(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char) {
        // scroll history
        QString text(mInput->getCaption());

        int d = 0;
        if(_key == MyGUI::KeyCode::ArrowUp) d = -1;
        else if(_key == MyGUI::KeyCode::ArrowDown) d = 1;

        if(d != 0) {
            if(mHistoryIndex == mHistory.size() && d == -1 && text != "") // we are at the bottom, and want to scroll up, so save the bottom line
                mCurrentCommand = text;

            // now scroll around
            mHistoryIndex += d;

            if(mHistoryIndex < 0) mHistoryIndex = 0; // don't scroll up too fast
            if(mHistoryIndex < mHistory.size()) {
                // we are in the list
                mInput->setCaption(mHistory.at(mHistoryIndex));
            } else {
                // we are below, so just be emtpy / current command
                mInput->setCaption(mCurrentCommand);
            }
        } /*else {
            // save current history entry
            if(mHistoryIndex > 0 && mHistoryIndex < mHistory.size()) {
                // but only if we selected something
                mHistory[mHistoryIndex] = text;
            }
        } */
    }

    void submitClicked(MyGUI::Widget* _sender) {
        execute(mInput->getCaption());
        mInput->setCaption("");
    }

    void editSubmitted(MyGUI::EditBox* _sender) {
        execute(mInput->getCaption());
        mInput->setCaption("");
    }

    void execute(QString script) {
        // write into history
        ++mCommandNumber;
        mHistory.append(script);
        mHistoryIndex = mHistory.size();
        mCurrentCommand = "";

        write("> " + script, INPUT_COLOR);

        if(dt::ScriptManager::get()->evaluate(script, "command-" + dt::Utils::toString(mCommandNumber))) {
            // success, print result
            QScriptValue result = dt::ScriptManager::get()->getLastReturnValue();
            if(!result.isUndefined()) {
                write(result.toString());
            }
        } else {
            QScriptValue error = dt::ScriptManager::get()->getLastReturnValue();
            write(error.toString(), ERROR_COLOR);
        }
    }

    void write(QString text, QString color = "#FFFFFF") {
        mOutput->setCaption(mOutput->getCaption() + "\n" + color + text);
    }

    void clear() {
        mOutput->setCaption("");
    }

    static QScriptValue printFunction(QScriptContext *context, QScriptEngine *engine) {
        QString line;
        for(int i = 0; i < context->argumentCount(); ++i) {
            // append the argument, converted to string
            line.append(context->argument(i).toString());
        }
        THE_MAIN->write(line);
        return engine->undefinedValue();
    }

    static QScriptValue coutFunction(QScriptContext *context, QScriptEngine *engine) {
        QString line;
        for(int i = 0; i < context->argumentCount(); ++i) {
            // append the argument, converted to string
            line.append(context->argument(i).toString());
        }
        dt::Logger::get().info("Script output: " + line);
        return engine->undefinedValue();
    }

    static QScriptValue infoFunction(QScriptContext *context, QScriptEngine *engine) {
        THE_MAIN->write("This is a scripting console example. "
                        "Start by typing a script command into the textfield.\n"
                        "For more info, enter "+CMD_COLOR+"help();"+INFO_COLOR+" below.", INFO_COLOR);
        return engine->undefinedValue();
    }

    static QScriptValue helpFunction(QScriptContext *context, QScriptEngine *engine) {
        THE_MAIN->write("Enter a line of script to execute. The result value (if any) will be displayed.", HELP_COLOR);
        THE_MAIN->write("Available functions:", HELP_COLOR);
        THE_MAIN->write("    "+CMD_COLOR+"help();"+INFO_COLOR+"      -- displays this help", HELP_COLOR);
        THE_MAIN->write("    "+CMD_COLOR+"info();"+INFO_COLOR+"      -- displays general information", HELP_COLOR);
        THE_MAIN->write("    "+CMD_COLOR+"clear();"+INFO_COLOR+"     -- clear the output", HELP_COLOR);
        THE_MAIN->write("    "+CMD_COLOR+"print(x);"+INFO_COLOR+"    -- prints the variable", HELP_COLOR);
        THE_MAIN->write("    "+CMD_COLOR+"cout(x);"+INFO_COLOR+"     -- prints the variable to std::cout", HELP_COLOR);
        THE_MAIN->write("    "+CMD_COLOR+"quit();"+INFO_COLOR+"      -- terminates the application", HELP_COLOR);
        THE_MAIN->write("Use the "+CMD_COLOR+"Up/Down Arrow Keys"+INFO_COLOR+" to scroll your command history.", HELP_COLOR);
        return engine->undefinedValue();
    }

    static QScriptValue clearFunction(QScriptContext *context, QScriptEngine *engine) {
        THE_MAIN->clear();
        return engine->undefinedValue();
    }

    static QScriptValue quitFunction(QScriptContext *context, QScriptEngine *engine) {
        dt::StateManager::get()->pop();
        THE_MAIN->write("Quitting...", INPUT_COLOR);
        return engine->undefinedValue();
    }

    void onInitialize() {
        mCommandNumber = 0;

        QScriptEngine* e = dt::ScriptManager::get()->getScriptEngine();
        e->globalObject().setProperty("print", e->newFunction(Main::printFunction));
        e->globalObject().setProperty("cout", e->newFunction(Main::coutFunction));
        e->globalObject().setProperty("help", e->newFunction(Main::helpFunction));
        e->globalObject().setProperty("info", e->newFunction(Main::infoFunction));
        e->globalObject().setProperty("clear", e->newFunction(Main::clearFunction));
        e->globalObject().setProperty("quit", e->newFunction(Main::quitFunction));
        e->globalObject().setProperty("exit", e->newFunction(Main::quitFunction));


        dt::DisplayManager::get()->setWindowSize(800, 600);
        dt::ResourceManager::get()->addResourceLocation("gui", "FileSystem", true);
        dt::ResourceManager::get()->addResourceLocation("", "FileSystem", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        auto scene = addScene(new dt::Scene("scene"));
        auto camnode = scene->addChildNode(new dt::Node("cam"));
        camnode->addComponent(new dt::CameraComponent("camera"));

        // create GUI
        dt::GuiRootWindow& win = dt::GuiManager::get()->getRootWindow();

        mOutput = win.addChildWidget(new dt::GuiEditBox("output"));
        mOutput->setPosition(10, 10);
        mOutput->setSize(780, 550);
        MyGUI::EditBox* output = dynamic_cast<MyGUI::EditBox*>(mOutput->getMyGUIWidget());
        output->changeWidgetSkin("TextBox");
        output->setEditMultiLine(true);
        output->setEditReadOnly(true);
        output->setTextAlign(MyGUI::Align::Bottom | MyGUI::Align::Left);

        mInput = win.addChildWidget(new dt::GuiEditBox("input"));
        mInput->setPosition(10, 570);
        mInput->setSize(700, 20);
        MyGUI::EditBox* input = dynamic_cast<MyGUI::EditBox*>(mInput->getMyGUIWidget());
        input->setEditMultiLine(false);
        input->setEditReadOnly(false);
        input->eventEditSelectAccept += MyGUI::newDelegate(this, &Main::editSubmitted);
        input->eventKeyButtonPressed += MyGUI::newDelegate(this, &Main::keyPressed);
        input->setTextAlign(MyGUI::Align::Bottom | MyGUI::Align::Left);

        mButton = win.addChildWidget(new dt::GuiButton("button"));
        mButton->setPosition(720, 570);
        mButton->setSize(70, 20);
        mButton->setCaption("Run");
        mButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &Main::submitClicked);


        infoFunction(nullptr, dt::ScriptManager::get()->getScriptEngine());
    }
    void updateStateFrame(double simulation_frame_time) {
    }

private:
    std::shared_ptr<dt::GuiButton> mButton;
    std::shared_ptr<dt::GuiEditBox> mInput;
    std::shared_ptr<dt::GuiEditBox> mOutput;

    QList<QString> mHistory;
    QString mCurrentCommand;
    int mHistoryIndex;
    int mCommandNumber;

};

int main(int argc, char** argv) {
    dt::Game game;
    THE_MAIN = new Main();
    game.run(THE_MAIN, argc, argv);
    return 0;
}
