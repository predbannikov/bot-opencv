#include "clickeroftime.h"

ClickerOfTime::ClickerOfTime(CaptureWindow *aCapture, SocketIO *aSock) : BaseAction(aCapture, aSock)
{
    m_sActionName = "CLICKEROFTIME";
}

void ClickerOfTime::init(QStringList &asListParam)
{
    sys_timeWaitMSec = 2147483647;
    state = STATE_1;
    timer.restart();
    countClick = 0;
//    QPoint p1 = capture->getPoint("rect_pushMassCraft");
//    pointClick = cv::Point(p1.x(), p1.y());
//    QPoint p2 = capture->getPoint("rect_fieldForRebuild");
//    pointRebuild = cv::Point(p2.x(), p2.y());
}

bool ClickerOfTime::logic(QStringList &asListParam)
{
    mouse_click(0);
    QThread::msleep(175);
//    push_key("e");
//    QThread::msleep(4000);


//    push_key("e");rect_fieldForClick
//    mouse_click(capture->getPoint("rect_pushMassCraft"));
//    switch (state) {
//    case STATE_1:
//        if(countClick >= 10) {
////            mouse_move(pointRebuild);

//            state = STATE_2;
//            countClick = 0;
//            mouse_move_rel(+150, -75);
//        } else {
////            mouse_move_click(pointClick.x, pointClick.y + 50);
//            mouse_click(0);
//        }
//        countClick++;
//        break;
//    case STATE_2:
////        mouse_move_click(pointRebuild.x, pointRebuild.y - 50);
//        mouse_move_rel(-150, +75);
//        mouse_click(0);

//        state = STATE_1;
//        break;
//    }

//    QThread::msleep(100);
    return false;
}

void ClickerOfTime::reset()
{
}



