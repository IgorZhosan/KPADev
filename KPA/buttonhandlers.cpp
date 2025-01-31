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
#include <QShortcut>

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
    QString s;

    // Если кнопка "Старт" ещё не нажата
    if (!clicledStartbutton)
    {
        // --- 1. Сбрасываем фон некоторых ячеек, НЕ трогаем тексты ---
        if (turning_on_the_equipment)
        {
            // Сбрасываем цвет (0,1), (1,0), (1,1), (2,0), (2,1)
            if (auto item = turning_on_the_equipment->item(0, 1))
                item->setBackground(Qt::white);
            if (auto item = turning_on_the_equipment->item(1, 0))
                item->setBackground(Qt::white);
            if (auto item = turning_on_the_equipment->item(1, 1))
                item->setBackground(Qt::white);
            if (auto item = turning_on_the_equipment->item(2, 0))
                item->setBackground(Qt::white);
            if (auto item = turning_on_the_equipment->item(2, 1))
                item->setBackground(Qt::white);

            // Ячейка (2,2) только текст: возвращаем исходную надпись, убираем ошибки
            if (auto item = turning_on_the_equipment->item(2, 2)) {
                item->setText("Тгот = (Допуск (0-15)с)");  // или ваш изначальный текст
                item->setBackground(Qt::white);           // Белый фон (без окраски)
            }
        }

        bool isDeviceConnected = false;
        bool isReceivingData   = false;

        // --- 2. Подсвечиваем "Гот ТЕРМ." (0,1) синим на 1сек ---
        if (turning_on_the_equipment && turning_on_the_equipment->item(0, 1))
        {
            turning_on_the_equipment->item(0, 1)->setBackground(QColor(0, 0, 255));
            QTimer* gotTempTimer = new QTimer();
            gotTempTimer->setSingleShot(true);
            gotTempTimer->setInterval(1000);
            QObject::connect(gotTempTimer, &QTimer::timeout, [=]() {
                if (turning_on_the_equipment && turning_on_the_equipment->item(0, 1))
                    turning_on_the_equipment->item(0, 1)->setBackground(Qt::white);
                gotTempTimer->deleteLater();
            });
            gotTempTimer->start();
        }

        // --- 3. Проверяем CH1 сразу ---
        if (!State_ECE0206_0) {
            hECE0206_0 = OpenDeviceByIndex(0, &Error);
            if (hECE0206_0 == INVALID_HANDLE_VALUE) {
                State_ECE0206_0 = false;
                if (toolButton_14)
                    toolButton_14->setIcon(createCircleIcon(Qt::red));
                if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                    turning_on_the_equipment->item(1, 0)->setBackground(QColor(255, 0, 0));
            } else {
                // Успешно
                DeviceIoControl(hECE0206_0, ECE02061_XP_SET_LONG_MODE, nullptr, 0, nullptr, 0, &nOutput, nullptr);
                DeviceIoControl(hECE0206_0, ECE02061_XP_GET_SERIAL_NUMBER, nullptr, 0, &bufOutput, 10, &nOutput, nullptr);

                s = "ARINC429_CH1  S\\N: " + QString::fromUtf8(reinterpret_cast<const char*>(bufOutput), 5);
                SI_clear_array(0, 1);
                SI_pusk(0, 1, 0, 1, 0);
                State_ECE0206_0 = true;

                if (toolButton_14)
                    toolButton_14->setIcon(createCircleIcon(Qt::green));
                if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                    turning_on_the_equipment->item(1, 0)->setBackground(Qt::white);
                if (turning_on_the_equipment && turning_on_the_equipment->item(1, 1))
                    turning_on_the_equipment->item(1, 1)->setBackground(QColor(0, 0, 255));

                isDeviceConnected = true;
            }
        }

        // --- 4. Создаём таймер на 2с для CH2 ---
        QTimer* timer2 = new QTimer();
        timer2->setSingleShot(true);
        QObject::connect(timer2, &QTimer::timeout, [=, &isDeviceConnected, &s]() mutable {
            if (!State_ECE0206_1) {
                hECE0206_1 = OpenDeviceByIndex(1, &Error);
                if (hECE0206_1 == INVALID_HANDLE_VALUE) {
                    State_ECE0206_1 = false;
                    if (toolButton_15)
                        toolButton_15->setIcon(createCircleIcon(Qt::red));
                    if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                        turning_on_the_equipment->item(1, 0)->setBackground(QColor(255, 0, 0));
                } else {
                    // Успешно
                    DeviceIoControl(hECE0206_1, ECE02061_XP_SET_LONG_MODE, nullptr, 0, nullptr, 0, &nOutput, nullptr);
                    DeviceIoControl(hECE0206_1, ECE02061_XP_GET_SERIAL_NUMBER, nullptr, 0, &bufOutput, 10, &nOutput, nullptr);

                    s = "ARINC429_CH2  S\\N: " + QString::fromUtf8(reinterpret_cast<const char*>(bufOutput), 5);
                    SI_clear_array(1, 2);
                    SI_pusk(1, 2, 0, 1, 0);
                    State_ECE0206_1 = true;

                    if (toolButton_15)
                        toolButton_15->setIcon(createCircleIcon(Qt::green));
                    if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                        turning_on_the_equipment->item(1, 0)->setBackground(Qt::white);
                    if (turning_on_the_equipment && turning_on_the_equipment->item(1, 1))
                        turning_on_the_equipment->item(1, 1)->setBackground(QColor(0, 0, 255));

                    isDeviceConnected = true;
                }
            }
            timer2->deleteLater();
        });
        timer2->start(2000);

        // --- 5. Если CH1 уже открыт -> запускаем обмен ---
        if (isDeviceConnected)
        {
            clicledStartbutton = true;
            if (handleStartButton) {
                handleStartButton->setCheckable(true);
                handleStartButton->setChecked(true);
                handleStartButton->setText("Стоп");
            }

            // Основной таймер (40 мс)
            Timer->start(40);
            QObject::connect(Timer, &QTimer::timeout, [=, &isDeviceConnected, &isReceivingData]() mutable {
                if (!State_ECE0206_0 && !State_ECE0206_1)
                {
                    Timer->stop();
                    isDeviceConnected = false;
                    isReceivingData   = false;
                    if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                        turning_on_the_equipment->item(1, 0)->setBackground(QColor(255, 0, 0));
                }
                else
                {
                    Timer_Event();
                }
            });

            isReceivingData = true;

            // (2,0) "Т1А Гот" — синий, пока идёт обмен
            if (turning_on_the_equipment && turning_on_the_equipment->item(2, 0))
                turning_on_the_equipment->item(2, 0)->setBackground(QColor(0, 0, 255));

            // --- 6. Таймер (120 мс), только чтобы разблокировать «Подготовка» ---
            timerPreparation->setInterval(120);
            timerPreparation->start();
            QObject::connect(timerPreparation, &QTimer::timeout, [=]() {
                if (preparationButton)
                    preparationButton->setEnabled(true);
                // НЕ красим (2,1) здесь! Это будет в preparation().
                timerPreparation->stop();
            });
        }
    }
    else
    {
        // --- Кнопка "Стоп" ---
        clicledStartbutton = false;
        if (handleStartButton)
        {
            handleStartButton->setCheckable(true);
            handleStartButton->setChecked(false);
            handleStartButton->setText("Старт");
        }

        Timer->stop();
        timerPreparation->stop();
        isReceivingData = false;

        // Закрываем CH1
        if (State_ECE0206_0) {
            SI_stop(0, 1);
            SO_stop(0);
            CloseHandle(hECE0206_0);
            State_ECE0206_0 = false;
            if (toolButton_14)
                toolButton_14->setIcon(createCircleIcon(Qt::red));
            if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                turning_on_the_equipment->item(1, 0)->setBackground(QColor(255, 0, 0));
            if (turning_on_the_equipment && turning_on_the_equipment->item(1, 1))
                turning_on_the_equipment->item(1, 1)->setBackground(Qt::white);
        }

        // Закрываем CH2
        if (State_ECE0206_1) {
            SI_stop(1, 2);
            SO_stop(1);
            CloseHandle(hECE0206_1);
            State_ECE0206_1 = false;
            if (toolButton_15)
                toolButton_15->setIcon(createCircleIcon(Qt::red));
            if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                turning_on_the_equipment->item(1, 0)->setBackground(QColor(255, 0, 0));
            if (turning_on_the_equipment && turning_on_the_equipment->item(1, 1))
                turning_on_the_equipment->item(1, 1)->setBackground(Qt::white);
        }

        // При новом «Старт» убираем старые ошибки:
        if (turning_on_the_equipment)
        {
            if (auto item = turning_on_the_equipment->item(0, 1))
                item->setBackground(Qt::white);
            if (auto item = turning_on_the_equipment->item(2, 0))
                item->setBackground(Qt::white);
            if (auto item = turning_on_the_equipment->item(2, 1))
                item->setBackground(Qt::white);

            // (2,2) возвращаем исходный текст
            if (auto item = turning_on_the_equipment->item(2, 2)) {
                item->setText("Тгот = (Допуск (0-15)с)"); // или изначальная надпись
                item->setBackground(Qt::white);
            }
        }
    }
}

static QTimer* readinessTimer = nullptr;  // Один таймер на всё время, глобально/статически

void preparation(bool checked)
{
    // Если таймер ещё не создан — создаём
    if (!readinessTimer) {
        readinessTimer = new QTimer();
        readinessTimer->setSingleShot(true); // одноразовый
    }

    if (checked)
    {
        // --- Кнопка «Подготовка» нажата (checked = true) ---
        // Ставим бит 9
        OUT_AD9M2[0] |= (1UL << 9);

        // Подсчитываем КС и отправляем
        ULONG sum = KS(OUT_AD9M2, 6);
        OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
        BUF256x32_write(0, OUT_AD9M2, 7);
        SO_pusk(0);

        // Настраиваем 20-секундный таймер
        readinessTimer->stop();
        readinessTimer->setInterval(20000);

        // Удаляем предыдущие connect, чтобы не было дублирующих сигналов
        QObject::disconnect(readinessTimer, nullptr, nullptr, nullptr);

        // Подключаем логику по истечении таймера
        QObject::connect(readinessTimer, &QTimer::timeout, [=]() {
            // (1) Считываем свежие данные IN_KPA
            receiveDataAndDisplay();

            // (2) Берём биты (28, 27..24, DM=30)
            unsigned long val2 = IN_KPA[2] & 0x7FFFFFFF;
            unsigned long val0 = IN_KPA[0] & 0x7FFFFFFF;

            bool bit28 = (val2 & (1UL << 28));
            bool bit27 = (val2 & (1UL << 27));
            bool bit26 = (val2 & (1UL << 26));
            bool bit25 = (val2 & (1UL << 25));
            bool bit24 = (val2 & (1UL << 24));
            bool bit30 = (val0 & (1UL << 30)); // DM

            QStringList errorList;
            if (bit27) errorList << "ДГ";
            if (bit26) errorList << "KRL";
            if (bit25) errorList << "Курс ДВ";
            if (bit24) errorList << "Танг ДВ";
            if (!bit30) {
                errorList << "DM";
            }

            // (3) Ячейки (2,1) — «Т1А ГОТ» и (2,2) — текст
            QTableWidgetItem* gotItem = turning_on_the_equipment->item(2, 1);
            if (!gotItem) {
                gotItem = new QTableWidgetItem("Т1А Гот");
                turning_on_the_equipment->setItem(2, 1, gotItem);
            }
            QTableWidgetItem* errItem = turning_on_the_equipment->item(2, 2);
            if (!errItem) {
                errItem = new QTableWidgetItem("Тгот = (Допуск (0-15)с)");
                turning_on_the_equipment->setItem(2, 2, errItem);
            }
            errItem->setBackground(Qt::white);

            // (4) Логика
            if (bit28)
            {
                if (errorList.isEmpty())
                {
                    // Нет ошибок
                    gotItem->setBackground(QColor(0, 128, 0)); // зелёный
                    errItem->setText("Готов (бит 28)");
                }
                else
                {
                    // Ошибки
                    gotItem->setBackground(QColor(255, 0, 0)); // красный
                    errItem->setText("Готов, но ошибки: " + errorList.join(", "));
                }
            }
            else
            {
                // бит 28 = 0 -> не готов
                gotItem->setBackground(QColor(255, 0, 0));
                QString baseTxt = "Тгот = (Допуск (0-15)с)";
                if (!errorList.isEmpty()) {
                    baseTxt += "; Ошибки: " + errorList.join(", ");
                }
                errItem->setText(baseTxt);
            }
        });

        readinessTimer->start();
    }
    else
    {
        // --- Кнопка «Подготовка» отжата (checked = false) ---
        // Останавливаем таймер (если шёл)
        if (readinessTimer->isActive()) {
            readinessTimer->stop();
        }

        // Снимаем бит 9
        OUT_AD9M2[0] &= ~(1UL << 9);
        ULONG sum = KS(OUT_AD9M2, 6);
        OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
        BUF256x32_write(0, OUT_AD9M2, 7);
        SO_pusk(0);

        // Сбрасываем «Т1А ГОТ» (2,1) и (2,2)
        if (QTableWidgetItem* gotItem = turning_on_the_equipment->item(2, 1)) {
            gotItem->setBackground(Qt::white);
        }
        if (QTableWidgetItem* errItem = turning_on_the_equipment->item(2, 2)) {
            errItem->setText("Тгот = (Допуск (0-15)с)");
            errItem->setBackground(Qt::white);
        }
    }
}

void handleButtonClick1() {
    // clickedButton1 = !clickedButton1;
    OUT_AD9M2[0] &= ~(0x1 << 16); // Сбрасываем бит
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 16; // Устанавливаем бит

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);

    BUF256x32_write(0, OUT_AD9M2, 7);
    // Проверка статуса выполнения
  //  updateOperationStatus(0, (QString::number(OUT_AD9M2[0], 16) == "110280"));
}

void handleButtonClick2() {
    // clickedButton2 = !clickedButton2;
    OUT_AD9M2[0] &= ~(0x1 << 16); // Сбрасываем бит
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 16;

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
    // Проверка статуса выполнения
  //  updateOperationStatus(1, (QString::number(OUT_AD9M2[0], 16) == "110280"));
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
  //  updateOperationStatus(3, (QString::number(OUT_AD9M2[0], 16) == "110280"));
}

void handleButtonClick5() {
    // clickedButton5 = !clickedButton5;
    OUT_AD9M2[0] &= ~(0x1 << 16); // Сбрасываем бит
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 16; // Устанавливаем бит

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
    // Проверка статуса выполнения
   // updateOperationStatus(4, (QString::number(OUT_AD9M2[0], 16) == "110280"));
}

void handleButtonClick6() {
    // clickedButton6 = !clickedButton6;
    OUT_AD9M2[0] &= ~(0x1 << 16); // Сбрасываем бит
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 16; // Устанавливаем бит

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
    // Проверка статуса выполнения
  //  updateOperationStatus(5, (QString::number(OUT_AD9M2[0], 16) == "110280"));
}

void handleButtonClick7() {
    // clickedButton7 = !clickedButton7;
    OUT_AD9M2[0] &= ~(0x1 << 16); // Сбрасываем бит
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 16; // Устанавливаем бит

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
    // Проверка статуса выполнения
   // updateOperationStatus(6, (QString::number(OUT_AD9M2[0], 16) == "110280"));
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
