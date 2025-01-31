#include "buttonhandlers.h"
#include "data_receiver.h"
#include "library02061.h"
#include "intrfECE0206.h"
#include <QDebug>
#include <QTimer>
#include <QTime>
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
static QTimer* readinessTimer = nullptr;  // Таймер для периодического опроса
static QTime   prepStartTime;            // Когда нажали «Подготовка»
static QTime   deadlineTime;             // Момент, до которого ждём, обычно +20 c
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

void resetOutAD9M2PreservingLiters()
{
    // 1) Сохраняем биты литер
    ULONG savedBit20 = OUT_AD9M2[0] & (1UL << 20);       // бит 20
    ULONG savedLitersIn1 = OUT_AD9M2[1] & 0xFFFFFE00UL;  // биты [9..31]

    // 2) Возвращаем весь OUT_AD9M2[] к дефолту
    OUT_AD9M2[0] = 0x80;
    OUT_AD9M2[1] = 0x40;
    OUT_AD9M2[2] = 0xC0;
    OUT_AD9M2[3] = 0x20;
    OUT_AD9M2[4] = 0xA0;
    OUT_AD9M2[5] = 0x60;
    OUT_AD9M2[6] = 0xE5;

    // 3) Восстанавливаем сохранённые биты литер
    OUT_AD9M2[0] |= savedBit20;
    OUT_AD9M2[1] |= savedLitersIn1;

    // 4) Считаем контрольную сумму и отправляем
    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);

    BUF256x32_write(0, OUT_AD9M2, 7);
    SO_pusk(0);
}

// (B) Сбрасываем флаги clickedButton1..15
void resetAllClickedButtons()
{
    clickedButton1 = false;
    clickedButton2 = false;
    clickedButton3 = false;
    clickedButton4 = false;
    clickedButton5 = false;
    clickedButton6 = false;
    clickedButton7 = false;
    clickedButton8 = false;
    clickedButton9 = false;
    clickedButton10 = false;
    clickedButton11 = false;
    clickedButton12 = false;
    clickedButton13 = false;
    clickedButton14 = false;
    clickedButton15 = false;
}

void handleStartButtonClick()
{
    QString s;

    // Если кнопка "Старт" ещё не нажата
    if (!clicledStartbutton)
    {
        // --- 1. Сбрасываем фон, не трогаем тексты ---
        if (turning_on_the_equipment)
        {
            // Ячейки (0,1), (1,0), (1,1), (2,0), (2,1), (2,2) -> белый
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

            if (auto item = turning_on_the_equipment->item(2, 2)) {
                item->setText("Тгот = (Допуск (0-15)с)");
                item->setBackground(Qt::white);
            }
        }

        bool isDeviceConnected = false;
        bool isReceivingData   = false;

        // --- 2. Подсвечиваем "Гот ТЕРМ." (0,1) синим на 1с ---
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

        // --- 3. Проверяем CH1 ---
        if (!State_ECE0206_0) {
            hECE0206_0 = OpenDeviceByIndex(0, &Error);
            if (hECE0206_0 == INVALID_HANDLE_VALUE) {
                State_ECE0206_0 = false;
                if (toolButton_14)
                    toolButton_14->setIcon(createCircleIcon(Qt::red));
                if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                    turning_on_the_equipment->item(1, 0)->setBackground(QColor(255, 0, 0));
            } else {
                DeviceIoControl(hECE0206_0, ECE02061_XP_SET_LONG_MODE,
                                nullptr, 0, nullptr, 0, &nOutput, nullptr);
                DeviceIoControl(hECE0206_0, ECE02061_XP_GET_SERIAL_NUMBER,
                                nullptr, 0, &bufOutput, 10, &nOutput, nullptr);

                s = "ARINC429_CH1  S\\N: " +
                    QString::fromUtf8(reinterpret_cast<const char*>(bufOutput), 5);
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

        // --- 4. Таймер 2с для CH2 ---
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
                    DeviceIoControl(hECE0206_1, ECE02061_XP_SET_LONG_MODE,
                                    nullptr, 0, nullptr, 0, &nOutput, nullptr);
                    DeviceIoControl(hECE0206_1, ECE02061_XP_GET_SERIAL_NUMBER,
                                    nullptr, 0, &bufOutput, 10, &nOutput, nullptr);

                    s = "ARINC429_CH2  S\\N: " +
                        QString::fromUtf8(reinterpret_cast<const char*>(bufOutput), 5);
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

        // --- 5. Запуск обмена, если CH1 ок ---
        if (isDeviceConnected)
        {
            clicledStartbutton = true;
            if (handleStartButton) {
                handleStartButton->setCheckable(true);
                handleStartButton->setChecked(true);
                handleStartButton->setText("Стоп");
            }

            // Основной таймер 40мс
            Timer->start(40);
            QObject::connect(Timer, &QTimer::timeout,
                             [=, &isDeviceConnected, &isReceivingData]() mutable {
                                 if (!State_ECE0206_0 && !State_ECE0206_1) {
                                     Timer->stop();
                                     isDeviceConnected = false;
                                     isReceivingData   = false;
                                     if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                                         turning_on_the_equipment->item(1, 0)->setBackground(QColor(255, 0, 0));
                                 } else {
                                     Timer_Event();
                                 }
                             });

            isReceivingData = true;

            // (2,0) "Т1А ГОТ" — синий
            if (turning_on_the_equipment && turning_on_the_equipment->item(2, 0))
                turning_on_the_equipment->item(2, 0)->setBackground(QColor(0, 0, 255));

            // Разблокируем «Подготовка» через 120 мс
            timerPreparation->setInterval(120);
            timerPreparation->start();
            QObject::connect(timerPreparation, &QTimer::timeout,
                             [=]() {
                                 if (preparationButton)
                                     preparationButton->setEnabled(true);
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

        if (preparationButton)
        {
            if (preparationButton->isChecked())
            {
                // Снимем флаг checked
                preparationButton->setChecked(false);

                // Вызовем слот, который отрабатывает "Подготовка off"
                // (Убедитесь, что вы имеете доступ к функции preparation(bool)
                //  или подходящий connect)
                preparation(false);
            }
        }

        // (I) Сбрасываем OUT_AD9M2, кроме литер
        resetOutAD9M2PreservingLiters();

        // (II) Сбрасываем "clickedButton1..15"
        resetAllClickedButtons();



        // (III) Убираем старые ошибки из GUI
        if (turning_on_the_equipment)
        {
            if (auto item = turning_on_the_equipment->item(0, 1))
                item->setBackground(Qt::white);
            if (auto item = turning_on_the_equipment->item(2, 0))
                item->setBackground(Qt::white);
            if (auto item = turning_on_the_equipment->item(2, 1))
                item->setBackground(Qt::white);

            if (auto item = turning_on_the_equipment->item(2, 2)) {
                item->setText("Тгот = (Допуск (0-15)с)");
                item->setBackground(Qt::white);
            }
        }


    }
}

void preparation(bool checked)
{
    // 1) Синхронизируем clickedPreparation (для handleButtonClickN)
    clickedPreparation = checked;

    // Если ещё не создан readinessTimer — создаём
    if (!readinessTimer) {
        readinessTimer = new QTimer();
        // Периодический опрос:
        readinessTimer->setSingleShot(false);
    }

    if (checked)
    {
        // --- «Подготовка» включена (On) ---

        // (A) Ставим бит 9
        OUT_AD9M2[0] |= (1UL << 9);

        ULONG sum = KS(OUT_AD9M2, 6);
        OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
        BUF256x32_write(0, OUT_AD9M2, 7);
        SO_pusk(0);

        // (B) Запоминаем текущее время + крайний срок (20 с)
        prepStartTime = QTime::currentTime();
        deadlineTime  = prepStartTime.addSecs(20);

        // (C) Настраиваем период опроса 200 мс
        readinessTimer->stop();
        readinessTimer->setInterval(200);

        // (D) Отключаем старые connect
        QObject::disconnect(readinessTimer, nullptr, nullptr, nullptr);

        // (E) Подключаем новый обработчик
        QObject::connect(readinessTimer, &QTimer::timeout, [=]() {
            // 1) Каждые 200 мс считываем свежие данные
            receiveDataAndDisplay();

            // 2) Смотрим текущее время
            QTime now = QTime::currentTime();

            // 3) Если вышли 20 с — финальная проверка
            if (now >= deadlineTime)
            {
                // Останавливаем таймер
                readinessTimer->stop();

                // Ещё раз читаем биты (на случай, что bit28 пришёл ровно на 20‑й c)
                unsigned long val2 = IN_KPA[2] & 0x7FFFFFFF;
                unsigned long val0 = IN_KPA[0] & 0x7FFFFFFF;

                bool bit28 = (val2 & (1UL << 28));
                bool bit27 = (val2 & (1UL << 27));
                bool bit26 = (val2 & (1UL << 26));
                bool bit25 = (val2 & (1UL << 25));
                bool bit24 = (val2 & (1UL << 24));
                bool bit30 = (val0 & (1UL << 30));

                // Соберём ошибки
                QStringList errorList;
                if (bit27) errorList << "ДГ";
                if (bit26) errorList << "KRL";
                if (bit25) errorList << "Курс ДВ";
                if (bit24) errorList << "Танг ДВ";
                if (!bit30) {
                    errorList << "DM";
                }

                // Ячейки (2,1) и (2,2)
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

                if (bit28)
                {
                    // Пришёл в последний момент
                    int ms = prepStartTime.msecsTo(now);
                    double sec = ms / 1000.0;

                    if (errorList.isEmpty())
                    {
                        gotItem->setBackground(QColor(0, 128, 0));
                        errItem->setText(
                            QString("Готов (бит28); Tгот = %1 c").arg(sec, 0, 'f', 2)
                            );
                    }
                    else
                    {
                        gotItem->setBackground(QColor(255, 0, 0));
                        errItem->setText(
                            QString("Готов, но ошибки: %1; Tгот = %2 c")
                                .arg(errorList.join(", "))
                                .arg(QString::number(sec, 'f', 2))
                            );
                    }
                }
                else
                {
                    // Так и не пришёл
                    gotItem->setBackground(QColor(255, 0, 0));

                    // Всегда выводим ошибки
                    QString baseTxt = QString("Нет готовности (бит28=0) за 20с");
                    if (!errorList.isEmpty()) {
                        baseTxt += "; Ошибки: " + errorList.join(", ");
                    }
                    errItem->setText(baseTxt);
                }
                return; // Завершаем лямбду
            }

            // 4) Если ещё не вышли 20с — смотрим, не пришёл ли bit28 сейчас
            unsigned long val2 = IN_KPA[2] & 0x7FFFFFFF;
            unsigned long val0 = IN_KPA[0] & 0x7FFFFFFF;

            bool bit28 = (val2 & (1UL << 28));
            bool bit27 = (val2 & (1UL << 27));
            bool bit26 = (val2 & (1UL << 26));
            bool bit25 = (val2 & (1UL << 25));
            bool bit24 = (val2 & (1UL << 24));
            bool bit30 = (val0 & (1UL << 30));

            // Список ошибок
            QStringList errorList;
            if (bit27) errorList << "ДГ";
            if (bit26) errorList << "KRL";
            if (bit25) errorList << "Курс ДВ";
            if (bit24) errorList << "Танг ДВ";
            if (!bit30) {
                errorList << "DM";
            }

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

            if (bit28)
            {
                // Готов пришёл досрочно, до 20с
                readinessTimer->stop();

                // Вычисляем фактическое время
                int ms = prepStartTime.msecsTo(now);
                double sec = ms / 1000.0;

                if (errorList.isEmpty())
                {
                    gotItem->setBackground(QColor(0, 128, 0));
                    errItem->setText(
                        QString("Готов (бит28); Tгот = %1 c").arg(QString::number(sec, 'f', 2))
                        );
                }
                else
                {
                    gotItem->setBackground(QColor(255, 0, 0));
                    errItem->setText(
                        QString("Готов, но ошибки: %1; Tгот = %2 c")
                            .arg(errorList.join(", "))
                            .arg(QString::number(sec, 'f', 2))
                        );
                }
            }
            else
            {
                // bit28=0, но время ещё идёт
                // Просто показываем «ожидание» + ошибки
                gotItem->setBackground(QColor(255, 0, 0));

                // Сколько уже ждём?
                int msWaiting = prepStartTime.msecsTo(now);
                double secWait = msWaiting / 1000.0;

                QString txt = QString("Ожидание готовности: %1 c").arg(secWait, 0, 'f', 2);
                if (!errorList.isEmpty()) {
                    txt += "; Ошибки: " + errorList.join(", ");
                }
                errItem->setText(txt);
                // Продолжаем ожидать, не выходим
            }
        });

        // (F) Стартуем
        readinessTimer->start();
    }
    else
    {
        // --- «Подготовка» отключена (Off) ---
        readinessTimer->stop();

        // Снимаем бит 9
        OUT_AD9M2[0] &= ~(1UL << 9);
        ULONG sum = KS(OUT_AD9M2, 6);
        OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
        BUF256x32_write(0, OUT_AD9M2, 7);
        SO_pusk(0);

        // Сбрасываем биты (кроме литер)
        resetOutAD9M2PreservingLiters();
        resetAllClickedButtons();

        // Очищаем GUI
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
}

void handleButtonClick2() {
    // clickedButton2 = !clickedButton2;
    OUT_AD9M2[0] &= ~(0x1 << 16); // Сбрасываем бит
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 16;

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
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
