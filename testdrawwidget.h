#pragma once

#include <QMainWindow>
#include"camlib.h"

class TestDrawWidget : public QMainWindow
{
	Q_OBJECT

public:
	TestDrawWidget(QObject *parent);
	~TestDrawWidget();
	
	QChartView* chartView;
	QTextEdit* statShow;

	void closeEvent(QCloseEvent*);

signals:
	void widgetClose();
};
