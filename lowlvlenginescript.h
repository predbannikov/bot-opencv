#ifndef LOWLVLENGINESCRIPT_H
#define LOWLVLENGINESCRIPT_H

#include <QString>
#include <QDebug>
#include <QElapsedTimer>
#include <QTcpSocket>
#include <QCoreApplication>
#include "capturewindow.h"
#include "global.h"
#include "socketio.h"
#include "actions/actiondebug.h"
#include "actions/actionwait.h"
#include "actions/restorgame.h"
#include "actions/imageexpectedclose.h"
#include "actions/imageexpected.h"
#include "actions/sendeventcontrol.h"
#include "actions/marker.h"
#include "actions/getstrstaticfield.h"
#include "actions/panelinventory.h"
#include "actions/manufacturing.h"
#include "actions/fishing.h"
#include "actions/tests.h"




class LowLvlEngineScript
{
    QStringList     command;


    QVector<BaseAction *> actions;
    SocketIO            *m_sock;
public:
    virtual ~LowLvlEngineScript();
    LowLvlEngineScript();
    void update();
    bool cycle = true;
    virtual void    init();
    virtual void    readCommand(QStringList &aslistCommand) {}
    CaptureWindow   *capture;
    IOData          *m_pIOData;
    std::map<std::string, ImageROI> *m_pDataSet;

protected:
    BaseAction          *parseAction();


    cv::Scalar  minScalar;
    cv::Scalar  maxScalar;
    int         minNumber;
    int         midNumber;
    int         maxNumber;

    QString m_sRetFunction;

private:



};

#endif // LOWLVLENGINESCRIPT_H
