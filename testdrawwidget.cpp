#include "testdrawwidget.h"


TestDrawWidget::TestDrawWidget(QObject *parent)
{
	//ÉèÖÃÍ¼±íÏÔÊ¾
	QWidget* widget = new QWidget(this);
	QHBoxLayout* mainLayout = new QHBoxLayout();
	chartView = new QChartView(this);
	statShow = new QTextEdit(this);
	mainLayout->addWidget(chartView);
	mainLayout->addWidget(statShow);

	widget->setLayout(mainLayout);
	this->setCentralWidget(widget);
}

TestDrawWidget::~TestDrawWidget()
{
}

void TestDrawWidget::closeEvent(QCloseEvent*)
{
	emit widgetClose();
}