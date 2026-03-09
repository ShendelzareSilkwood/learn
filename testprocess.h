#pragma once
#include <QThread>
#include "camlib.h"

class test : public QThread
{
	Q_OBJECT

public:
	test(QObject *parent);
	~test();
	void run() override;
	bool dataShowFlag = true;
signals:
	void resultReady(int num);

};
