#ifndef TESTS_H
#define TESTS_H

#include "baseaction.h"

class Tests : public BaseAction
{

public:
    Tests(CaptureWindow *aCapture, SocketIO *aSock);
    void init(QStringList &asListParam) override;
    bool logic(QStringList &asListParam) override;
    void printDebug() override;
    void reset() override;
};


#endif // TESTS_H
