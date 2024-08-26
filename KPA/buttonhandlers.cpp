#include "buttonhandlers.h"
#include "data_receiver.h"
#include "library02061.h"
#include "intrfECE0206.h"
#include <QDebug>
#include <QTimer>
#include <QTextEdit>


extern HANDLE hECE0206_0;
extern HANDLE hECE0206_1;
extern DWORD nOutput;
extern UCHAR bufOutput[10];
extern DWORD Error;
extern QTextEdit *terminal_down;
extern QTimer *Timer;

bool State_ECE0206_0 = false;
bool State_ECE0206_1 = false;
bool isReceivingData = false;

QTimer *Timer = new QTimer();
UCHAR bufOutput[10] = {0};
DWORD Error = 0;


void handleStartButtonClick()
{
    QString s;
    if (!isReceivingData) {  // Если данные еще не отправляются
        // Проверка подключения к устройству ARINC429 для CH1
        if (State_ECE0206_0 == false)
        {
            hECE0206_0 = OpenDeviceByIndex(0, &Error);
            if (hECE0206_0 == INVALID_HANDLE_VALUE)
            {
                terminal_down->append("Состояние CH1: ОШИБКА ПОДКЛ. ECE-0206-1C-S");
                State_ECE0206_0 = false;
            }
            else
            {
                DeviceIoControl(hECE0206_0, ECE02061_XP_SET_LONG_MODE, NULL, 0, NULL, 0, &nOutput, NULL);
                DeviceIoControl(hECE0206_0, ECE02061_XP_GET_SERIAL_NUMBER, NULL, 0, &bufOutput, 10, &nOutput, NULL);
                s = "ARINC429_CH1  S\\N: " + QString::fromUtf8(reinterpret_cast<const char*>(bufOutput), 5);
                terminal_down->append(s);

                SI_clear_array(0, 1);   // очистка буфера приемника 1 канала
                SI_pusk(0, 1, 0, 1, 0); // канал 1, рабочий режим, контроль четности, прием на частотах 36-100КГц
                terminal_down->append("Состояние CH1: ОЖИДАНИЕ");
                State_ECE0206_0 = true;
            }
        }

        // Проверка подключения к устройству ARINC429 для CH2
        if (State_ECE0206_1 == false)
        {
            hECE0206_1 = OpenDeviceByIndex(1, &Error);
            if (hECE0206_1 == INVALID_HANDLE_VALUE)
            {
                terminal_down->append("Состояние CH2: ОШИБКА ПОДКЛ. ECE-0206-1C-S");
                State_ECE0206_1 = false;
            }
            else
            {
                DeviceIoControl(hECE0206_1, ECE02061_XP_SET_LONG_MODE, NULL, 0, NULL, 0, &nOutput, NULL);
                DeviceIoControl(hECE0206_1, ECE02061_XP_GET_SERIAL_NUMBER, NULL, 0, &bufOutput, 10, &nOutput, NULL);
                s = "ARINC429_CH2  S\\N: " + QString::fromUtf8(reinterpret_cast<const char*>(bufOutput), 5);

                terminal_down->append(s);

                SI_clear_array(1, 2);   // очистка буфера приемника 2 канала
                SI_pusk(1, 2, 0, 1, 0); // канал 2, рабочий режим, контроль четности, прием на частотах 36-100КГц
                terminal_down->append("Состояние CH2: ОЖИДАНИЕ");
                State_ECE0206_1 = true;
            }
        }

        // Если хотя бы одно устройство подключено, начинаем отправку данных
        if (State_ECE0206_0 == true || State_ECE0206_1 == true) {
            Timer->start(40);  // Запуск таймера с интервалом в 1000 мс (1 секунда)
            QObject::connect(Timer, &QTimer::timeout, &ifCheckBoxesIsTrue);  // Подключаем к таймеру функцию ifCheckBoxesIsTrue
            terminal_down->append("Данные отправляются...");
            isReceivingData = true;
        }
    }
    else {  // Если данные уже отправляются, остановить процесс
        Timer->stop();
        terminal_down->append("Процесс получения данных остановлен.");
        isReceivingData = false;

        // Остановка каналов и закрытие устройств
        if (State_ECE0206_0) {
            SI_stop(0, 1);
            SO_stop(0);
            CloseHandle(hECE0206_0);
            terminal_down->append("Состояние CH1: НЕ ПОДКЛЮЧЕН");
            State_ECE0206_0 = false;
        }

        if (State_ECE0206_1) {
            SI_stop(1, 2);
            SO_stop(1);
            CloseHandle(hECE0206_1);
            terminal_down->append("Состояние CH2: НЕ ПОДКЛЮЧЕН");
            State_ECE0206_1 = false;
        }
    }
}

void preparation() {

}

void handleButtonClick1() {

}

void handleButtonClick2() {
    terminal_down -> append("Кнопка 2 нажата");
}

void handleButtonClick3() {
    qDebug() << "Кнопка 3 нажата";
}

void handleButtonClick4() {
    qDebug() << "Кнопка 4 нажата";
}

void handleButtonClick5() {
    qDebug() << "Кнопка 5 нажата";
}

void handleButtonClick6() {
    qDebug() << "Кнопка 6 нажата";
}

void handleButtonClick7() {
    qDebug() << "Кнопка 7 нажата";
}

void handleButtonClick8() {
    qDebug() << "Кнопка 8 нажата";
}

void handleButtonClick9() {
    qDebug() << "Кнопка 9 нажата";
}

void handleButtonClick10() {
    qDebug() << "Кнопка 10 нажата";
}

void handleButtonClick11() {
    qDebug() << "Кнопка 11 нажата";
}

void handleButtonClick12() {
    qDebug() << "Кнопка 12 нажата";
}

void handleButtonClick13() {
    qDebug() << "Кнопка 13 нажата";
}

void handleButtonClick14() {
    qDebug() << "Кнопка 14 нажата";
}

void handleButtonClick15() {
    qDebug() << "Кнопка 15 нажата";
}
