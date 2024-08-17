/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_10;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_9;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_5;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QComboBox *comboBox_3;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_4;
    QComboBox *comboBox_4;
    QVBoxLayout *verticalLayout_6;
    QPushButton *StartPushBtn;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_8;
    QToolButton *toolButton_14;
    QToolButton *toolButton_15;
    QToolButton *toolButton_16;
    QToolButton *toolButton_17;
    QToolButton *toolButton_18;
    QToolButton *toolButton_19;
    QToolButton *toolButton_20;
    QTextEdit *textEdit;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_2;
    QTableWidget *tableWidget_2;
    QGroupBox *groupBox_7;
    QHBoxLayout *horizontalLayout_11;
    QTextEdit *textEdit_2;
    QVBoxLayout *verticalLayout_9;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_8;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QCheckBox *checkBox_7;
    QCheckBox *checkBox_8;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_7;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1593, 1221);
        action = new QAction(MainWindow);
        action->setObjectName("action");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_10 = new QVBoxLayout(centralwidget);
        verticalLayout_10->setObjectName("verticalLayout_10");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        horizontalLayout_9 = new QHBoxLayout(groupBox);
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        widget_2 = new QWidget(groupBox);
        widget_2->setObjectName("widget_2");
        verticalLayout_4 = new QVBoxLayout(widget_2);
        verticalLayout_4->setObjectName("verticalLayout_4");
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        groupBox_2 = new QGroupBox(widget_2);
        groupBox_2->setObjectName("groupBox_2");
        verticalLayout_5 = new QVBoxLayout(groupBox_2);
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName("label_3");

        horizontalLayout_6->addWidget(label_3);

        comboBox_3 = new QComboBox(groupBox_2);
        comboBox_3->setObjectName("comboBox_3");

        horizontalLayout_6->addWidget(comboBox_3);


        verticalLayout_5->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName("label_4");

        horizontalLayout_7->addWidget(label_4);

        comboBox_4 = new QComboBox(groupBox_2);
        comboBox_4->setObjectName("comboBox_4");

        horizontalLayout_7->addWidget(comboBox_4);


        verticalLayout_5->addLayout(horizontalLayout_7);


        horizontalLayout_5->addWidget(groupBox_2);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName("verticalLayout_6");
        StartPushBtn = new QPushButton(widget_2);
        StartPushBtn->setObjectName("StartPushBtn");

        verticalLayout_6->addWidget(StartPushBtn);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_2);


        horizontalLayout_5->addLayout(verticalLayout_6);


        verticalLayout_4->addLayout(horizontalLayout_5);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalLayout_8->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        toolButton_14 = new QToolButton(widget_2);
        toolButton_14->setObjectName("toolButton_14");
        toolButton_14->setMinimumSize(QSize(60, 60));
        toolButton_14->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

        horizontalLayout_8->addWidget(toolButton_14);

        toolButton_15 = new QToolButton(widget_2);
        toolButton_15->setObjectName("toolButton_15");
        toolButton_15->setMinimumSize(QSize(60, 60));
        toolButton_15->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

        horizontalLayout_8->addWidget(toolButton_15);

        toolButton_16 = new QToolButton(widget_2);
        toolButton_16->setObjectName("toolButton_16");
        toolButton_16->setMinimumSize(QSize(60, 60));
        toolButton_16->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

        horizontalLayout_8->addWidget(toolButton_16);

        toolButton_17 = new QToolButton(widget_2);
        toolButton_17->setObjectName("toolButton_17");
        toolButton_17->setMinimumSize(QSize(60, 60));
        toolButton_17->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

        horizontalLayout_8->addWidget(toolButton_17);

        toolButton_18 = new QToolButton(widget_2);
        toolButton_18->setObjectName("toolButton_18");
        toolButton_18->setMinimumSize(QSize(60, 60));
        toolButton_18->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

        horizontalLayout_8->addWidget(toolButton_18);

        toolButton_19 = new QToolButton(widget_2);
        toolButton_19->setObjectName("toolButton_19");
        toolButton_19->setMinimumSize(QSize(60, 60));
        toolButton_19->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

        horizontalLayout_8->addWidget(toolButton_19);

        toolButton_20 = new QToolButton(widget_2);
        toolButton_20->setObjectName("toolButton_20");
        toolButton_20->setMinimumSize(QSize(60, 60));
        toolButton_20->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

        horizontalLayout_8->addWidget(toolButton_20);


        verticalLayout_4->addLayout(horizontalLayout_8);


        horizontalLayout_9->addWidget(widget_2);

        textEdit = new QTextEdit(groupBox);
        textEdit->setObjectName("textEdit");

        horizontalLayout_9->addWidget(textEdit);


        verticalLayout_10->addWidget(groupBox);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName("groupBox_3");
        gridLayout = new QGridLayout(groupBox_3);
        gridLayout->setObjectName("gridLayout");
        tableWidget = new QTableWidget(groupBox_3);
        if (tableWidget->columnCount() < 1)
            tableWidget->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        if (tableWidget->rowCount() < 1)
            tableWidget->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem1);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setAutoFillBackground(false);
        tableWidget->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);

        gridLayout->addWidget(tableWidget, 0, 0, 1, 1);


        verticalLayout_10->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(centralwidget);
        groupBox_4->setObjectName("groupBox_4");
        gridLayout_2 = new QGridLayout(groupBox_4);
        gridLayout_2->setObjectName("gridLayout_2");
        tableWidget_2 = new QTableWidget(groupBox_4);
        if (tableWidget_2->columnCount() < 1)
            tableWidget_2->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        if (tableWidget_2->rowCount() < 1)
            tableWidget_2->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(0, __qtablewidgetitem3);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setAutoFillBackground(false);
        tableWidget_2->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);

        gridLayout_2->addWidget(tableWidget_2, 0, 0, 1, 1);


        verticalLayout_10->addWidget(groupBox_4);

        groupBox_7 = new QGroupBox(centralwidget);
        groupBox_7->setObjectName("groupBox_7");
        horizontalLayout_11 = new QHBoxLayout(groupBox_7);
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        textEdit_2 = new QTextEdit(groupBox_7);
        textEdit_2->setObjectName("textEdit_2");

        horizontalLayout_11->addWidget(textEdit_2);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName("verticalLayout_9");
        groupBox_6 = new QGroupBox(groupBox_7);
        groupBox_6->setObjectName("groupBox_6");
        verticalLayout_8 = new QVBoxLayout(groupBox_6);
        verticalLayout_8->setObjectName("verticalLayout_8");
        checkBox_5 = new QCheckBox(groupBox_6);
        checkBox_5->setObjectName("checkBox_5");

        verticalLayout_8->addWidget(checkBox_5);

        checkBox_6 = new QCheckBox(groupBox_6);
        checkBox_6->setObjectName("checkBox_6");

        verticalLayout_8->addWidget(checkBox_6);

        checkBox_7 = new QCheckBox(groupBox_6);
        checkBox_7->setObjectName("checkBox_7");

        verticalLayout_8->addWidget(checkBox_7);

        checkBox_8 = new QCheckBox(groupBox_6);
        checkBox_8->setObjectName("checkBox_8");

        verticalLayout_8->addWidget(checkBox_8);


        verticalLayout_9->addWidget(groupBox_6);

        groupBox_5 = new QGroupBox(groupBox_7);
        groupBox_5->setObjectName("groupBox_5");
        verticalLayout_7 = new QVBoxLayout(groupBox_5);
        verticalLayout_7->setObjectName("verticalLayout_7");
        checkBox = new QCheckBox(groupBox_5);
        checkBox->setObjectName("checkBox");

        verticalLayout_7->addWidget(checkBox);

        checkBox_2 = new QCheckBox(groupBox_5);
        checkBox_2->setObjectName("checkBox_2");

        verticalLayout_7->addWidget(checkBox_2);

        checkBox_3 = new QCheckBox(groupBox_5);
        checkBox_3->setObjectName("checkBox_3");

        verticalLayout_7->addWidget(checkBox_3);

        checkBox_4 = new QCheckBox(groupBox_5);
        checkBox_4->setObjectName("checkBox_4");

        verticalLayout_7->addWidget(checkBox_4);


        verticalLayout_9->addWidget(groupBox_5);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        pushButton_3 = new QPushButton(groupBox_7);
        pushButton_3->setObjectName("pushButton_3");

        horizontalLayout_10->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(groupBox_7);
        pushButton_4->setObjectName("pushButton_4");

        horizontalLayout_10->addWidget(pushButton_4);


        verticalLayout_9->addLayout(horizontalLayout_10);


        horizontalLayout_11->addLayout(verticalLayout_9);


        verticalLayout_10->addWidget(groupBox_7);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1593, 22));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName("menu_2");
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName("menu_3");
        menu_4 = new QMenu(menubar);
        menu_4->setObjectName("menu_4");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menubar->addAction(menu_4->menuAction());
        menu->addAction(action);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\271\321\202\320\270", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\261\320\276\321\200 \321\200\320\265\320\266\320\270\320\274\320\260 \320\272\320\276\320\275\321\202\321\200\320\276\320\273\321\217", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\275\321\202\321\200\320\276\320\273\321\214", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\320\242\320\270\320\277", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        StartPushBtn->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\320\260\321\200\321\202", nullptr));
        toolButton_14->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_15->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_16->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_17->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_18->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_19->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_20->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\320\260\320\277\321\200\320\262", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \321\201\321\202\320\276\320\273\320\261\320\265\321\206", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "\320\235\320\276\320\262\320\260\321\217 \321\201\321\202\321\200\320\276\320\272\320\260", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "\320\260\320\277\321\200\320\262", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \321\201\321\202\320\276\320\273\320\261\320\265\321\206", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_2->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "\320\235\320\276\320\262\320\260\321\217 \321\201\321\202\321\200\320\276\320\272\320\260", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        checkBox_5->setText(QCoreApplication::translate("MainWindow", "CheckBox", nullptr));
        checkBox_6->setText(QCoreApplication::translate("MainWindow", "CheckBox", nullptr));
        checkBox_7->setText(QCoreApplication::translate("MainWindow", "CheckBox", nullptr));
        checkBox_8->setText(QCoreApplication::translate("MainWindow", "CheckBox", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        checkBox->setText(QCoreApplication::translate("MainWindow", "CheckBox", nullptr));
        checkBox_2->setText(QCoreApplication::translate("MainWindow", "CheckBox", nullptr));
        checkBox_3->setText(QCoreApplication::translate("MainWindow", "CheckBox", nullptr));
        checkBox_4->setText(QCoreApplication::translate("MainWindow", "CheckBox", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
        menu_2->setTitle(QCoreApplication::translate("MainWindow", "\320\222\320\270\320\264", nullptr));
        menu_3->setTitle(QCoreApplication::translate("MainWindow", "\320\243\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270", nullptr));
        menu_4->setTitle(QCoreApplication::translate("MainWindow", "?", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
