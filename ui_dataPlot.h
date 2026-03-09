/********************************************************************************
** Form generated from reading UI file 'dataPlot.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATAPLOT_H
#define UI_DATAPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <qchartview.h>

QT_BEGIN_NAMESPACE

class Ui_dataPlot
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QChartView *widget;
    QPushButton *plot_button;

    void setupUi(QWidget *dataPlot)
    {
        if (dataPlot->objectName().isEmpty())
            dataPlot->setObjectName(QString::fromUtf8("dataPlot"));
        dataPlot->resize(710, 529);
        horizontalLayout = new QHBoxLayout(dataPlot);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(dataPlot);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        widget = new QChartView(groupBox);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 20, 531, 461));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        plot_button = new QPushButton(groupBox);
        plot_button->setObjectName(QString::fromUtf8("plot_button"));
        plot_button->setGeometry(QRect(550, 60, 75, 23));

        horizontalLayout->addWidget(groupBox);


        retranslateUi(dataPlot);

        QMetaObject::connectSlotsByName(dataPlot);
    } // setupUi

    void retranslateUi(QWidget *dataPlot)
    {
        dataPlot->setWindowTitle(QApplication::translate("dataPlot", "dataPlot", nullptr));
        groupBox->setTitle(QString());
        plot_button->setText(QApplication::translate("dataPlot", "plot", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dataPlot: public Ui_dataPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATAPLOT_H
