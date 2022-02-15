#ifndef FISHING_H
#define FISHING_H

#include "baseaction.h"


class fishing : public BaseAction
{

    enum trans {TRANS_1, TRANS_2, TRANS_3, TRANS_4, TRANS_5, TRANS_6} trans;
    enum {TRANS_SRCHITEM_1, TRANS_SRCHITEM_2} trans_searchItem;

    cv::Rect m_rectSrc;
    cv::Rect m_rectForRecogniz;
    cv::Mat m_matForRecogniz;

    QStringList listForRemove;

    struct TrofejCells{
        int col;
        int row;
        int cels;
        int currentItem;
        cv::Point pointStart;
        cv::Point pointCursor;
        QString nameItem;
        int widthCell;
        int heightCell;
    } trofejCells;

    bool waitSpace();
    bool recognizeKey(int aType = 0);
    int findRectRecognize();
    int colKey;
public:
    fishing(CaptureWindow *aCapture, SocketIO *aSock);
    void init(QStringList &asListParam) override;
    bool logic(QStringList &asListParam) override;
    void printDebug() override;
    void reset() override;

private:
    void nextCell();
    void setCell(int aiValue);
};
#endif // FISHING_H
