#ifndef PANELINVENTORY_H
#define PANELINVENTORY_H

#include "baseaction.h"

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

class PanelInventory : public BaseAction
{
    enum {TRANS_ENABLE, TRANS_SRCHITEM, TRANS_ACTION} trans;
    enum {TRANS_SRCHITEM_READNAME_1, TRANS_SRCHITEM_READNAME_2} trans_srchitem;
    bool active = false;
    QString sHeaderName;
    cv::Mat m_matTooltip1;
    cv::Rect m_rectSrc;
    cv::Rect m_rectItems;
    cv::Rect m_rectTooltip1;
    Inventory inventory;

    void nextItem();
    cv::Point getPointItem();

public:
    PanelInventory(CaptureWindow *aCapture, SocketIO *aSock);
    void init(QStringList &asListParam) override;
    bool logic(QStringList &asListParam) override;
    void printDebug() override;
    void reset() override;
};



#endif // PANELINVENTORY_H
