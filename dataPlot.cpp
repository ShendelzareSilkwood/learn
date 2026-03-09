#include "dataPlot.h"

dataPlot::dataPlot(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

dataPlot::~dataPlot()
{
	
}

void dataPlot::m_plot(vector<vector<Point3d>> m_data)
{
	
	if (m_data.empty())
	{
		QMessageBox::information(NULL, "INFO", "��������ȡ����", QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}
	int num = m_data[0].size();
	m_chart = new QChart();
	m_chart->setTitle("����߶�ֵ");

	QLineSeries* series = new QLineSeries();
	QScatterSeries* series1 = new QScatterSeries();
	for (int i = 0; i < num; i++)
	{
		series->append(QPointF(i, m_data[0][i].y));
		if (i % 100 == 0)
			series1->append(QPointF(i, m_data[0][i].y));
	}
	series1->setPointLabelsFormat("(@xPoint,@yPoint)");
	series1->setMarkerSize(6);
	series1->setPointLabelsVisible();

	QValueAxis* xAxis = new QValueAxis();
	QValueAxis* yAxis = new QValueAxis();
	xAxis->setRange(0, 1279);
	yAxis->setRange(0, 2200);

	m_chart->addSeries(series);
	m_chart->addSeries(series1);
	m_chart->addAxis(xAxis, Qt::AlignBottom);
	m_chart->addAxis(yAxis, Qt::AlignLeft);
	series->attachAxis(xAxis);
	series->attachAxis(yAxis);
	series1->attachAxis(xAxis);
	series1->attachAxis(yAxis);

	m_chart->setTheme(QChart::ChartThemeDark);
	m_chart->legend()->hide();

	ui.widget->setChart(m_chart);
	ui.widget->setRubberBand(QChartView::RectangleRubberBand);
	ui.widget->setRenderHint(QPainter::Antialiasing, true);

}