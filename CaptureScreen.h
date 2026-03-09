#pragma once
#include <QtWidgets/QMainWindow>
#include <qpainter.h>

class mousechoose;
class QPushButton;


class CaptureScreen : public QMainWindow
{
    Q_OBJECT

public:
    CaptureScreen(QWidget* parent = Q_NULLPTR);
    ~CaptureScreen();
private slots:
    void openImage();
private:
    mousechoose* m_pMyLabel;
    QWidget* m_pCenterWgt;
    QPushButton* m_pOpenBtn;

};

