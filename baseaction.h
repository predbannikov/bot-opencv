#ifndef BASEACTION_H
#define BASEACTION_H
#include <QElapsedTimer>
#include "socketio.h"
#include "global.h"
#include "capturewindow.h"
class BaseAction
{
    enum TRANSITION {TRANSITION_1, TRANSITION_2 } transition;
    SocketIO        *m_sock;

public:
    BaseAction(CaptureWindow *aCapture, SocketIO *aSock);
    virtual ~BaseAction();

    virtual void init(QStringList &asListParam) = 0;
    virtual bool logic(QStringList &asListParam) = 0;
    virtual void reset() = 0;
    virtual void printDebug();

    void setMinScalar(cv::Scalar acvMinScalar);
    void setMaxScalar(cv::Scalar acvMaxScalar);


    bool expectedImage(QString asStr, double aDiffCoeff, int anCount, int anStart, int anEnd);
//    bool expectedImageClose(QString sStr);
    bool srchAreaOnceInPart();
    bool perform(QStringList &asListParam);
    void resetBase();
    void push_f_key(QString aStr);
    void push_key(QString aChar);
    void press_key(QString aChar);
    void release_key(QString aChar);
    void typingText(QString asText);
    void mouse_click(int anSideType = 0);   // 0 - left button; 1 - right button
    void mouse_move(int x, int y);
    void mouse_move(cv::Point aPoint);
    void mouse_wheel(int anCount);
    void mouse_move_rel(int x, int y);
    void mouse_move_click(int x, int y);
    void send_test();

    QString         m_sActionName;
    QString getName();


    CaptureWindow   *capture;

    QElapsedTimer   sys_timeElapsed;
    QElapsedTimer   sys_elapsedFunc;
    int             sys_timeWaitMSec;
    bool            sys_resulLogic;
    bool            sys_debugLog;
    QStringList     sys_listCommand;

    cv::Mat         m_src;
    cv::Rect        m_rect;
    cv::Point       m_point;
    bool            m_bCompass;
    bool            target;
    bool            trigger;
    QPoint          point;
    QString         sPanHeadName;
    QString         sSearchImage;
    QString         sName;
    QString         sTypingText;
    QStringList     list;
    QStringList     listForSearch;
    QElapsedTimer   timer;
    QElapsedTimer   confirmTimer;
    QElapsedTimer   m_timeUpdate;
    double          diffCoef;
    double          coeff;
    double          radius;
    double          distance;
    ulong           waitEnable;
    ulong           waitMSec;
    ulong           confirmTime;
    ulong           whileWaitMSec;
    int             nCount;
    int             iStart;
    int             iEnd;
    qint64          lastElapse;

protected:
    cv::Scalar  minScalar;
    cv::Scalar  maxScalar;

};

#endif // BASEACTION_H
