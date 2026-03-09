#include "CaptureScreen.h"
#include <QImage>
#include "mousechoose.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>

CaptureScreen::CaptureScreen(QWidget* parent)
	: QMainWindow(parent)
{
	m_pCenterWgt = new QWidget(this);
	m_pMyLabel = new mousechoose(this);
	m_pOpenBtn = new QPushButton("Open", this);

	connect(m_pOpenBtn, &QPushButton::clicked, this, &CaptureScreen::openImage);

	QVBoxLayout* mainLayout = new QVBoxLayout;

	mainLayout->addWidget(m_pOpenBtn);
	mainLayout->addWidget(m_pMyLabel);
	mainLayout->addStretch();				//ռ��label����ĵط�
	m_pCenterWgt->setLayout(mainLayout);

	this->setCentralWidget(m_pCenterWgt);
	this->resize(800, 600);
}


CaptureScreen::~CaptureScreen()
{

}

void CaptureScreen::openImage()
{
	
	QString fileName = QFileDialog::getOpenFileName(this, "open", "./external/", "image (*.bmp *.png *.jpg)");

	if (fileName.isEmpty())
	{
		return;
	}

	QImage image(fileName);
	m_pMyLabel->setBackImage(image);
}