/********************************************************************************
** Form generated from reading UI file 'camlib.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMLIB_H
#define UI_CAMLIB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_camlibClass
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *plainTextEdit;
    QPlainTextEdit *plainTextEdit_2;
    QPlainTextEdit *plainTextEdit_3;
    QLabel *picture;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_open;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer;
    QPushButton *btn_A;
    QPushButton *pushButton_5;
    QSpacerItem *verticalSpacer_2;
    QPushButton *btn_RT1;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer_5;
    QPushButton *pushButton_7;
    QPushButton *pushButton_9;
    QSpacerItem *verticalSpacer_4;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QSpacerItem *verticalSpacer_3;
    QPushButton *pushButton_4;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_12;
    QPushButton *pushButton_8;
    QPushButton *pushButton_11;
    QPushButton *pushButton_10;

    void setupUi(QMainWindow *camlibClass)
    {
        if (camlibClass->objectName().isEmpty())
            camlibClass->setObjectName(QString::fromUtf8("camlibClass"));
        camlibClass->resize(1280, 720);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(camlibClass->sizePolicy().hasHeightForWidth());
        camlibClass->setSizePolicy(sizePolicy);
        camlibClass->setMinimumSize(QSize(1280, 720));
        camlibClass->setMaximumSize(QSize(1280, 720));
        camlibClass->setBaseSize(QSize(0, 0));
        centralWidget = new QWidget(camlibClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(320, 320, 960, 320));
        groupBox_2->setMinimumSize(QSize(960, 320));
        groupBox_2->setMaximumSize(QSize(960, 320));
        horizontalLayout = new QHBoxLayout(groupBox_2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        plainTextEdit = new QPlainTextEdit(groupBox_2);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(plainTextEdit->sizePolicy().hasHeightForWidth());
        plainTextEdit->setSizePolicy(sizePolicy1);
        plainTextEdit->setMinimumSize(QSize(300, 300));
        plainTextEdit->setMaximumSize(QSize(300, 300));

        horizontalLayout->addWidget(plainTextEdit);

        plainTextEdit_2 = new QPlainTextEdit(groupBox_2);
        plainTextEdit_2->setObjectName(QString::fromUtf8("plainTextEdit_2"));
        sizePolicy1.setHeightForWidth(plainTextEdit_2->sizePolicy().hasHeightForWidth());
        plainTextEdit_2->setSizePolicy(sizePolicy1);
        plainTextEdit_2->setMinimumSize(QSize(300, 300));
        plainTextEdit_2->setMaximumSize(QSize(300, 300));

        horizontalLayout->addWidget(plainTextEdit_2);

        plainTextEdit_3 = new QPlainTextEdit(groupBox_2);
        plainTextEdit_3->setObjectName(QString::fromUtf8("plainTextEdit_3"));
        sizePolicy1.setHeightForWidth(plainTextEdit_3->sizePolicy().hasHeightForWidth());
        plainTextEdit_3->setSizePolicy(sizePolicy1);
        plainTextEdit_3->setMinimumSize(QSize(300, 300));
        plainTextEdit_3->setMaximumSize(QSize(300, 300));

        horizontalLayout->addWidget(plainTextEdit_3);

        picture = new QLabel(centralWidget);
        picture->setObjectName(QString::fromUtf8("picture"));
        picture->setEnabled(true);
        picture->setGeometry(QRect(0, 0, 1280, 320));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(picture->sizePolicy().hasHeightForWidth());
        picture->setSizePolicy(sizePolicy2);
        picture->setMinimumSize(QSize(1280, 320));
        picture->setMaximumSize(QSize(1280, 320));
        picture->setFrameShape(QFrame::Box);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(0, 320, 320, 320));
        groupBox->setMinimumSize(QSize(320, 320));
        groupBox->setMaximumSize(QSize(320, 320));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        btn_open = new QPushButton(groupBox);
        btn_open->setObjectName(QString::fromUtf8("btn_open"));
        btn_open->setEnabled(true);
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btn_open->sizePolicy().hasHeightForWidth());
        btn_open->setSizePolicy(sizePolicy3);
        btn_open->setMinimumSize(QSize(75, 25));
        btn_open->setMaximumSize(QSize(75, 25));

        gridLayout->addWidget(btn_open, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 4, 1, 1);

        verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 1, 1, 1, 1);

        btn_A = new QPushButton(groupBox);
        btn_A->setObjectName(QString::fromUtf8("btn_A"));
        sizePolicy3.setHeightForWidth(btn_A->sizePolicy().hasHeightForWidth());
        btn_A->setSizePolicy(sizePolicy3);
        btn_A->setMinimumSize(QSize(75, 25));
        btn_A->setMaximumSize(QSize(75, 25));

        gridLayout->addWidget(btn_A, 2, 1, 1, 1);

        pushButton_5 = new QPushButton(groupBox);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        sizePolicy3.setHeightForWidth(pushButton_5->sizePolicy().hasHeightForWidth());
        pushButton_5->setSizePolicy(sizePolicy3);
        pushButton_5->setMinimumSize(QSize(75, 25));
        pushButton_5->setMaximumSize(QSize(75, 25));

        gridLayout->addWidget(pushButton_5, 2, 3, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 3, 1, 1, 1);

        btn_RT1 = new QPushButton(groupBox);
        btn_RT1->setObjectName(QString::fromUtf8("btn_RT1"));
        sizePolicy3.setHeightForWidth(btn_RT1->sizePolicy().hasHeightForWidth());
        btn_RT1->setSizePolicy(sizePolicy3);
        btn_RT1->setMinimumSize(QSize(75, 25));
        btn_RT1->setMaximumSize(QSize(75, 25));

        gridLayout->addWidget(btn_RT1, 4, 1, 1, 1);

        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        sizePolicy3.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy3);
        pushButton_2->setMinimumSize(QSize(75, 25));
        pushButton_2->setMaximumSize(QSize(75, 25));

        gridLayout->addWidget(pushButton_2, 4, 3, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_5, 5, 1, 1, 1);

        pushButton_7 = new QPushButton(groupBox);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        sizePolicy3.setHeightForWidth(pushButton_7->sizePolicy().hasHeightForWidth());
        pushButton_7->setSizePolicy(sizePolicy3);
        pushButton_7->setMinimumSize(QSize(75, 25));
        pushButton_7->setMaximumSize(QSize(75, 25));

        gridLayout->addWidget(pushButton_7, 6, 1, 1, 1);

        pushButton_9 = new QPushButton(groupBox);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        sizePolicy3.setHeightForWidth(pushButton_9->sizePolicy().hasHeightForWidth());
        pushButton_9->setSizePolicy(sizePolicy3);
        pushButton_9->setMinimumSize(QSize(75, 25));
        pushButton_9->setMaximumSize(QSize(75, 25));

        gridLayout->addWidget(pushButton_9, 6, 3, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_4, 7, 1, 1, 1);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        sizePolicy3.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy3);
        pushButton->setMinimumSize(QSize(75, 25));
        pushButton->setMaximumSize(QSize(75, 25));

        gridLayout->addWidget(pushButton, 8, 1, 1, 1);

        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setMinimumSize(QSize(75, 25));
        pushButton_3->setMaximumSize(QSize(75, 25));

        gridLayout->addWidget(pushButton_3, 8, 3, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_3, 9, 1, 1, 1);

        pushButton_4 = new QPushButton(groupBox);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        gridLayout->addWidget(pushButton_4, 0, 3, 1, 1);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(500, 650, 280, 45));
        sizePolicy3.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy3);
        groupBox_3->setMinimumSize(QSize(280, 45));
        groupBox_3->setMaximumSize(QSize(280, 45));
        horizontalLayout_2 = new QHBoxLayout(groupBox_3);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton_12 = new QPushButton(groupBox_3);
        pushButton_12->setObjectName(QString::fromUtf8("pushButton_12"));

        horizontalLayout_2->addWidget(pushButton_12);

        pushButton_8 = new QPushButton(groupBox_3);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));

        horizontalLayout_2->addWidget(pushButton_8);

        pushButton_11 = new QPushButton(groupBox_3);
        pushButton_11->setObjectName(QString::fromUtf8("pushButton_11"));

        horizontalLayout_2->addWidget(pushButton_11);

        pushButton_10 = new QPushButton(groupBox_3);
        pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));

        horizontalLayout_2->addWidget(pushButton_10);

        camlibClass->setCentralWidget(centralWidget);

        retranslateUi(camlibClass);

        QMetaObject::connectSlotsByName(camlibClass);
    } // setupUi

    void retranslateUi(QMainWindow *camlibClass)
    {
        camlibClass->setWindowTitle(QApplication::translate("camlibClass", "camlib", nullptr));
        groupBox_2->setTitle(QString());
        picture->setText(QString());
        groupBox->setTitle(QString());
        btn_open->setText(QApplication::translate("camlibClass", "\346\211\223\345\274\200\347\233\270\346\234\272", nullptr));
        btn_A->setText(QApplication::translate("camlibClass", "\345\206\205\345\217\202\350\256\241\347\256\227", nullptr));
        pushButton_5->setText(QApplication::translate("camlibClass", "\345\212\240\350\275\275\345\206\205\345\217\202", nullptr));
        btn_RT1->setText(QApplication::translate("camlibClass", "\345\244\226\345\217\202\350\256\241\347\256\227", nullptr));
        pushButton_2->setText(QApplication::translate("camlibClass", "\345\233\276\345\203\217\346\210\252\345\217\226", nullptr));
        pushButton_7->setText(QApplication::translate("camlibClass", "\344\270\255\345\277\203\346\217\220\345\217\226", nullptr));
        pushButton_9->setText(QApplication::translate("camlibClass", "\347\225\270\345\217\230\347\237\253\346\255\243", nullptr));
        pushButton->setText(QApplication::translate("camlibClass", "\347\202\271\344\272\221\350\256\241\347\256\227", nullptr));
        pushButton_3->setText(QApplication::translate("camlibClass", "\346\230\240\345\260\204\350\241\250\350\256\241\347\256\227", nullptr));
        pushButton_4->setText(QApplication::translate("camlibClass", "\345\233\276\345\203\217\350\216\267\345\217\226", nullptr));
        groupBox_3->setTitle(QString());
        pushButton_12->setText(QApplication::translate("camlibClass", "\350\203\214\346\231\257\344\270\255\345\277\203", nullptr));
        pushButton_8->setText(QApplication::translate("camlibClass", "\350\203\214\346\231\257\344\277\235\345\255\230", nullptr));
        pushButton_11->setText(QApplication::translate("camlibClass", "\347\202\271\344\272\221\346\230\276\347\244\272", nullptr));
        pushButton_10->setText(QApplication::translate("camlibClass", "\346\265\213\350\257\225\346\250\241\345\274\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class camlibClass: public Ui_camlibClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMLIB_H
