#pragma once

#include <QThread>
#include "camlib.h"


class dataprocess : public QThread
{
	Q_OBJECT

public:
	dataprocess(QObject *parent);
	~dataprocess();
	void run() override;
signals:
    void resultReady(int buffFlag);
	void recvError(int dataNum);
};
