#ifndef BUTTONHANDLERS_H
#define BUTTONHANDLERS_H

#include <windows.h>  // Подключение Windows API типов
#include <QTextEdit>

extern QTextEdit *terminal_down;
extern QTimer *Timer;
extern UCHAR bufOutput[10];
extern DWORD Error;


void handleStartButtonClick();
void preparation();
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

HANDLE OpenDeviceByIndex(DWORD Index, PDWORD pError);
void SI_clear_array(UCHAR Device, UCHAR ChanNumber);
void SI_pusk(UCHAR Device, UCHAR ChanNumber, UCHAR Mode, UCHAR Parity, UCHAR Freq);
void SI_stop(UCHAR Device, UCHAR ChanNumber);
void SO_stop(UCHAR Device);


#endif // BUTTONHANDLERS_H
