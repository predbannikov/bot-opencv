#include "lowlvlenginescript.h"

LowLvlEngineScript::LowLvlEngineScript()
{

}

BaseAction *LowLvlEngineScript::parseAction()
{
    for(int i = 0; i < actions.size(); i++) {
        if(command[0] == actions[i]->getName())
            return actions[i];
    }
    return nullptr;
}

void LowLvlEngineScript::update()
{
    BaseAction  *action;
    while(cycle) {
        readCommand(command);
        action = parseAction();
        if(action->getName() != "DEBUG") {
            qDebug() << "";
            qDebug() << "";
            qDebug() << "";
//            qDebug() << "new action" << action->getName();
        }
        do {
            capture->update();
//            action->printDebug();
            QCoreApplication::processEvents();
        } while(!action->perform(command) && cycle);
        QCoreApplication::processEvents();
    }
}

void LowLvlEngineScript::init()
{
    m_sock = new SocketIO(QPoint( g_screen.x(), g_screen.y()));

    actions.append( new SimLegendKult(capture, m_sock));
    actions.append( new ActionWait(capture, m_sock));
    actions.append( new ActionDebug(capture, m_sock));
    actions.append( new RestorGame(capture, m_sock));
    actions.append( new ImageExpectedClose(capture, m_sock));
    actions.append( new ImageExpected(capture, m_sock));
    actions.append( new SendEventControl(capture, m_sock));
    actions.append( new Marker(capture, m_sock));
    actions.append( new GetStrStaticField(capture, m_sock));
    actions.append( new PanelInventory(capture, m_sock));
    actions.append( new Manufacturing(capture, m_sock));
    actions.append( new fishing(capture, m_sock));
    actions.append( new Tests(capture, m_sock));
    actions.append( new ClickerOfTime(capture, m_sock));
}

LowLvlEngineScript::~LowLvlEngineScript()
{
    delete m_sock;
}
