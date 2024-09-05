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
    QHBoxLayout *horizontalLayout_8;
    QToolButton *toolButton_14;
    QToolButton *toolButton_15;
    QToolButton *toolButton_16;
    QToolButton *toolButton_17;
    QToolButton *toolButton_18;
    QToolButton *toolButton_19;
    QToolButton *toolButton_20;
    QTextEdit *terminal_up;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout;
    QTableWidget *turning_on_the_equipment;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_2;
    QTableWidget *checking_the_operation;
    QGroupBox *groupBox_7;
    QHBoxLayout *horizontalLayout_11;
    QTextEdit *terminal_down;
    QVBoxLayout *verticalLayout_9;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_8;
    QCheckBox *TM;
    QCheckBox *KPA;
    QCheckBox *AD9M2;
    QCheckBox *P9M;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_7;
    QCheckBox *reception;
    QCheckBox *broadcast;
    QCheckBox *codes;
    QCheckBox *parameters;
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
        MainWindow->setMinimumSize(QSize(0, 300));
        action = new QAction(MainWindow);
        action->setObjectName("action");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_10 = new QVBoxLayout(centralwidget);
        verticalLayout_10->setObjectName("verticalLayout_10");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setMinimumSize(QSize(0, 220));
        groupBox->setMaximumSize(QSize(16777215, 220));
        QFont font;
        font.setFamilies({QString::fromUtf8("Times New Roman")});
        font.setPointSize(12);
        groupBox->setFont(font);
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
        groupBox_2->setMinimumSize(QSize(250, 105));
        groupBox_2->setMaximumSize(QSize(250, 105));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Times New Roman")});
        font1.setPointSize(14);
        groupBox_2->setFont(font1);
        verticalLayout_5 = new QVBoxLayout(groupBox_2);
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName("label_3");
        label_3->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_6->addWidget(label_3);

        comboBox_3 = new QComboBox(groupBox_2);
        comboBox_3->addItem(QString());
        comboBox_3->setObjectName("comboBox_3");

        horizontalLayout_6->addWidget(comboBox_3);


        verticalLayout_5->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName("label_4");
        label_4->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_7->addWidget(label_4);

        comboBox_4 = new QComboBox(groupBox_2);
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->setObjectName("comboBox_4");

        horizontalLayout_7->addWidget(comboBox_4);


        verticalLayout_5->addLayout(horizontalLayout_7);


        horizontalLayout_5->addWidget(groupBox_2);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName("verticalLayout_6");

        horizontalLayout_5->addLayout(verticalLayout_6);


        verticalLayout_4->addLayout(horizontalLayout_5);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalLayout_8->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        toolButton_14 = new QToolButton(widget_2);
        toolButton_14->setObjectName("toolButton_14");
        toolButton_14->setEnabled(false);
        toolButton_14->setMinimumSize(QSize(35, 35));
        toolButton_14->setMaximumSize(QSize(30, 30));
        toolButton_14->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

        horizontalLayout_8->addWidget(toolButton_14);

        toolButton_15 = new QToolButton(widget_2);
        toolButton_15->setObjectName("toolButton_15");
        toolButton_15->setEnabled(false);
        toolButton_15->setMinimumSize(QSize(35, 35));
        toolButton_15->setMaximumSize(QSize(30, 30));
        toolButton_15->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

        horizontalLayout_8->addWidget(toolButton_15);

        toolButton_16 = new QToolButton(widget_2);
        toolButton_16->setObjectName("toolButton_16");
        toolButton_16->setEnabled(false);
        toolButton_16->setMinimumSize(QSize(35, 35));
        toolButton_16->setMaximumSize(QSize(30, 30));
        toolButton_16->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

        horizontalLayout_8->addWidget(toolButton_16);

        toolButton_17 = new QToolButton(widget_2);
        toolButton_17->setObjectName("toolButton_17");
        toolButton_17->setEnabled(false);
        toolButton_17->setMinimumSize(QSize(35, 35));
        toolButton_17->setMaximumSize(QSize(30, 30));
        toolButton_17->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

        horizontalLayout_8->addWidget(toolButton_17);

        toolButton_18 = new QToolButton(widget_2);
        toolButton_18->setObjectName("toolButton_18");
        toolButton_18->setEnabled(false);
        toolButton_18->setMinimumSize(QSize(35, 35));
        toolButton_18->setMaximumSize(QSize(30, 30));
        toolButton_18->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

        horizontalLayout_8->addWidget(toolButton_18);

        toolButton_19 = new QToolButton(widget_2);
        toolButton_19->setObjectName("toolButton_19");
        toolButton_19->setEnabled(false);
        toolButton_19->setMinimumSize(QSize(35, 35));
        toolButton_19->setMaximumSize(QSize(30, 30));
        toolButton_19->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

        horizontalLayout_8->addWidget(toolButton_19);

        toolButton_20 = new QToolButton(widget_2);
        toolButton_20->setObjectName("toolButton_20");
        toolButton_20->setEnabled(false);
        toolButton_20->setMinimumSize(QSize(35, 35));
        toolButton_20->setMaximumSize(QSize(30, 30));
        toolButton_20->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);

        horizontalLayout_8->addWidget(toolButton_20);


        verticalLayout_4->addLayout(horizontalLayout_8);


        horizontalLayout_9->addWidget(widget_2);

        terminal_up = new QTextEdit(groupBox);
        terminal_up->setObjectName("terminal_up");
        terminal_up->setMinimumSize(QSize(0, 100));
        terminal_up->setMaximumSize(QSize(16777215, 150));

        horizontalLayout_9->addWidget(terminal_up);


        verticalLayout_10->addWidget(groupBox);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setMaximumSize(QSize(16777215, 173));
        groupBox_3->setFont(font);
        gridLayout = new QGridLayout(groupBox_3);
        gridLayout->setObjectName("gridLayout");
        turning_on_the_equipment = new QTableWidget(groupBox_3);
        if (turning_on_the_equipment->columnCount() < 3)
            turning_on_the_equipment->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font);
        turning_on_the_equipment->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font);
        turning_on_the_equipment->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font);
        turning_on_the_equipment->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (turning_on_the_equipment->rowCount() < 3)
            turning_on_the_equipment->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font);
        turning_on_the_equipment->setVerticalHeaderItem(0, __qtablewidgetitem3);
        turning_on_the_equipment->setObjectName("turning_on_the_equipment");
        turning_on_the_equipment->setMaximumSize(QSize(16777215, 173));
        turning_on_the_equipment->setAutoFillBackground(false);
        turning_on_the_equipment->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);

        gridLayout->addWidget(turning_on_the_equipment, 0, 0, 1, 1);


        verticalLayout_10->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(centralwidget);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setFont(font);
        gridLayout_2 = new QGridLayout(groupBox_4);
        gridLayout_2->setObjectName("gridLayout_2");
        checking_the_operation = new QTableWidget(groupBox_4);
        checking_the_operation->setObjectName("checking_the_operation");
        checking_the_operation->setMaximumSize(QSize(16777215, 500));
        checking_the_operation->setAutoFillBackground(false);
        checking_the_operation->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);

        gridLayout_2->addWidget(checking_the_operation, 0, 0, 1, 1);


        verticalLayout_10->addWidget(groupBox_4);

        groupBox_7 = new QGroupBox(centralwidget);
        groupBox_7->setObjectName("groupBox_7");
        groupBox_7->setMaximumSize(QSize(16777215, 265));
        groupBox_7->setFont(font);
        horizontalLayout_11 = new QHBoxLayout(groupBox_7);
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        terminal_down = new QTextEdit(groupBox_7);
        terminal_down->setObjectName("terminal_down");

        horizontalLayout_11->addWidget(terminal_down);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName("verticalLayout_9");
        groupBox_6 = new QGroupBox(groupBox_7);
        groupBox_6->setObjectName("groupBox_6");
        verticalLayout_8 = new QVBoxLayout(groupBox_6);
        verticalLayout_8->setObjectName("verticalLayout_8");
        TM = new QCheckBox(groupBox_6);
        TM->setObjectName("TM");

        verticalLayout_8->addWidget(TM);

        KPA = new QCheckBox(groupBox_6);
        KPA->setObjectName("KPA");

        verticalLayout_8->addWidget(KPA);

        AD9M2 = new QCheckBox(groupBox_6);
        AD9M2->setObjectName("AD9M2");

        verticalLayout_8->addWidget(AD9M2);

        P9M = new QCheckBox(groupBox_6);
        P9M->setObjectName("P9M");

        verticalLayout_8->addWidget(P9M);


        verticalLayout_9->addWidget(groupBox_6);

        groupBox_5 = new QGroupBox(groupBox_7);
        groupBox_5->setObjectName("groupBox_5");
        verticalLayout_7 = new QVBoxLayout(groupBox_5);
        verticalLayout_7->setObjectName("verticalLayout_7");
        reception = new QCheckBox(groupBox_5);
        reception->setObjectName("reception");

        verticalLayout_7->addWidget(reception);

        broadcast = new QCheckBox(groupBox_5);
        broadcast->setObjectName("broadcast");

        verticalLayout_7->addWidget(broadcast);

        codes = new QCheckBox(groupBox_5);
        codes->setObjectName("codes");

        verticalLayout_7->addWidget(codes);

        parameters = new QCheckBox(groupBox_5);
        parameters->setObjectName("parameters");

        verticalLayout_7->addWidget(parameters);


        verticalLayout_9->addWidget(groupBox_5);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        pushButton_3 = new QPushButton(groupBox_7);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setCheckable(false);

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
        menubar->setGeometry(QRect(0, 0, 1593, 26));
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
        comboBox_3->setItemText(0, QCoreApplication::translate("MainWindow", "\320\232\320\276\320\275\321\202\321\200\320\276\320\273\321\214 \320\2421\320\220-02", nullptr));

        label_4->setText(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\266\320\270\320\274", nullptr));
        comboBox_4->setItemText(0, QCoreApplication::translate("MainWindow", "\320\240\321\203\321\207\320\275\320\276\320\271", nullptr));
        comboBox_4->setItemText(1, QCoreApplication::translate("MainWindow", "\320\220\320\262\321\202\320\276\320\274\320\260\321\202\320\270\321\207\320\265\321\201\320\272\320\270\320\271", nullptr));

        toolButton_14->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_15->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_16->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_17->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_18->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_19->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        toolButton_20->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\320\222\320\272\320\273\321\216\321\207\320\265\320\275\320\270\320\265 \320\260\320\277\320\277\320\260\321\200\320\260\321\202\321\203\321\200\321\213 \320\2421\320\220-02", nullptr));
        QTableWidgetItem *___qtablewidgetitem = turning_on_the_equipment->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\277\320\276\320\273\320\275\321\217\320\265\320\274\321\213\320\265 \320\276\320\277\320\265\321\200\320\260\321\206\320\270\320\270", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = turning_on_the_equipment->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "\320\230\320\275\320\264\320\270\320\272\320\260\321\206\320\270\321\217 \321\200\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\320\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = turning_on_the_equipment->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\277\320\276\320\273\320\275\320\270\321\202\320\265\320\273\321\214\320\275\320\260\321\217 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = turning_on_the_equipment->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\276\320\262\320\265\321\200\320\272\320\260 \321\204\321\203\320\275\320\272\321\206\320\270\320\276\320\275\320\270\321\200\320\276\320\262\320\260\320\275\320\270\321\217", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("MainWindow", "\320\226\321\203\321\200\320\275\320\260\320\273", nullptr));
        groupBox_6->setTitle(QString());
        TM->setText(QCoreApplication::translate("MainWindow", "\320\242\320\234", nullptr));
        KPA->setText(QCoreApplication::translate("MainWindow", "\320\232\320\237\320\220", nullptr));
        AD9M2->setText(QCoreApplication::translate("MainWindow", "\320\220\320\2249-\320\2342", nullptr));
        P9M->setText(QCoreApplication::translate("MainWindow", "\320\2379\320\234", nullptr));
        groupBox_5->setTitle(QString());
        reception->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\270\321\221\320\274", nullptr));
        broadcast->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\200\320\265\320\264\320\260\321\207\320\260", nullptr));
        codes->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\264\321\213", nullptr));
        parameters->setText(QCoreApplication::translate("MainWindow", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "\320\237\320\260\321\203\320\267\320\260", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\320\270\321\201\321\214", nullptr));
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
