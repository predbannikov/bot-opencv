#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QQueue>
#include "enginescript.h"
#include "global.h"

namespace Ui {
class ControlPanel;
}

class ControlPanel : public QMainWindow
{
    Q_OBJECT
    EngineScript *m_engine;
    QQueue<QStringList> queue;
    QQueue<QStringList> testQueue;
    QString     m_sSystemTarget;
    QString     m_sStationTarget;
    QJsonObject m_jObject;
//    QStringList slistSystems;
    bool started = false;
    QMultiMap<QString, QString>   mMapSystems;
public:
    explicit ControlPanel(EngineScript *aEngine, QWidget *parent = nullptr);
    ~ControlPanel();



    void test();
    void manufacturing();
    void fishing();
    void init();
    void performList();
    void prepScript();
public slots:
    void slotReceivReturnCommand(QStringList aList);
private slots:


    void on_checkBox_2_clicked();

    void on_radioButton_3_clicked();

    void on_pushButton_clicked();











    void on_radioButton_5_clicked();

    void on_radioButton_6_clicked();

private:
    Ui::ControlPanel *ui;

signals:
    void signalSetSide(int aSide);
    void signalSetQueue(QQueue<QStringList> aQueue);
    void signalFreeze();
    void signalResizeImage();
    void signalEngineEnable(bool aState);
    void signalEngineSetCurStation(QString asStation);
    void signalEngineEnableTest();
    void closeWindow();

};

#endif // CONTROLPANEL_H
