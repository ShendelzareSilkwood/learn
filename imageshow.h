#pragma once

#include <QThread>
#include "camlib.h"

class imageshow : public QThread
{
	Q_OBJECT

public:
	imageshow(QObject *parent);
	~imageshow();
	void run() override;
	bool imageShowFlag = true;
signals:
	void imageReady();

};
