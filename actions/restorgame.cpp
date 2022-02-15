#include "restorgame.h"

RestorGame::RestorGame(CaptureWindow *aCapture, SocketIO *aSock) : BaseAction(aCapture, aSock)
{
    m_sActionName = "RESTORGAME";
}

void RestorGame::init(QStringList &asListParam)
{

}

bool RestorGame::logic(QStringList &asListParam)
{
    QThread::msleep(500);
    point = QPoint(g_screen.width()/2, g_screen.height()/2);
//    press_key("ALT");
//    push_key("TAB");
//    release_key("ALT");
//    int relat = 25;
//    int x = point.x();
//    int y = point.y();
//    mouse_move_click(g_screen.width()/2, g_screen.height()/2);
////    mouse_move(x, y);
//    for(int i = 0; i < 1; i++) {
//        ulong wait = 30;
//        QThread::msleep(wait);
//        mouse_move_rel(relat, 0);
//        QThread::msleep(wait);
//        mouse_move_rel(0, relat);
//        QThread::msleep(wait);
//        mouse_move_rel(-relat, 0);
//        QThread::msleep(wait);
//        mouse_move_rel(0, -relat);
//        QThread::msleep(wait);
//        QThread::msleep(waitMSec);

//    }
//    mouse_move_click(x, y);
//    mouse_click(1);
//    QThread::msleep(100);
    mouse_move_click(g_screen.width()/2, g_screen.height() / 2);


//    mouse_move_click(g_screen.width()/2, g_screen.height()/2);
    return true;
}

void RestorGame::reset()
{
}
