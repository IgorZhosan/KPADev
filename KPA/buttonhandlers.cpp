#include "buttonhandlers.h"
#include "data_receiver.h"
#include "library02061.h"
#include "intrfECE0206.h"
#include <QDebug>
#include <QTimer>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QIcon>
#include <QPixmap>
#include <QPainter>


bool State_ECE0206_0 = false;
bool State_ECE0206_1 = false;
bool isReceivingData = false;
bool isTerminalPause = false;
bool clickedPreparation = false;

bool clicledStartbutton = false;
bool clickedButton1 = false;
bool clickedButton2 = false;
bool clickedButton3 = false;
bool clickedButton4 = false;
bool clickedButton5 = false;
bool clickedButton6 = false;
bool clickedButton7 = false;
bool clickedButton8 = false;
bool clickedButton9 = false;
bool clickedButton10 = false;
bool clickedButton11 = false;
bool clickedButton12 = false;
bool clickedButton13 = false;
bool clickedButton14 = false;
bool clickedButton15 = false;

QTimer *Timer = new QTimer();
QTimer *timerPreparation = new QTimer();
UCHAR bufOutput[10] = {0};
DWORD Error = 0;

QIcon createCircleIcon(const QColor &color, int diameter = 16) {
    QPixmap pixmap(diameter, diameter);
    pixmap.fill(Qt::transparent);  // Прозрачный фон

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);  // Сглаживание для круга
    painter.setBrush(color);  // Устанавливаем цвет заливки
    painter.setPen(Qt::NoPen);  // Без обводки
    painter.drawEllipse(0, 0, diameter, diameter);  // Рисуем круг

    return QIcon(pixmap);
}

void handleStartButtonClick() {
    QString s;
    clicledStartbutton = !clicledStartbutton;
    handleStartButton->setCheckable(true);
    handleStartButton->setChecked(clicledStartbutton);
    handleStartButton->setText("Стоп");

    if (!isReceivingData) {  // Если данные еще не отправляются
        // Проверка подключения к устройству ARINC429 для CH1
        if (State_ECE0206_0 == false)
        {
            hECE0206_0 = OpenDeviceByIndex(0, &Error);
            if (hECE0206_0 == INVALID_HANDLE_VALUE)
            {
              //  terminal_down->append("Состояние CH1: ОШИБКА ПОДКЛ. ECE-0206-1C-S");
                State_ECE0206_0 = false;
                toolButton_14->setIcon(createCircleIcon(Qt::red));
            }
            else
            {
                DeviceIoControl(hECE0206_0, ECE02061_XP_SET_LONG_MODE, NULL, 0, NULL, 0, &nOutput, NULL);
                DeviceIoControl(hECE0206_0, ECE02061_XP_GET_SERIAL_NUMBER, NULL, 0, &bufOutput, 10, &nOutput, NULL);
                s = "ARINC429_CH1  S\\N: " + QString::fromUtf8(reinterpret_cast<const char*>(bufOutput), 5);
              //  terminal_down->append(s);

                SI_clear_array(0, 1);   // очистка буфера приемника 1 канала
                SI_pusk(0, 1, 0, 1, 0); // канал 1, рабочий режим, контроль четности, прием на частотах 36-100КГц
              //  terminal_down->append("Состояние CH1: ОЖИДАНИЕ");
                State_ECE0206_0 = true;
                toolButton_14->setIcon(createCircleIcon(Qt::green));
            }
        }

        // Проверка подключения к устройству ARINC429 для CH2
        if (State_ECE0206_1 == false)
        {
            hECE0206_1 = OpenDeviceByIndex(1, &Error);
            if (hECE0206_1 == INVALID_HANDLE_VALUE)
            {
              //  terminal_down->append("Состояние CH2: ОШИБКА ПОДКЛ. ECE-0206-1C-S");
                State_ECE0206_1 = false;
                toolButton_15->setIcon(createCircleIcon(Qt::red));
            }
            else
            {
                DeviceIoControl(hECE0206_1, ECE02061_XP_SET_LONG_MODE, NULL, 0, NULL, 0, &nOutput, NULL);
                DeviceIoControl(hECE0206_1, ECE02061_XP_GET_SERIAL_NUMBER, NULL, 0, &bufOutput, 10, &nOutput, NULL);
                s = "ARINC429_CH2  S\\N: " + QString::fromUtf8(reinterpret_cast<const char*>(bufOutput), 5);
              //  terminal_down->append(s);r
                SI_clear_array(1, 2);   // очистка буфера приемника 2 канала
                SI_pusk(1, 2, 0, 1, 0); // канал 2, рабочий режим, контроль четности, прием на частотах 36-100КГц
              //  terminal_down->append("Состояние CH2: ОЖИДАНИЕ");
                State_ECE0206_1 = true;
                toolButton_15->setIcon(createCircleIcon(Qt::green));
            }
        }
        // Если хотя бы одно устройство подключено, начинаем отправку данных
        if (State_ECE0206_0 == true || State_ECE0206_1 == true) {
            Timer->start(40);
            QObject::connect(Timer, &QTimer::timeout, &Timer_Event);
           // terminal_down->append("Данные отправляются...");
            isReceivingData = true;

            timerPreparation->setInterval(120);
            timerPreparation->start();
            QObject::connect(timerPreparation, &QTimer::timeout, []() {
                preparationButton->setEnabled(true);
               turning_on_the_equipment->item(2, 1)->setBackground(QColor(0, 128, 0));
              //  item->setBackground(QColor(0, 128, 0));
                timerPreparation -> stop();
            });
        }
    }
    else {  // Если данные уже отправляются, остановить процесс
        Timer->stop();
        timerPreparation->stop();
        terminal_down->append("Процесс получения данных остановлен.");
        isReceivingData = false;

        // Остановка каналов и закрытие устройств
        if (State_ECE0206_0) {
            SI_stop(0, 1);
            SO_stop(0);
            CloseHandle(hECE0206_0);
           // terminal_down->append("Состояние CH1: НЕ ПОДКЛЮЧЕН");
            State_ECE0206_0 = false;
            toolButton_14->setIcon(createCircleIcon(Qt::red));
        }

        if (State_ECE0206_1) {
            SI_stop(1, 2);
            SO_stop(1);
            CloseHandle(hECE0206_1);
           // terminal_down->append("Состояние CH2: НЕ ПОДКЛЮЧЕН");
            State_ECE0206_1 = false;
            toolButton_15->setIcon(createCircleIcon(Qt::red));
        }
    handleStartButton -> setText("Старт");
    }
}

void preparation() {
    clickedPreparation = !clickedPreparation;
    preparationButton->setCheckable(true);
    preparationButton->setChecked(clickedPreparation);
}

void handleButtonClick1() {
    clickedButton1 = !clickedButton1;
   // button1->setCheckable(true);
   // button1->setChecked(clickedButton1);
}

void handleButtonClick2() {
    clickedButton2 = !clickedButton2;
   // button2->setCheckable(true);
   // button2->setChecked(clickedButton2);
}

void handleButtonClick3() {
    clickedButton3 = !clickedButton3;
   // button3->setCheckable(true);
   // button3->setChecked(clickedButton3);
}

void handleButtonClick4() {
    clickedButton4 = !clickedButton4;
   // button4->setCheckable(true);
    //button4->setChecked(clickedButton4);
}

void handleButtonClick5() {
    clickedButton5 = !clickedButton5;
   // button5->setCheckable(true);
   // button5->setChecked(clickedButton5);
}

void handleButtonClick6() {
    clickedButton6 = !clickedButton6;
   // button6->setCheckable(true);
   // button6->setChecked(clickedButton6);
}

void handleButtonClick7() {
    clickedButton7 = !clickedButton7;
   // button7->setCheckable(true);
   // button7->setChecked(clickedButton7);
}

void handleButtonClick8() {
    clickedButton8 = !clickedButton8;
   // button8->setCheckable(true);
   // button8->setChecked(clickedButton8);
}

void handleButtonClick9() {
    clickedButton9 = !clickedButton9;
   // button9->setCheckable(true);
   // button9->setChecked(clickedButton9);
}

void handleButtonClick10() {
    clickedButton10 = !clickedButton10;
   // button10->setCheckable(true);
   // button10->setChecked(clickedButton10);
}

void handleButtonClick11() {
    clickedButton11 = !clickedButton11;
   // button11->setCheckable(true);
   // button11->setChecked(clickedButton11);
}

void handleButtonClick12() {
    clickedButton12 = !clickedButton12;
   // button12->setCheckable(true);
   // button12->setChecked(clickedButton12);
}

void handleButtonClick13() {
    clickedButton13 = !clickedButton13;
   // button13->setCheckable(true);
   // button13->setChecked(clickedButton13);
}

void handleButtonClick14() {
    clickedButton14 = !clickedButton14;
   // button14->setCheckable(true);
   // button14->setChecked(clickedButton14);
}

void handleButtonClick15() {
    clickedButton15 = !clickedButton15;
   // button15->setCheckable(true);
   // button15->setChecked(clickedButton15);
}

void handleClearButton() {
    terminal_down->clear();
}

void on_pushButton_3_clicked() {
    isTerminalPause = !isTerminalPause;
    pushButton_3->setCheckable(true);
    pushButton_3->setChecked(isTerminalPause);
}
