
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

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
    void KeyPressed(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char) {
        // scroll history
        QString text(mInput->GetCaption());

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
                mInput->SetCaption(mHistory.at(mHistoryIndex));
            } else {
                // we are below, so just be emtpy / current command
                mInput->SetCaption(mCurrentCommand);
            }
        } /*else {
            // save current history entry
            if(mHistoryIndex > 0 && mHistoryIndex < mHistory.size()) {
                // but only if we selected something
                mHistory[mHistoryIndex] = text;
            }
        } */
    }

    void SubmitClicked(MyGUI::Widget* _sender) {
        Execute(mInput->GetCaption());
        mInput->SetCaption("");
    }

    void EditSubmitted(MyGUI::EditBox* _sender) {
        Execute(mInput->GetCaption());
        mInput->SetCaption("");
    }

    void Execute(QString script) {
        // write into history
        ++mCommandNumber;
        mHistory.append(script);
        mHistoryIndex = mHistory.size();
        mCurrentCommand = "";

        Write("> " + script, INPUT_COLOR);

        if(dt::ScriptManager::Get()->Evaluate(script, "command-" + dt::Utils::ToString(mCommandNumber))) {
            // success, print result
            QScriptValue result = dt::ScriptManager::Get()->GetLastReturnValue();
            if(!result.isUndefined()) {
                Write(result.toString());
            }
        } else {
            QScriptValue error = dt::ScriptManager::Get()->GetLastReturnValue();
            Write(error.toString(), ERROR_COLOR);
        }
    }

    void Write(QString text, QString color = "#FFFFFF") {
        mOutput->SetCaption(mOutput->GetCaption() + "\n" + color + text);
    }

    void Clear() {
        mOutput->SetCaption("");
    }

    static QScriptValue PrintFunction(QScriptContext *context, QScriptEngine *engine) {
        QString line;
        for(int i = 0; i < context->argumentCount(); ++i) {
            // append the argument, converted to string
            line.append(context->argument(i).toString());
        }
        THE_MAIN->Write(line);
        return engine->undefinedValue();
    }

    static QScriptValue CoutFunction(QScriptContext *context, QScriptEngine *engine) {
        QString line;
        for(int i = 0; i < context->argumentCount(); ++i) {
            // append the argument, converted to string
            line.append(context->argument(i).toString());
        }
        dt::Logger::Get().Info("Script output: " + line);
        return engine->undefinedValue();
    }

    static QScriptValue InfoFunction(QScriptContext *context, QScriptEngine *engine) {
        THE_MAIN->Write("This is a scripting console example. "
                        "Start by typing a script command into the textfield.\n"
                        "For more info, enter "+CMD_COLOR+"help();"+INFO_COLOR+" below.", INFO_COLOR);
        return engine->undefinedValue();
    }

    static QScriptValue HelpFunction(QScriptContext *context, QScriptEngine *engine) {
        THE_MAIN->Write("Enter a line of script to execute. The result value (if any) will be displayed.", HELP_COLOR);
        THE_MAIN->Write("Available functions:", HELP_COLOR);
        THE_MAIN->Write("    "+CMD_COLOR+"help();"+INFO_COLOR+"      -- displays this help", HELP_COLOR);
        THE_MAIN->Write("    "+CMD_COLOR+"info();"+INFO_COLOR+"      -- displays general information", HELP_COLOR);
        THE_MAIN->Write("    "+CMD_COLOR+"clear();"+INFO_COLOR+"     -- clear the output", HELP_COLOR);
        THE_MAIN->Write("    "+CMD_COLOR+"print(x);"+INFO_COLOR+"    -- prints the variable", HELP_COLOR);
        THE_MAIN->Write("    "+CMD_COLOR+"cout(x);"+INFO_COLOR+"     -- prints the variable to std::cout", HELP_COLOR);
        THE_MAIN->Write("    "+CMD_COLOR+"quit();"+INFO_COLOR+"      -- terminates the application", HELP_COLOR);
        THE_MAIN->Write("Use the "+CMD_COLOR+"Up/Down Arrow Keys"+INFO_COLOR+" to scroll your command history.", HELP_COLOR);
        return engine->undefinedValue();
    }

    static QScriptValue ClearFunction(QScriptContext *context, QScriptEngine *engine) {
        THE_MAIN->Clear();
        return engine->undefinedValue();
    }

    static QScriptValue QuitFunction(QScriptContext *context, QScriptEngine *engine) {
        dt::StateManager::Get()->Pop();
        THE_MAIN->Write("Quitting...", INPUT_COLOR);
        return engine->undefinedValue();
    }

    void OnInitialize() {
        mCommandNumber = 0;

        QScriptEngine* e = dt::ScriptManager::Get()->GetScriptEngine();
        e->globalObject().setProperty("print", e->newFunction(Main::PrintFunction));
        e->globalObject().setProperty("cout", e->newFunction(Main::CoutFunction));
        e->globalObject().setProperty("help", e->newFunction(Main::HelpFunction));
        e->globalObject().setProperty("info", e->newFunction(Main::InfoFunction));
        e->globalObject().setProperty("clear", e->newFunction(Main::ClearFunction));
        e->globalObject().setProperty("quit", e->newFunction(Main::QuitFunction));
        e->globalObject().setProperty("exit", e->newFunction(Main::QuitFunction));


        dt::DisplayManager::Get()->SetWindowSize(800, 600);
        dt::ResourceManager::Get()->AddResourceLocation("gui", "FileSystem", true);
        dt::ResourceManager::Get()->AddResourceLocation("", "FileSystem", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        dt::Scene* scene = AddScene(new dt::Scene("scene"));
        dt::Node* camnode = scene->AddChildNode(new dt::Node("cam"));
        camnode->AddComponent(new dt::CameraComponent("camera"));

        // create GUI
        dt::GuiRootWindow& win = dt::GuiManager::Get()->GetRootWindow();

        mOutput = win.CreateChild<dt::GuiEditBox>("output");
        mOutput->SetPosition(10, 10);
        mOutput->SetSize(780, 550);
        MyGUI::EditBox* output = dynamic_cast<MyGUI::EditBox*>(mOutput->GetMyGUIWidget());
        output->changeWidgetSkin("TextBox");
        output->setEditMultiLine(true);
        output->setEditReadOnly(true);
        output->setTextAlign(MyGUI::Align::Bottom | MyGUI::Align::Left);

        mInput = win.CreateChild<dt::GuiEditBox>("input");
        mInput->SetPosition(10, 570);
        mInput->SetSize(700, 20);
        MyGUI::EditBox* input = dynamic_cast<MyGUI::EditBox*>(mInput->GetMyGUIWidget());
        input->setEditMultiLine(false);
        input->setEditReadOnly(false);
        input->eventEditSelectAccept += MyGUI::newDelegate(this, &Main::EditSubmitted);
        input->eventKeyButtonPressed += MyGUI::newDelegate(this, &Main::KeyPressed);
        input->setTextAlign(MyGUI::Align::Bottom | MyGUI::Align::Left);

        mButton = win.CreateChild<dt::GuiButton>("button");
        mButton->SetPosition(720, 570);
        mButton->SetSize(70, 20);
        mButton->SetCaption("Run");
        dynamic_cast<MyGUI::Button*>(mButton->GetMyGUIWidget())->eventMouseButtonClick += MyGUI::newDelegate(this, &Main::SubmitClicked);


        InfoFunction(nullptr, dt::ScriptManager::Get()->GetScriptEngine());
    }

private:
    dt::GuiButton* mButton;
    dt::GuiEditBox* mInput;
    dt::GuiEditBox* mOutput;

    QList<QString> mHistory;
    QString mCurrentCommand;
    int mHistoryIndex;
    int mCommandNumber;

};

int main(int argc, char** argv) {
    dt::Game game;
    THE_MAIN = new Main();
    game.Run(THE_MAIN, argc, argv);
    return 0;
}
