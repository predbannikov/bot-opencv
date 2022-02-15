#ifndef MANUFACTURING_H
#define MANUFACTURING_H

#include "baseaction.h"


class Manufacturing : public BaseAction
{
    struct Inventory {
        int col;
        int row;
        int max_col;
        int max_row;
        int index;
        int count;
        int length;
        int x, y;
        cv::Rect rect;
        QString name;
        QMap <int, QString> map;
    };

    enum {TRANS_ENABLE_DIALOG, TRANS_ENABLE_STOREHOUSE, TRANS_SRCHITEM, TRANS_ADDITEM, TRANS_EXIT_HOUSESTORE,
          TRANS_ADD_TO_CRAFT, ACTION_WAIT_CRAFT,
          TRANS_END} trans;
    enum {TRANS_SRCHITEM_READNAME_1, TRANS_SRCHITEM_READNAME_2} trans_srchitem;
    bool active = false;
    bool trigger2 = false;
    QString sHeaderName;
    cv::Mat m_matTooltip1;
    cv::Rect m_rectSrc;
    cv::Rect m_rectItems;
    cv::Rect m_rectTooltip1;
    Inventory inventory;
    Inventory sotreHouse;

    void nextItem();
    void setItem(int aIndex);
    void setStoreHouse(int aIndex);
    cv::Point getPointItem();
    QStringList listForRemove;
    QString strForAdding;

public:
    Manufacturing(CaptureWindow *aCapture, SocketIO *aSock);
    void init(QStringList &asListParam) override;
    bool logic(QStringList &asListParam) override;
    void printDebug() override;
    void reset() override;
};


#endif // MANUFACTURING_H
