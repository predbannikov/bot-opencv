#include "tests.h"


Tests::Tests(CaptureWindow *aCapture, SocketIO *aSock) : BaseAction(aCapture, aSock)
{
    m_sActionName = "TESTS";
}

void Tests::init(QStringList &asListParam)
{
    sys_debugLog = false;

}

bool Tests::logic(QStringList &asListParam)
{
    return true;
}

void Tests::printDebug()
{
    qDebug() << "DEBUG: " << m_sActionName << list;
    QString sOut = QString("panel -> name=%1 active=%2 \n\tsrchName=%3 resLogic=%4")
            .arg("pan->sBodyName")
            .arg("pan->activeBody")
            .arg(sName)
            .arg(sys_resulLogic);
    qDebug() << qPrintable(sOut);
    qDebug() << "--------------------------------------------------";

}

void Tests::reset()
{
}
