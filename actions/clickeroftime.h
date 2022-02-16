#ifndef CLICKEROFTIME_H
#define CLICKEROFTIME_H
#include "baseaction.h"

class ClickerOfTime : public BaseAction
{
    int countClick = 0;
    enum STATE {STATE_1, STATE_2};
    cv::Point pointClick;
    cv::Point pointRebuild;
public:
    STATE state;
    ClickerOfTime(CaptureWindow *aCapture, SocketIO *aSock);
    void init(QStringList &asListParam) override;
    bool logic(QStringList &asListParam) override;
    void reset() override;
};



#endif // CLICKEROFTIME_H
