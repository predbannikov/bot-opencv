#include "manufacturing.h"

Manufacturing::Manufacturing(CaptureWindow *aCapture, SocketIO *aSock) : BaseAction(aCapture, aSock)
{
    m_sActionName = "MANUFACTURING";
}
#define COUNT_CELL_FOR_FIND_ITEM_IN_STOREHOSE  5

#define INDEX_IN_STOREHOME_ITEM   86

#define COUNT_GET_ITEMS     1300         // Количество итемов сколько брать в рюкзак на обработку

void Manufacturing::init(QStringList &asListParam)
{
    sys_debugLog = false;
    waitEnable = 10000;
    confirmTime = 500;
    trigger = false;
    trigger2 = false;
    timer.restart();
    confirmTimer.restart();
//    trans = TRANS_ENABLE_DIALOG;
    trans = TRANS_ENABLE_STOREHOUSE;
    trans_srchitem = TRANS_SRCHITEM_READNAME_1;
    inventory.max_col = 8;
    inventory.max_row = 8;
    inventory.col = 0;
    inventory.row = 0;
    inventory.index = 0;
    inventory.name = "";
    inventory.count = 0;
    inventory.length = 54;
    inventory.rect = cv::Rect(0, 0, inventory.length, inventory.length);

    sotreHouse.max_col = 9;
    sotreHouse.max_row = 10;
    sotreHouse.col = 0;
    sotreHouse.row = 0;
    setStoreHouse(INDEX_IN_STOREHOME_ITEM);
//    sotreHouse.index = 72;
    sotreHouse.name = "";
    sotreHouse.count = 0;
    sotreHouse.length = 51;
    sotreHouse.rect = cv::Rect(0, 0, sotreHouse.length, sotreHouse.length);
//    const QStringList constList = {"меди", "слиток", "руда" };
    const QStringList constList = {"фанера", "доска" };
    listForRemove = constList;
//    strForAdding = "железная";
    strForAdding = "доска";
//    strForAdding = "свинцов";


}

bool Manufacturing::logic(QStringList &asListParam)
{
    bool enable = static_cast<bool>(asListParam[2].toUInt());
    cv::Mat mat;
    cv::Mat mat2;
    cv::Point _point;
    QString sText;
    m_rectSrc = capture->getRect("rect_inventoryItems");
//    m_rectSrc.y -= 16;
    m_rectSrc.y -= 16;
    capture->getMatOfRect(m_src, m_rectSrc);

    switch(trans){
    case TRANS_ENABLE_DIALOG: {
        capture->getMatOfRectName(mat, "rect_storehouse");
        if(capture->containTextMat(mat, _point, "склад", "ru") == enable) {
            if(confirmTimer.elapsed() > confirmTime) { // ждать для подтверждения
                qDebug() << "Вызвали складовщика:";
                confirmTimer.restart();
                trigger = false;
                trans = TRANS_ENABLE_STOREHOUSE;
            }
        } else {
            confirmTimer.restart();
            if(!trigger) {
                timer.restart();
                push_key("r");
                trigger = true;
            } else {
                if(timer.elapsed() > waitEnable) {
                    trigger = false;
                }
            }
        }
        break;
    }
    case TRANS_ENABLE_STOREHOUSE: {
        if(!trigger2) {
            QThread::msleep(500);
//            push_key("2");              //
            push_key("r");
            trigger2 = true;
        } else {
            m_src(cv::Rect(45, 8, 150, 30)).copyTo(mat);        // Заголовок инвентарь
            if(capture->containTextMat(mat, _point, "инвентарь", "ru") == enable) {
                if(confirmTimer.elapsed() > confirmTime) {
                    qDebug() << "Склад включен:";
                    confirmTimer.restart();
                    trans = TRANS_SRCHITEM;
                }
            } else {
                confirmTimer.restart();
                if(!trigger) {
                    timer.restart();
//                    push_key("i");
                    trigger = true;
                } else {
                    if(timer.elapsed() > waitEnable) {
                        trigger = false;
                    }
                }
            }
        }
        break;
    }
    case TRANS_SRCHITEM: {

//        qDebug() << "hit the target";
//        return true;
        m_rectItems = cv::Rect(1, 143, inventory.length * inventory.max_col, inventory.length * inventory.max_row);      // Инвентарь
        m_src(m_rectItems).copyTo(mat);
        mat(inventory.rect).copyTo(mat2);

        switch (trans_srchitem) {
        case TRANS_SRCHITEM_READNAME_1:
            mouse_move(getPointItem());
            trans_srchitem = TRANS_SRCHITEM_READNAME_2;
            break;
        case TRANS_SRCHITEM_READNAME_2: {
            qDebug() << "m_timeUpdate =" << m_timeUpdate.elapsed();
            if(m_timeUpdate.elapsed() < 90)
                return false;
            cv::Rect rectTooltipField1 = m_rectSrc;
            rectTooltipField1.x -= 315;
            rectTooltipField1.y = m_rectSrc.y;
            rectTooltipField1.width = 315;
            rectTooltipField1.height = g_screen.height() - m_rectSrc.y;
            cv::Mat matTooltipField1;
            capture->getMatOfRect(matTooltipField1, rectTooltipField1);
            cv::Rect rectTooltip1Head = rectTooltipField1;
            rectTooltip1Head.x = 10;
            rectTooltip1Head.y = 37;
            rectTooltip1Head.width = 285;
            rectTooltip1Head.height = 25;
            cv::Mat matTooltip1Head;
            matTooltipField1(rectTooltip1Head).copyTo(matTooltip1Head);
            cv::imshow("win3", matTooltip1Head);
            sText = capture->getTextMat(matTooltip1Head, _point, "ru");
            for(int i = 0; i < listForRemove.size(); i++) {
                if(sText.contains(listForRemove[i])) {
                    mouse_click(1);
                    QThread::msleep(300);
                    push_key(" ");
                    QThread::msleep(300);
                }
            }

//            inventory.map.insert(inventory.index, sText);
//            qDebug() << capture->getTextMat(matTooltip1Head, _point, "ru");
            trans_srchitem = TRANS_SRCHITEM_READNAME_1;
            if(inventory.index == 3) {
                mouse_move(1, 1);
                trans = TRANS_ADDITEM;
                setItem(0);
//                inventory.index = 0;
            } else {
                nextItem();
            }
            break;
        }

        }

        cv::imshow("win4", mat);
        cv::imshow("win4", mat2);
        break;
    }
    case TRANS_ADDITEM: {
        m_rectItems = cv::Rect(960, 257, sotreHouse.length * sotreHouse.max_col, sotreHouse.length * sotreHouse.max_row);      // Склад
        capture->getMatOfRect(mat, m_rectItems);
        cv::imshow("win4", mat);

//        return false;
        mat(sotreHouse.rect).copyTo(mat2);

        switch (trans_srchitem) {
        case TRANS_SRCHITEM_READNAME_1:
            sotreHouse.x = m_rectItems.x + (sotreHouse.col * sotreHouse.length) + (sotreHouse.length / 2);
            sotreHouse.y = m_rectItems.y + (sotreHouse.row * sotreHouse.length) + (sotreHouse.length / 2);
            mouse_move(sotreHouse.x, sotreHouse.y);
            trans_srchitem = TRANS_SRCHITEM_READNAME_2;
            QThread::msleep(100);
            break;
        case TRANS_SRCHITEM_READNAME_2: {
            qDebug() << "m_timeUpdate =" << m_timeUpdate.elapsed();
            if(m_timeUpdate.elapsed() < 90)
                return false;
            cv::Rect rectTooltipField1 = m_rectItems;
            rectTooltipField1.x -= 315;
//            rectTooltipField1.y = m_rectSrc.y;
            rectTooltipField1.width = 315;
            rectTooltipField1.height = g_screen.height() - m_rectItems.y;
            cv::Mat matTooltipField1;
            capture->getMatOfRect(matTooltipField1, rectTooltipField1);
            cv::Rect rectTooltip1Head; // = rectTooltipField1;
            rectTooltip1Head.x = 10;
            rectTooltip1Head.y = 37;
            rectTooltip1Head.width = 285;
            rectTooltip1Head.height = 25;
            cv::Mat matTooltip1Head;
            matTooltipField1(rectTooltip1Head).copyTo(matTooltip1Head);
            cv::imshow("win3", matTooltip1Head);
            sText = capture->getTextMat(matTooltip1Head, _point, "ru");
            if(sText.contains(strForAdding)) {
                mouse_click(1);
                QThread::msleep(500);
                typingText(QString::number(COUNT_GET_ITEMS));                                                                 // количество

                QThread::msleep(300);
                push_key(" ");
                trans = TRANS_EXIT_HOUSESTORE;
                QThread::msleep(1000);
            }
//            sotreHouse.map.insert(sotreHouse.index, sText);
//            qDebug() << capture->getTextMat(matTooltip1Head, _point, "ru");
            trans_srchitem = TRANS_SRCHITEM_READNAME_1;
            if(sotreHouse.index == (INDEX_IN_STOREHOME_ITEM + COUNT_CELL_FOR_FIND_ITEM_IN_STOREHOSE) ) {
                trans = TRANS_END;
            } else {
                sotreHouse.index++;
                if(sotreHouse.index >= 99) {
                    sotreHouse.index = 0;
                }
                sotreHouse.col = sotreHouse.index % sotreHouse.max_col;
                sotreHouse.row = sotreHouse.index / sotreHouse.max_row;
                sotreHouse.rect = cv::Rect(sotreHouse.col * sotreHouse.length
                                          , sotreHouse.row * sotreHouse.length
                                          , sotreHouse.length, sotreHouse.length);
            }
            break;
        }

        }

        cv::imshow("win4", mat);
        cv::imshow("win5", mat2);
//        return true;
        break;
    }
    case TRANS_EXIT_HOUSESTORE:
//        push_f_key("ESC");
//        QThread::msleep(500);
        push_f_key("ESC");
        QThread::msleep(1000);
        push_f_key("l");
        QThread::msleep(1000);
        trans = TRANS_ADD_TO_CRAFT;

        break;
    case TRANS_ADD_TO_CRAFT: {
        m_rectItems = cv::Rect(1, 143, inventory.length * inventory.max_col, inventory.length * inventory.max_row);      // Инвентарь
        m_src(m_rectItems).copyTo(mat);
        mat(inventory.rect).copyTo(mat2);

        switch (trans_srchitem) {
        case TRANS_SRCHITEM_READNAME_1:
            mouse_move(getPointItem());
            trans_srchitem = TRANS_SRCHITEM_READNAME_2;
            break;
        case TRANS_SRCHITEM_READNAME_2:
            qDebug() << "m_timeUpdate =" << m_timeUpdate.elapsed();
            if(m_timeUpdate.elapsed() < 90)
                return false;
            cv::Rect rectTooltipField1 = m_rectSrc;
            rectTooltipField1.x -= 315;
            rectTooltipField1.y = m_rectSrc.y;
            rectTooltipField1.width = 315;
            rectTooltipField1.height = g_screen.height() - m_rectSrc.y;
            cv::Mat matTooltipField1;
            capture->getMatOfRect(matTooltipField1, rectTooltipField1);
            cv::Rect rectTooltip1Head = rectTooltipField1;
            rectTooltip1Head.x = 10;
            rectTooltip1Head.y = 53;
            rectTooltip1Head.width = 285;
            rectTooltip1Head.height = 26;
            cv::Mat matTooltip1Head;
            matTooltipField1(rectTooltip1Head).copyTo(matTooltip1Head);
            cv::imshow("win3", matTooltip1Head);
            sText = capture->getTextMat(matTooltip1Head, _point, "ru");
            if(comparisonStr(strForAdding, sText) <= 2 || sText.contains(sText)) {
                mouse_click(1);
                QThread::msleep(1000);
                QPoint pointMassCraft = capture->getPoint("rect_pushMassCraft");
                mouse_move_click(pointMassCraft.x(), pointMassCraft.y());
                QThread::msleep(1000);
                trans = ACTION_WAIT_CRAFT;
            }
//            inventory.map.insert(inventory.index, sText);
//            qDebug() << capture->getTextMat(matTooltip1Head, _point, "ru");
            trans_srchitem = TRANS_SRCHITEM_READNAME_1;
            if(inventory.index == 2) {
                mouse_move(1, 1);
                trans = TRANS_END;
            } else {
                nextItem();
            }
            break;
        }

        cv::imshow("win4", mat);
        cv::imshow("win4", mat2);
        break;
    }
    case ACTION_WAIT_CRAFT:
        m_rectSrc = capture->getRect("rect_inventoryItems");
//        m_rectSrc.y -= 16;
        capture->getMatOfRect(m_src, m_rectSrc);
        m_src(cv::Rect(45, 8, 150, 30)).copyTo(mat);        // Заголовок инвентарь
        if(capture->containTextMat(mat, _point, "инвентарь", "ru") ) {
            qDebug() << "end craft";
            QThread::msleep(1000);
            push_f_key("ESC");
            QThread::msleep(1000);

            return true;
        }
        cv::imshow("win4", mat);
//        trans = TRANS_END;
        break;
    case TRANS_END:
        qDebug() << "end";
        return true;
    }

    cv::imshow("win2", m_src);
    m_timeUpdate.restart();
    return false;

}

void Manufacturing::nextItem()
{
    inventory.index++;
    if(inventory.index >= 64) {
        inventory.index = 0;
    }
    inventory.col = inventory.index % inventory.max_col;
    inventory.row = inventory.index / inventory.max_row;
    inventory.rect = cv::Rect(inventory.col * inventory.length
                              , inventory.row * inventory.length
                              , inventory.length, inventory.length);

    qDebug() << "inventory.x =" << inventory.x << " inventory.y =" << inventory.y << " inventory.index =" << inventory.index;
}

void Manufacturing::setItem(int aIndex)
{
    inventory.index = 0;

    inventory.col = inventory.index % inventory.max_col;
    inventory.row = inventory.index / inventory.max_row;
    inventory.rect = cv::Rect(inventory.col * inventory.length
                              , inventory.row * inventory.length
                              , inventory.length, inventory.length);

//    qDebug() << "inventory.x =" << inventory.x << " inventory.y =" << inventory.y << " inventory.index =" << inventory.index;

}

void Manufacturing::setStoreHouse(int aIndex)
{
    sotreHouse.index = aIndex;
    sotreHouse.col = sotreHouse.index % sotreHouse.max_col;
    sotreHouse.row = sotreHouse.index / sotreHouse.max_row ;
    sotreHouse.rect = cv::Rect(sotreHouse.col * sotreHouse.length
                              , sotreHouse.row * sotreHouse.length
                              , sotreHouse.length, sotreHouse.length);

}

cv::Point Manufacturing::getPointItem()
{
    inventory.x = m_rectSrc.x + m_rectItems.x + (inventory.col * inventory.length) + (inventory.length / 2) + 3;
    inventory.y = m_rectSrc.y + m_rectItems.y + (inventory.row * inventory.length) + (inventory.length / 2);
    return cv::Point(inventory.x, inventory.y);
}


void Manufacturing::printDebug()
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

void Manufacturing::reset()
{
}
