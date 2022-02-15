#include "controlpanel.h"
#include "ui_controlpanel.h"

ControlPanel::ControlPanel(EngineScript *aEngine, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControlPanel)
{
    ui->setupUi(this);
    m_engine = aEngine;
    connect(this, &ControlPanel::signalEngineEnable, m_engine, &EngineScript::slotEngineEnable, Qt::QueuedConnection);
    connect(this, &ControlPanel::signalEngineSetCurStation, m_engine, &EngineScript::slotSetCurStation, Qt::QueuedConnection);
    connect(this, &ControlPanel::signalEngineEnableTest, m_engine, &EngineScript::slotStartTest, Qt::QueuedConnection);
    connect(this, &ControlPanel::signalFreeze, m_engine, &EngineScript::slotFreeze, Qt::QueuedConnection);
    connect(this, &ControlPanel::signalResizeImage, m_engine, &EngineScript::slotResizeImage, Qt::QueuedConnection);
    connect(this, &ControlPanel::signalSetQueue, m_engine, &EngineScript::slotSetQueue, Qt::QueuedConnection);
    connect(this, &ControlPanel::signalSetSide, m_engine, &EngineScript::slotSetSide, Qt::QueuedConnection);
    connect(m_engine, &EngineScript::signalReturnCommand, this, &ControlPanel::slotReceivReturnCommand, Qt::QueuedConnection);


    init();

}

ControlPanel::~ControlPanel()
{
    delete ui;
}


void ControlPanel::test()
{
    queue.clear();
    queue.enqueue(QStringList {"MAPSYSTEMENABLE",           "0", "," });

    //    queue.enqueue(QStringList {"MARKER",                    "0", "test"});
}

void ControlPanel::manufacturing()
{
    qDebug() << "manufacturing";
    queue.enqueue(QStringList {"MANUFACTURING",             "0", "1" });
    queue.enqueue(QStringList {"MARKER",                    "0", "manufacturing"});

}

void ControlPanel::fishing()
{
    qDebug() << "fishing";
    queue.enqueue(QStringList {"FISHING",                   "0", "1" });
    queue.enqueue(QStringList {"MARKER",                    "0", "fishing"});
}



void ControlPanel::init()
{


}

void ControlPanel::prepScript()
{
    if(ui->checkBox_2->isChecked()) {
        if(ui->radioButton_6->isChecked()) {
            manufacturing();
        } else if(ui->radioButton->isChecked()) {
        } else if(ui->radioButton_4->isChecked()) {
        } else if(ui->radioButton_5->isChecked()) {
            fishing();
        } else if(ui->radioButton_3->isChecked()) {
        }
    }
}


void ControlPanel::slotReceivReturnCommand(QStringList aList)
{
    if(!aList.isEmpty()) {
        if(ui->checkBox_3->checkState() && ui->checkBox_2->checkState()) {
            if(aList[1].toInt() == 1) {



                qDebug() << "";
                qDebug() << "Control panel success";
                qDebug() << "send first part queue";
                qDebug() << "";


            } else {
                qDebug() << "Control panel not recognize return value";
            }
        } else {

            queue.clear();
//            qDebug() << "receiv return command " << aList;
            if(aList[1] == "1") {                                                   // Не выполнилась комманда
                if(aList[0] == "AIMPFLYAROUND") {
                    if(aList[2] == "compass not found") {
                        qDebug() << "Пропал компас - прилетели на пункт назначения / помехи в данных";
                        qDebug() << "Текущее решение, устанавливаем новый пункт назначения";
//                        checkCurSystem();
                        emit signalSetQueue(queue);
                    } else if(aList[2] == "") {
                        qDebug() << "Не обработыннй выход из процедуры" << aList[0];
//                        checkCurSystem();
                    }
                } else if(aList[0] == "IMAGEEXPECTEDCLOSE") {
                    if(aList[2] == "warning triangle") {
                        qDebug() << "Вышли из режима автопилота, появилось предупреждение";
//                        flyAround();
                        emit signalSetQueue(queue);
                    } else if(aList[2] == "detected intercept") {                                                                           // Доделать обнаруженный перехват
                        qDebug() << "Обнаружен перехват";
//                        transitToHyper();
                        emit signalSetQueue(queue);
                    }
                } else if(aList[0] == "MAPSYSTEM") {
                    if(aList[2] == "no cursor set system") {
                        qDebug() << "Скрипт установки курсора сброшен, повторяем попытку";
//                        setCursorSystem();
                        emit signalSetQueue(queue);
                    }
                } else {
                    qDebug() << "slotReceivReturnCommand" << " if(aList[1] == 1) не обработанные состояния";
                }
            } else if(aList[1] == "0") {                                            // Выполнилась комманда
                if(aList[0] == "MARKER") {
                    if(aList[2] == "setCursorSystem") {
//                        dockAutoStart();
                    } else if(aList[2] == "manufacturing") {
                        prepScript();
//                        manufacturing();
                    } else if(aList[2] == "fishing") {
                        prepScript();
//                        fishing();
                    }
                    emit signalSetQueue(queue);
                } else if(aList[0] == "TESTS") {
                    if(aList[2] == "test") {
                        if(ui->checkBox_3->isChecked()) {
                            queue = testQueue;
                            emit signalSetQueue(queue);
                        }
                    }
                    // ничего не делаем не засоряем stdout
                } else {
                    qDebug() << "slotReceivReturnCommand" << " if(aList[0] == MARKER) { не обработанные состояния";
                }

            } else if(aList[1] == "2") {                                            // Возврат результата
                if(aList[0] == "MARKER") {
                    if(aList[2] == "checkCurSystem") {

                        int ret = comparisonStr(m_sSystemTarget, aList[3]);
                        if(ret <= 2) {
                            qDebug() << "Проверка на место прибытия успешна";
//                            QMultiMap<QString, QString>::iterator it = mMapSystems.find(m_sSystemTarget);
//                            while(it != mMapSystems.end() && it.key() == m_sSystemTarget) {
//                                m_sStationTarget = it.value();
//                                ++it;
//                            }
//                            setCursorStation();
                            emit signalSetQueue(queue);
                        } else {
                            qDebug() << "Не обработанное исключение, компаса нет, но в систему не прибыли" ;
                        }

                    }
                }
            } else {
                qDebug() << "slotReceivReturnCommand" << " } else if(aList[1] == 0) { не обработанные состояния";
            }


        }
    }

}


void ControlPanel::on_checkBox_2_clicked()          // Пуск
{

    QThread::msleep(1000);
    started = false;
    prepScript();
    queue.prepend(QStringList {"RESTORGAME",                "0" });

    emit signalSetQueue(queue);
}


void ControlPanel::on_radioButton_3_clicked()
{
    emit signalSetSide(0);
}

void ControlPanel::on_pushButton_clicked()          //  ТЕСТ
{
    queue.clear();


//    queue.enqueue(QStringList {"PANELINVENTORY",          "0", "1" });
//    queue.enqueue(QStringList {"MANUFACTURING",          "0", "1" });
    queue.enqueue(QStringList {"FISHING",                   "0", "1" });




//    queue.enqueue(QStringList {"SENDEVENTCONTROL",          "0", "push_f_key", "CTRL" });
//    queue.enqueue(QStringList {"SENDEVENTCONTROL",          "0", "press_key", "w" });
//    queue.enqueue(QStringList {"SENDEVENTCONTROL",          "0", "release_key", "w" });







//    queue.append(QStringList {"TESTS",                      "0", "test" });
    testQueue.clear();
    testQueue = queue;
    queue.prepend(QStringList {"RESTORGAME",                "0" });
    emit signalSetQueue(queue);
}


void ControlPanel::on_radioButton_5_clicked()
{

}


void ControlPanel::on_radioButton_6_clicked()
{

}

