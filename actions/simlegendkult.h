#ifndef SIMLEGENDKULT_H
#define SIMLEGENDKULT_H
#include "baseaction.h"

class SimLegendKult : public BaseAction
{
    int countClick = 0;
    enum STATE_PUMP {STATE_PUMP_1, STATE_PUMP_2, STATE_PUMP_3, STATE_PUMP_4, STATE_PUMP_5};
    STATE_PUMP statePump;
public:
    SimLegendKult(CaptureWindow *aCapture, SocketIO *aSock);
    void init(QStringList &asListParam) override;
    bool logic(QStringList &asListParam) override;
    void reset() override;
};




#endif // SIMLEGENDKULT_H
