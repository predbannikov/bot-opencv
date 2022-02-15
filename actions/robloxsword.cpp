#include "robloxsword.h"


RobloxSword::RobloxSword(CaptureWindow *aCapture, SocketIO *aSock) : BaseAction(aCapture, aSock)
{
    m_sActionName = "ROBLOXSWORD";
}

void RobloxSword::init(QStringList &asListParam)
{
//    cv::Rect rectCellsTrofej = cv::Rect()

    timer.restart();
    trans = TRANS_1;
    trans_searchItem = TRANS_SRCHITEM_1;
    colKey = 0;
    trofejCells.col = 6;
    trofejCells.row = 2;
    trofejCells.cels = 12;
    trofejCells.currentItem = 0;
    trofejCells.widthCell = capture->getRect("rect_trofejFichingItems").width / trofejCells.col;
    trofejCells.heightCell = capture->getRect("rect_trofejFichingItems").height / trofejCells.row;
    trofejCells.pointStart = cv::Point(capture->getRect("rect_trofejFichingItems").x + trofejCells.widthCell / 2,
                                  capture->getRect("rect_trofejFichingItems").y + trofejCells.heightCell / 2);
    trofejCells.pointCursor = trofejCells.pointStart;


    const QStringList constlistForRemove = {"голец"};
    listForRemove = constlistForRemove;
#define GET_ALL_ITEMS 1
}

bool RobloxSword::logic(QStringList &asListParam)
{
//    cv::Mat mat;
//    cv::Mat mat2;
//    cv::Point _point;

//    QThread::msleep(500);


//    if(!findRectRecognize())
//        return false;

//    if(recognizeKey(1))
//        return true;
//    else
//        return false;



    switch (trans) {
    case TRANS_1:
        if(waitSpace())
            trans = TRANS_2;
        break;
    case TRANS_2:
        push_key(" ");
        QThread::msleep(1470);
        push_key(" ");
        trans = TRANS_3;
        qDebug() << "start recognize 1";
        break;
    case TRANS_3: {
        int _ret = findRectRecognize();
        if(_ret == 1) {
            trans = TRANS_4;
            qDebug() << "start recognize 2";
        } else if(_ret == 2) {
            QThread::msleep(5500);
            trans = TRANS_5;
            qDebug() << "end RobloxSword";
        }
        break;
    }
    case TRANS_4:
        if(recognizeKey(1)) {
            trans = TRANS_5;
            QThread::msleep(4000);
//            push_f_key("CTRL");
            qDebug() << "stop";
        }
        break;
    case TRANS_5: {
        qDebug() << "TRANS_5";

        if(GET_ALL_ITEMS) {
            trans = TRANS_6;
            push_key("r");
            return false;
        }


        cv::Rect rect_trofejCells = capture->getRect("rect_trofejFichingItems");


        switch (trans_searchItem) {
        case TRANS_SRCHITEM_1:
            mouse_move(trofejCells.pointCursor);
            trans_searchItem = TRANS_SRCHITEM_2;
            break;
        case TRANS_SRCHITEM_2: {
            qDebug() << "m_timeUpdate =" << m_timeUpdate.elapsed();
            if(m_timeUpdate.elapsed() < 90)
                return false;
            cv::Rect rectTooltipField1 = rect_trofejCells;
            rectTooltipField1.x -= 315;
            //                rectTooltipField1.y = m_rectSrc.y;
            rectTooltipField1.width = 315;
            rectTooltipField1.height = g_screen.height() - rectTooltipField1.y;
            cv::Mat matTooltipField1;
            capture->getMatOfRect(matTooltipField1, rectTooltipField1);
            cv::Rect rectTooltip1Head = rectTooltipField1;
            rectTooltip1Head.x = 10;
            rectTooltip1Head.y = 37;
            rectTooltip1Head.width = 285;
            rectTooltip1Head.height = 25;
            cv::Mat matTooltip1Head;
            matTooltipField1(rectTooltip1Head).copyTo(matTooltip1Head);

            cv::imshow("win3", matTooltipField1);
            cv::imshow("win4", matTooltip1Head);
            cv::Point srchPointText;
            QString sText = capture->getTextMat(matTooltip1Head, srchPointText, "ru");

            trans_searchItem = TRANS_SRCHITEM_1;
            for(int i = 0; i < listForRemove.size(); i++) {
                if(sText.contains(listForRemove[i])) {
                    qDebug() << "Есть в списке, пропускаем";
                    if(trofejCells.currentItem == 2) {
                        setCell(0);
                        trans = TRANS_6;
                    } else {
                        nextCell();
                    }
                    QThread::msleep(300);
                    return false;
                    //                        push_key(" ");
                    //                        QThread::msleep(300);
                }
            }
            QThread::msleep(500);
            mouse_click(1);
            //                trans_searchItem = TRANS_SRCHITEM_1;

            //            inventory.map.insert(inventory.index, sText);
            //            qDebug() << capture->getTextMat(matTooltip1Head, _point, "ru");
            if(trofejCells.currentItem == 2) {
                setCell(0);
                trans = TRANS_6;
            } else {
                nextCell();
            }
            QThread::msleep(300);
            break;
        }

        }

        break;
    }
    case TRANS_6:
        qDebug() << "TRANS_6";
        push_key(" ");
        QThread::msleep(6000);
        return true;

        break;
    }



//    cv::imshow("win2", m_src);
    return false;

}


bool RobloxSword::waitSpace()
{
    QString sText;
    m_rectSrc = capture->getRect("rect_pressSpace");
    m_rectSrc.x -= 10;
    m_rectSrc.width += 20;
    capture->getMatOfRect(m_src, m_rectSrc);
    cv::Mat dst = m_src;
    cv::Mat mask;
    cv::Mat edgeMat;
    cv::Canny(dst, edgeMat, 10, 150);
    std::vector< std::vector< cv::Point> > contours;
    cv::findContours(edgeMat, contours, cv::noArray(), cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    cv::Rect foundRect;
    for(size_t i = 0; i < contours.size(); i++) {


        double area = cv::contourArea(contours[i]);
        if(area > 2900 && area < 3400) {
            foundRect = cv::boundingRect(contours[i]);

            qDebug() << "areas " << i << " =" << area;
            break;
        }

    }
    if(foundRect.x == 0)
        return false;
    cv::Mat recognizMat;
    dst(foundRect).copyTo(recognizMat);
    cv::Mat gray;
    cv::Mat recognizMask;
    cv::cvtColor(recognizMat, gray, cv::COLOR_BGR2GRAY);
    int minVal1 = 35;
    bool ret = false;
    while(minVal1 < 256 && !ret) {
        cv::threshold(gray, mask, minVal1, 153, cv::THRESH_BINARY);
        cv::inRange(mask, cv::Scalar(0, 0, 0), cv::Scalar(1, 1, 1), recognizMask);
        int black, white;
        if(!capture->blackLessWhite(recognizMask, black, white)) {
            cv::bitwise_not(recognizMask, recognizMask);
        }

        cv::imshow("win2", edgeMat);
        cv::imshow("win3", mask);
        cv::imshow("win4", gray);
        cv::imshow("win5", recognizMask);
        QApplication::processEvents();


        sText = capture->recognizeText(recognizMask, "eng");
        sText = sText.simplified();
        sText = sText.toLower();
        if(sText.isEmpty())
            sText = "not recognized";
        qDebug() << "source text recognize =" << sText;
        if(sText.contains("space")) {
            ret = true;
            return true;
        }
        minVal1 += 55;
    }


    sText = sText.simplified();
    sText = sText.toLower();
    deleteCharExtra(sText);
//    qDebug() << sText;

    cv::imshow("win2", edgeMat);
    cv::imshow("win3", mask);
    cv::imshow("win4", gray);
    cv::imshow("win5", recognizMask);

    return false;








}

bool RobloxSword::recognizeKey(int aType)
{
//    m_rectSrc = capture->getRect("rect_recognKey");
//    m_rectSrc = cv::Rect(767, 360, 380, 22);
//    m_rectSrc.y -= 15;
//    capture->getMatOfRect(m_src, m_rectSrc);

    m_src = m_matForRecogniz;

    int colKeyWidth = m_src.size().width / 10;
//    int colKeyWidth = m_rectSrc.width / 10;
    cv::Rect colKeyRect = cv::Rect(colKey * colKeyWidth + 3, 0 + 2, colKeyWidth - 6,  m_rectSrc.height - 4);
    cv::Mat colKeyMat;
    m_src(colKeyRect).copyTo(colKeyMat);



    QString sText;

    cv::Mat gray;
    cv::Mat mask;
    cv::Mat recognizMask;
    cv::cvtColor(colKeyMat, gray, cv::COLOR_BGR2GRAY);
    int minVal1 = 15;
    bool ret = false;
    while(minVal1 < 256 && !ret) {
        cv::threshold(gray, mask, minVal1, minVal1 + 50, cv::THRESH_BINARY);
//        cv::threshold(gray, mask, minVal1, 153, cv::THRESH_BINARY);
        cv::inRange(mask, cv::Scalar(0, 0, 0), cv::Scalar(1, 1, 1), recognizMask);
        int black, white;
        if(!capture->blackLessWhite(recognizMask, black, white)) {
            cv::bitwise_not(recognizMask, recognizMask);
        }
//        cv::imshow("win2", m_src);
        cv::imshow("win3", mask);
        cv::imshow("win4", gray);
//        cv::imshow("win5", recognizMask);
        QApplication::processEvents();

        sText = capture->recognizeText(recognizMask, "eng");
        sText = sText.simplified();
        sText = sText.toLower();
        deleteCharExtra(sText);
//        if(sText.isEmpty()) {
//            sText = "not recognized";
//            return;
//        }
        qDebug() << "source text recognize =" << sText;
        if(sText == "5")
            sText = "s";
        if(sText.size() > 1 && sText[1] == "j")
            sText.remove(1, 1);
//        if(sText.contains("w") || sText.contains("a") || sText.contains("s") || sText.contains("d")) {
        if(sText == "w" || sText == "a" || sText == "s" || sText == "d") {
            if(sText.contains("w"))
                push_key("w");
            if(sText.contains("a"))
                push_key("a");
            if(sText.contains("s"))
                push_key("s");
            if(sText.contains("d"))
                push_key("d");
            ret = true;
            if(!aType) {
                return true;
            }
            colKey++;
            QThread::msleep(150);
            return false;
        }
        minVal1 += 15;
    }

    if(!aType)
        return false;
    else
        return true;



}

int RobloxSword::findRectRecognize()
{
    cv::Mat hsv;
    cv::Mat mask;
    cv::Mat recognizMask;
    m_rectSrc = cv::Rect(760, 330, 400, 125);
    cv::Mat keyRectMat;
    capture->getMatOfRect(m_src, m_rectSrc);
    cv::cvtColor(m_src, hsv, cv::COLOR_BGR2HSV);
//    int minVal1 = 15;
//    bool ret = false;
//        cv::inRange(hsv, minScalar, maxScalar, mask);
    cv::inRange(hsv, cv::Scalar(11, 192, 0), cv::Scalar(34, 255, 255), mask);


    std::vector< std::vector< cv::Point> > contours;
    cv::findContours(mask, contours, cv::noArray(), cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

    cv::Mat fishingTrofej;
    capture->getMatOfRect(fishingTrofej, capture->getRect("rect_trofeiFishing"));
//    cv::imshow("win5", fishingTrofej);

    cv::Point _pointFishTrof;
    if(capture->containTextMat(fishingTrofej, _pointFishTrof, "трофеи", "ru", 0)) {
        qDebug() << "+++";
        return 2;
    }

    if(contours.empty()) {
        return 0;
    }
    for(size_t i = 0; i < contours.size(); i++) {
        cv::Rect boundRect = cv::boundingRect(contours[i]);
        if(boundRect.height < 8 && boundRect.height > 4 && boundRect.width > 150) {
            cv::drawContours(m_src, contours, i, cv::Scalar(0, 255, 0));
            qDebug() << boundRect.width << boundRect.height;
            cv::Rect foundRect = cv::Rect(boundRect.x, boundRect.y - boundRect.height - 44, 370, 22);
            cv::rectangle(m_src, foundRect, cv::Scalar(0, 0, 255));
            m_src(foundRect).copyTo(m_matForRecogniz);
            cv::imshow("win2", m_src);
            cv::imshow("win3", mask);
            cv::imshow("win4", hsv);
            return 1;
        }
    }

    cv::imshow("win2", m_src);
    cv::imshow("win3", mask);
    cv::imshow("win4", hsv);
    QApplication::processEvents();




    return 0;
}


void RobloxSword::printDebug()
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

void RobloxSword::reset()
{
}

void RobloxSword::nextCell()
{
    trofejCells.currentItem++;
    if(trofejCells.currentItem == 12)
        trofejCells.currentItem = 0;
    if(trofejCells.currentItem < 6) {
        trofejCells.pointCursor.x = trofejCells.pointStart.x + trofejCells.currentItem * trofejCells.widthCell;
        trofejCells.pointCursor.y = trofejCells.pointStart.y;

    } else {
        trofejCells.pointCursor.x = trofejCells.pointStart.x + trofejCells.currentItem * trofejCells.widthCell;
        trofejCells.pointCursor.y = trofejCells.pointStart.y + trofejCells.heightCell;
    }
}

void RobloxSword::setCell(int aiValue)
{
    if(aiValue >= trofejCells.cels) {
        qDebug() << "error: нельзя выставить ячейку если их всего меньше";
        return;
    }
    trofejCells.currentItem = aiValue;
    if(trofejCells.currentItem == 12)
        trofejCells.currentItem = 0;
    if(trofejCells.currentItem < 6) {
        trofejCells.pointCursor.x = trofejCells.pointStart.x + trofejCells.currentItem * trofejCells.widthCell;
        trofejCells.pointCursor.y = trofejCells.pointStart.y;

    } else {
        trofejCells.pointCursor.x = trofejCells.pointStart.x + trofejCells.currentItem * trofejCells.widthCell;
        trofejCells.pointCursor.y = trofejCells.pointStart.y + trofejCells.heightCell;
    }
}
