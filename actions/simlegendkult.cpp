#include "simlegendkult.h"

SimLegendKult::SimLegendKult(CaptureWindow *aCapture, SocketIO *aSock) : BaseAction(aCapture, aSock)
{
    m_sActionName = "SIMLEGENDKULT";
}

void SimLegendKult::init(QStringList &asListParam)
{
    sys_timeWaitMSec = 2147483647;
    timer.restart();
    statePump = STATE_PUMP_1;
}

bool SimLegendKult::logic(QStringList &asListParam)
{
    switch (statePump) {

    }
//    push_key("e");
//    mouse_click(capture->getPoint("rect_pushMassCraft"));
    if(countClick = 10) {
//        mouse_move();
        countClick = 0;
//        mouse_move_rel(-100, 100);
    }
    mouse_click(0);
    QThread::msleep(225);
    return false;
}

void SimLegendKult::reset()
{
}


