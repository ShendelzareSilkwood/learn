#pragma once
#include <QtCharts>
#include <QWidget>
#include "ui_dataPlot.h"
#include "Pretreat.h"

using namespace cv;

class dataPlot : public QWidget
{
	Q_OBJECT

public:
	dataPlot(QWidget *parent = Q_NULLPTR);
	~dataPlot();
	

public slots:

public:
	QChart* m_chart;
	void m_plot(vector<vector<Point3d>> m_data);

private:
	Ui::dataPlot ui;
};
