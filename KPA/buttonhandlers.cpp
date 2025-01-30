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
QTimer* timer2 = new QTimer();
UCHAR bufOutput[10] = {0};
DWORD Error = 0;

QIcon createCircleIcon(const QColor &color) {
    QPixmap pixmap(16, 16);

    pixmap.fill(Qt::transparent);  // Прозрачный фон

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);  // Сглаживание для круга
    painter.setBrush(Qt::red);  // Устанавливаем цвет заливки
    painter.setPen(Qt::NoPen);  // Без обводки
    painter.drawEllipse(0, 0, 16, 16);  // Рисуем круг

    return QIcon(pixmap);
}

void updateOperationStatus(int row, bool success) {
    if (checking_the_operation) {
        QTableWidgetItem* item = checking_the_operation->item(row, 2);
        if (!item) {
            item = new QTableWidgetItem();
            checking_the_operation->setItem(row, 2, item);
        }
        if (success) {
            item->setText("Выполнено");
            item->setBackground(QColor(0, 128, 0)); // Зеленый цвет
        } else {
            item->setText("Не выполнено");
            item->setBackground(QColor(255, 0, 0)); // Красный цвет
        }
    }
}

void handleStartButtonClick()
{
    // Переменная для вывода серийного номера и прочих сообщений
    QString s;

    // Если кнопка "Старт" ещё не нажата
    if (!clicledStartbutton)
    {
        bool isDeviceConnected = false; // Флаг, что хотя бы одно устройство подключилось

        // --- Подсвечиваем "Гот ТЕРМ." синим на 1 секунду ---
        // Делается асинхронно через singleShot, чтобы не блокировать GUI
        if (turning_on_the_equipment && turning_on_the_equipment->item(0, 1))
        {
            // Устанавливаем синий цвет
            turning_on_the_equipment->item(0, 1)->setBackground(QColor(0, 0, 255));

            // Через 1 секунду возвращаем белый цвет
            QTimer::singleShot(1000, [=]() {
                if (turning_on_the_equipment && turning_on_the_equipment->item(0, 1))
                {
                    turning_on_the_equipment->item(0, 1)->setBackground(Qt::white);
                }
            });
        }

        // --- Подключаемся к устройству ARINC429 для CH1 ---
        // Если ещё не подключено
        if (!State_ECE0206_0)
        {
            // Пытаемся открыть устройство
            hECE0206_0 = OpenDeviceByIndex(0, &Error);
            if (hECE0206_0 == INVALID_HANDLE_VALUE)
            {
                State_ECE0206_0 = false;
                // Красная иконка на toolButton_14
                if (toolButton_14)
                {
                    toolButton_14->setIcon(createCircleIcon(Qt::red));
                }
                // Подсвечиваем "Включить тумблер СПС" красным, так как не подключено
                if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                {
                    turning_on_the_equipment->item(1, 0)->setBackground(QColor(255, 0, 0));
                }
            }
            else
            {
                // Успешное подключение — настраиваем устройство
                DeviceIoControl(hECE0206_0, ECE02061_XP_SET_LONG_MODE, nullptr, 0, nullptr, 0, &nOutput, nullptr);
                DeviceIoControl(hECE0206_0, ECE02061_XP_GET_SERIAL_NUMBER, nullptr, 0, &bufOutput, 10, &nOutput, nullptr);
                s = "ARINC429_CH1  S\\N: " + QString::fromUtf8(reinterpret_cast<const char*>(bufOutput), 5);

                SI_clear_array(0, 1);
                SI_pusk(0, 1, 0, 1, 0);

                State_ECE0206_0 = true; // Флаг «подключено»
                // Зелёная иконка на toolButton_14
                if (toolButton_14)
                {
                    toolButton_14->setIcon(createCircleIcon(Qt::green));
                }
                // Сбрасываем красную подсветку, если была
                if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                {
                    turning_on_the_equipment->item(1, 0)->setBackground(Qt::white);
                }
                // Подсвечиваем "СПС" синим при успешном подключении
                if (turning_on_the_equipment && turning_on_the_equipment->item(1, 1))
                {
                    turning_on_the_equipment->item(1, 1)->setBackground(QColor(0, 0, 255));
                }
                isDeviceConnected = true;
            }
        }

        // --- Подключаемся к устройству ARINC429 для CH2 ---
        // Здесь убираем задержку в 2 секунды, делаем проверку сразу
        if (!State_ECE0206_1)
        {
            hECE0206_1 = OpenDeviceByIndex(1, &Error);
            if (hECE0206_1 == INVALID_HANDLE_VALUE)
            {
                State_ECE0206_1 = false;
                // Красная иконка на toolButton_15
                if (toolButton_15)
                {
                    toolButton_15->setIcon(createCircleIcon(Qt::red));
                }
                // Подсвечиваем "Включить тумблер СПС" красным, так как не подключено
                if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                {
                    turning_on_the_equipment->item(1, 0)->setBackground(QColor(255, 0, 0));
                }
            }
            else
            {
                // Успешное подключение — настраиваем устройство
                DeviceIoControl(hECE0206_1, ECE02061_XP_SET_LONG_MODE, nullptr, 0, nullptr, 0, &nOutput, nullptr);
                DeviceIoControl(hECE0206_1, ECE02061_XP_GET_SERIAL_NUMBER, nullptr, 0, &bufOutput, 10, &nOutput, nullptr);
                s = "ARINC429_CH2  S\\N: " + QString::fromUtf8(reinterpret_cast<const char*>(bufOutput), 5);

                SI_clear_array(1, 2);
                SI_pusk(1, 2, 0, 1, 0);

                State_ECE0206_1 = true; // Флаг «подключено»
                // Зелёная иконка на toolButton_15
                if (toolButton_15)
                {
                    toolButton_15->setIcon(createCircleIcon(Qt::green));
                }
                // Сбрасываем красную подсветку, если была
                if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                {
                    turning_on_the_equipment->item(1, 0)->setBackground(Qt::white);
                }
                // Подсвечиваем "СПС" синим при успешном подключении
                if (turning_on_the_equipment && turning_on_the_equipment->item(1, 1))
                {
                    turning_on_the_equipment->item(1, 1)->setBackground(QColor(0, 0, 255));
                }
                isDeviceConnected = true;
            }
        }

        // --- Если хотя бы одно устройство удалось подключить ---
        if (isDeviceConnected)
        {
            // Меняем состояние на «стартанули»
            clicledStartbutton = true;

            // Настраиваем внешний вид/текст кнопки "Старт"/"Стоп"
            if (handleStartButton)
            {
                handleStartButton->setCheckable(true);
                handleStartButton->setChecked(clicledStartbutton);
                handleStartButton->setText("Стоп");
            }

            // Запускаем периодический опрос с периодом 40 мс
            Timer->start(40);

            // При каждом срабатывании таймера проверяем устройства и обрабатываем данные
            QObject::connect(Timer, &QTimer::timeout, [=]() {
                // Если оба устройства отвалились — останавливаем Timer
                if (!State_ECE0206_0 && !State_ECE0206_1)
                {
                    Timer->stop();
                    isReceivingData = false;
                    // Подсвечиваем красным "Включить тумблер СПС", если всё отключилось
                    if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                    {
                        turning_on_the_equipment->item(1, 0)->setBackground(QColor(255, 0, 0));
                    }
                }
                else
                {
                    // Если устройства активны — вызываем обработку
                    Timer_Event();
                }
            });

            isReceivingData = true;

            // Подсвечиваем "Т1А Гот" синим, пока идёт обмен
            if (turning_on_the_equipment && turning_on_the_equipment->item(2, 0))
            {
                turning_on_the_equipment->item(2, 0)->setBackground(QColor(0, 0, 255));
            }

            // Запускаем ещё один таймер на 120 мс для включения кнопки "Подготовка"
            timerPreparation->setInterval(120);
            timerPreparation->start();
            QObject::connect(timerPreparation, &QTimer::timeout, [=]() {
                if (preparationButton)
                {
                    preparationButton->setEnabled(true);
                }
                if (turning_on_the_equipment && turning_on_the_equipment->item(2, 1))
                {
                    turning_on_the_equipment->item(2, 1)->setBackground(QColor(0, 128, 0));
                }
                timerPreparation->stop();
            });
        }
    }
    else
    {
        // --- Если нажата кнопка "Стоп" ---
        clicledStartbutton = false; // Меняем состояние
        if (handleStartButton)
        {
            handleStartButton->setCheckable(true);
            handleStartButton->setChecked(clicledStartbutton);
            handleStartButton->setText("Старт");
        }

        // Останавливаем все таймеры, останавливаем приём данных
        Timer->stop();
        timerPreparation->stop();
        isReceivingData = false;

        // Закрываем CH1, если был активен
        if (State_ECE0206_0)
        {
            SI_stop(0, 1);
            SO_stop(0);
            CloseHandle(hECE0206_0);
            State_ECE0206_0 = false;
            if (toolButton_14)
            {
                toolButton_14->setIcon(createCircleIcon(Qt::red));
            }
            if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
            {
                turning_on_the_equipment->item(1, 0)->setBackground(QColor(255, 0, 0));
            }
        }

        // Закрываем CH2, если был активен
        if (State_ECE0206_1)
        {
            SI_stop(1, 2);
            SO_stop(1);
            CloseHandle(hECE0206_1);
            State_ECE0206_1 = false;
            if (toolButton_15)
            {
                toolButton_15->setIcon(createCircleIcon(Qt::red));
            }
            if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
            {
                turning_on_the_equipment->item(1, 0)->setBackground(QColor(255, 0, 0));
            }
        }

        // Возвращаем в исходное состояние подсветку полей
        if (turning_on_the_equipment)
        {
            // "Гот ТЕМПЕРАТУРЫ"
            if (turning_on_the_equipment->item(0, 1))
            {
                turning_on_the_equipment->item(0, 1)->setBackground(Qt::white);
            }
            // "СПС"
            if (turning_on_the_equipment->item(1, 1))
            {
                turning_on_the_equipment->item(1, 1)->setBackground(Qt::white);
            }
        }
    }
}

void preparation() {
    clickedPreparation = !clickedPreparation;
    preparationButton->setCheckable(true);
    preparationButton->setChecked(clickedPreparation);

    if (clickedPreparation) {
        OUT_AD9M2[0] |= (0x1 << 9);
    } else {
        OUT_AD9M2[0] &= ~(0x1 << 9);
    }

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);

    BUF256x32_write(0, OUT_AD9M2, 7);
    SO_pusk(0);
}

void handleButtonClick1() {
    // clickedButton1 = !clickedButton1;
    OUT_AD9M2[0] &= ~(0x1 << 16); // Сбрасываем бит
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 16; // Устанавливаем бит

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);

    BUF256x32_write(0, OUT_AD9M2, 7);
    // Проверка статуса выполнения
    updateOperationStatus(0, (QString::number(OUT_AD9M2[0], 16) == "110280"));
}

void handleButtonClick2() {
    // clickedButton2 = !clickedButton2;
    OUT_AD9M2[0] &= ~(0x1 << 16); // Сбрасываем бит
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 16;

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
    // Проверка статуса выполнения
    updateOperationStatus(1, (QString::number(OUT_AD9M2[0], 16) == "110280"));
}

void handleButtonClick3() {
    // clickedButton3 = !clickedButton3;
    if (clickedPreparation) {
        clickedPreparation = false;
        OUT_AD9M2[0] &= ~(0x1 << 9); // Сбрасываем бит 9 для подготовки
        OUT_AD9M2[0] &= ~(0x1 << 16); // Сбрасываем бит 16
        OUT_AD9M2[0] |= (0x1 & clickedButton3) << 16;

        ULONG sum = KS(OUT_AD9M2, 6);
        OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
        BUF256x32_write(0, OUT_AD9M2, 7);

        QTimer* timerButton3 = new QTimer();
        timerButton3->setSingleShot(true);
        timerButton3->setInterval(3000);
        timerButton3->start();

        QObject::connect(timerButton3, &QTimer::timeout, [timerButton3]() {
            clickedPreparation = true;
            OUT_AD9M2[0] |= (0x1 << 9); // Восстанавливаем бит 9 для подготовки
            clickedButton3 = false;

            ULONG sum = KS(OUT_AD9M2, 6);
            OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
            BUF256x32_write(0, OUT_AD9M2, 7);

            timerButton3->stop();
            delete timerButton3;
        });
    }
}

void handleButtonClick4() {
    // clickedButton4 = !clickedButton4;
    OUT_AD9M2[0] &= ~(0x1 << 16); // Сбрасываем бит
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 16; // Устанавливаем бит

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
    // Проверка статуса выполнения
    updateOperationStatus(3, (QString::number(OUT_AD9M2[0], 16) == "110280"));
}

void handleButtonClick5() {
    // clickedButton5 = !clickedButton5;
    OUT_AD9M2[0] &= ~(0x1 << 16); // Сбрасываем бит
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 16; // Устанавливаем бит

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
    // Проверка статуса выполнения
    updateOperationStatus(4, (QString::number(OUT_AD9M2[0], 16) == "110280"));
}

void handleButtonClick6() {
    // clickedButton6 = !clickedButton6;
    OUT_AD9M2[0] &= ~(0x1 << 16); // Сбрасываем бит
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 16; // Устанавливаем бит

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
    // Проверка статуса выполнения
    updateOperationStatus(5, (QString::number(OUT_AD9M2[0], 16) == "110280"));
}

void handleButtonClick7() {
    // clickedButton7 = !clickedButton7;
    OUT_AD9M2[0] &= ~(0x1 << 16); // Сбрасываем бит
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 16; // Устанавливаем бит

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
    // Проверка статуса выполнения
    updateOperationStatus(6, (QString::number(OUT_AD9M2[0], 16) == "110280"));
}

void handleButtonClick8() {
    // clickedButton8 = !clickedButton8;
    OUT_AD9M2[0] &= ~((0x1 << 16) | (0x1 << 11) | (0x1 << 14)); // Сбрасываем все три бита
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 11; // Устанавливаем бит 11

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
}

void handleButtonClick9() {
    // clickedButton9 = !clickedButton9;
    if (clickedPreparation) {
        clickedPreparation = false;
        OUT_AD9M2[0] &= ~((0x1 << 16) | (0x1 << 9)); // Сбрасываем бит 16 и бит 9 для подготовки
        OUT_AD9M2[0] |= (0x1 << 10); // Устанавливаем бит 10

        ULONG sum = KS(OUT_AD9M2, 6);
        OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
        BUF256x32_write(0, OUT_AD9M2, 7);

        QTimer* timerButton9 = new QTimer();
        timerButton9->setSingleShot(true);
        timerButton9->setInterval(3000);
        timerButton9->start();

        QObject::connect(timerButton9, &QTimer::timeout, [timerButton9]() {
            clickedPreparation = true;
            OUT_AD9M2[0] |= (0x1 << 9); // Восстанавливаем бит 9 для подготовки
            OUT_AD9M2[0] |= (0x1 << 14); // Устанавливаем бит 14

            ULONG sum = KS(OUT_AD9M2, 6);
            OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
            BUF256x32_write(0, OUT_AD9M2, 7);

            timerButton9->stop();
            delete timerButton9;
        });
    }
}

void handleButtonClick10() {
    // clickedButton10 = !clickedButton10;
    OUT_AD9M2[0] |= (0x1 << 11); // Устанавливаем бит 11

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
}

void handleButtonClick11() {
    // clickedButton11 = !clickedButton11;
    OUT_AD9M2[0] |= (0x1 << 16); // Устанавливаем бит 16

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
    updateOperationStatus(10, (QString::number(OUT_AD9M2[0], 16) == "110280"));
}

void handleButtonClick12() {
    // clickedButton12 = !clickedButton12;
    OUT_AD9M2[0] &= ~((0x1 << 30) | (0x1 << 27)); // Сбрасываем биты 30 и 27
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 30;
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 27;

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
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
