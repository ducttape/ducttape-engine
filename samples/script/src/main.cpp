
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
#include <Graphics/GuiWidgetComponent.hpp>
#include <Utils/Utils.hpp>

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
        QString text(mInput->getCaption().asUTF8_c_str());

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
                mInput->setCaption(mHistory.at(mHistoryIndex).toStdString());
            } else {
                // we are below, so just be emtpy / current command
                mInput->setCaption(mCurrentCommand.toStdString());
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
        Execute(QString(mInput->getCaption().asUTF8_c_str()));
        mInput->setCaption("");
    }

    void EditSubmitted(MyGUI::Edit* _sender) {
        Execute(QString(mInput->getCaption().asUTF8_c_str()));
        mInput->setCaption("");
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
        mOutput->addText(
                    (mOutput->getCaption() != "" ? "\n" : "") // newline, but not if empty
                     + color.toStdString() + text.toStdString());
    }

    void Clear() {
        mOutput->setCaption("");
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
        THE_MAIN->Write("    "+CMD_COLOR+"quit;"+INFO_COLOR+"        -- terminates the application", HELP_COLOR);
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
        dt::Node* node;
        dt::GuiWidgetComponent* component;


        node = scene->AddChildNode(new dt::Node("output"));
        node->SetPosition(Ogre::Vector3(10, 10, 0));
        node->SetScale(Ogre::Vector3(780, 550, 0));
        component = node->AddComponent(new dt::GuiWidgetComponent("Edit", "EditEmpty", "output"));
        component->SetUsesPixelCoordinates(true);
        mOutput = component->GetWidget<MyGUI::Edit>();
        mOutput->setTextColour(MyGUI::Colour::White);
        mOutput->setEditMultiLine(true);
        mOutput->setEditReadOnly(true);
        mOutput->setTextAlign(MyGUI::Align::Bottom | MyGUI::Align::Left);

        node = scene->AddChildNode(new dt::Node("input"));
        node->SetPosition(Ogre::Vector3(10, 570, 0));
        node->SetScale(Ogre::Vector3(700, 20, 0));
        component = node->AddComponent(new dt::GuiWidgetComponent("Edit", "Edit", "input"));
        component->SetUsesPixelCoordinates(true);
        mInput = component->GetWidget<MyGUI::Edit>();
        mInput->setEditMultiLine(false);
        mInput->setEditReadOnly(false);
        mInput->eventEditSelectAccept = MyGUI::newDelegate(this, &Main::EditSubmitted);
        mInput->eventKeyButtonPressed = MyGUI::newDelegate(this, &Main::KeyPressed);
        mInput->setTextAlign(MyGUI::Align::Bottom | MyGUI::Align::Left);

        node = scene->AddChildNode(new dt::Node("button"));
        node->SetPosition(Ogre::Vector3(720, 570, 0));
        node->SetScale(Ogre::Vector3(70, 20, 0));
        component = node->AddComponent(new dt::GuiWidgetComponent("Button", "Button", "button"));
        component->SetUsesPixelCoordinates(true);
        mButton = component->GetWidget<MyGUI::Button>();
        mButton->setCaption("Run");
        mButton->eventMouseButtonClick = MyGUI::newDelegate(this, &Main::SubmitClicked);

        MyGUI::InputManager::getInstance().setKeyFocusWidget(mInput);

        InfoFunction(nullptr, dt::ScriptManager::Get()->GetScriptEngine());
    }

private:
    MyGUI::Edit* mOutput;
    MyGUI::Edit* mInput;
    MyGUI::Button* mButton;
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
