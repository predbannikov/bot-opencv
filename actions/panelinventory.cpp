#include "panelinventory.h"

PanelInventory::PanelInventory(CaptureWindow *aCapture, SocketIO *aSock) : BaseAction(aCapture, aSock)
{
    m_sActionName = "PANELINVENTORY";
}

void PanelInventory::init(QStringList &asListParam)
{
    sys_debugLog = false;
    waitEnable = 500;
    confirmTime = 500;
    trigger = false;
    timer.restart();
    confirmTimer.restart();
    trans = TRANS_ENABLE;
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
}

bool PanelInventory::logic(QStringList &asListParam)
{
    bool enable = static_cast<bool>(asListParam[2].toUInt());
    capture->getMatOfRectName(m_src, "rect_inventoryItems");
    m_rectSrc = capture->getRect("rect_inventoryItems");
    cv::Mat mat;
    cv::Mat mat2;
    cv::Point _point;

    switch(trans){
    case TRANS_ENABLE:
        m_src(cv::Rect(45, 8, 150, 30)).copyTo(mat);        // Заголовок инвентарь
        if(capture->containTextMat(mat, _point, "инвентарь", "ru") == enable) {
            if(confirmTimer.elapsed() > confirmTime) {
                qDebug() << "Панель инвентаря включена:";
                confirmTimer.restart();
                trans = TRANS_SRCHITEM;
            }
        } else {
            confirmTimer.restart();
            if(!trigger) {
                timer.restart();
                push_key("i");
                trigger = true;
            } else {
                if(timer.elapsed() > waitEnable) {
                    trigger = false;
                }
            }
        }
        break;
    case TRANS_SRCHITEM:
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
            rectTooltip1Head.y = 35;
            rectTooltip1Head.width = 285;
            rectTooltip1Head.height = 25;
            cv::Mat matTooltip1Head;
            matTooltipField1(rectTooltip1Head).copyTo(matTooltip1Head);
            cv::imshow("win3", matTooltip1Head);
            inventory.map.insert(inventory.index, capture->getTextMat(matTooltip1Head, _point, "ru"));
//            qDebug() << capture->getTextMat(matTooltip1Head, _point, "ru");
            trans_srchitem = TRANS_SRCHITEM_READNAME_1;
            if(inventory.index == 25) {
                trans = TRANS_ACTION;
            } else {
                nextItem();
            }
            break;
        }




//        return  true;
        cv::imshow("win4", mat2);
//        QThread::msleep(50);
        break;

    case TRANS_ACTION:
        qDebug() << inventory.map;
        return true;
        break;
    }

    cv::imshow("win2", mat);
    m_timeUpdate.restart();
    return false;

}

void PanelInventory::nextItem()
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

cv::Point PanelInventory::getPointItem()
{
    inventory.x = m_rectSrc.x + m_rectItems.x + (inventory.col * inventory.length) + (inventory.length / 2);
    inventory.y = m_rectSrc.y + m_rectItems.y + (inventory.row * inventory.length) + (inventory.length / 2);
    return cv::Point(inventory.x, inventory.y);
}


void PanelInventory::printDebug()
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

void PanelInventory::reset()
{
}
