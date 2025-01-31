#ifndef BUTTONHANDLERS_H
#define BUTTONHANDLERS_H

#include <windows.h>  // Подключение Windows API типов
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QToolButton>
#include <QIcon>
#include <QColor>

extern QTextEdit *terminal_down;
extern QTimer *Timer;
extern UCHAR bufOutput[10];
extern DWORD Error;
extern HANDLE hECE0206_0;
extern HANDLE hECE0206_1;
extern DWORD nOutput;
extern UCHAR bufOutput[10];
extern DWORD Error;
extern QTimer *Timer;
extern QPushButton *handleStartButton;
extern QPushButton *pushButton_3;
extern ULONG OUT_AD9M2[7];
extern QPushButton* preparationButton;
extern QPushButton* button1;
extern QPushButton* button2;
extern QPushButton* button3;
extern QPushButton* button4;
extern QPushButton* button5;
extern QPushButton* button6;
extern QPushButton* button7;
extern QPushButton* button8;
extern QPushButton* button9;
extern QPushButton* button10;
extern QPushButton* button11;
extern QPushButton* button12;
extern QPushButton* button13;
extern QPushButton* button14;
extern QPushButton* button15;
extern QTableWidget *turning_on_the_equipment;
extern QToolButton *toolButton_14;
extern QToolButton *toolButton_15 ;
extern QTableWidget *checking_the_operation;

void handleStartButtonClick();
void preparation(bool checked);
void handleButtonClick1();
void handleButtonClick2();
void handleButtonClick3();
void handleButtonClick4();
void handleButtonClick5();
void handleButtonClick6();
void handleButtonClick7();
void handleButtonClick8();
void handleButtonClick9();
void handleButtonClick10();
void handleButtonClick11();
void handleButtonClick12();
void handleButtonClick13();
void handleButtonClick14();
void handleButtonClick15();
void on_pushButton_3_clicked();
void handleClearButton();
QIcon createCircleIcon(const QColor &color);

HANDLE OpenDeviceByIndex(DWORD Index, PDWORD pError);
void SI_clear_array(UCHAR Device, UCHAR ChanNumber);
void SI_pusk(UCHAR Device, UCHAR ChanNumber, UCHAR Mode, UCHAR Parity, UCHAR Freq);
void SI_stop(UCHAR Device, UCHAR ChanNumber);
void SO_stop(UCHAR Device);


#endif // BUTTONHANDLERS_H
