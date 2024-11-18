#include "library02061.h"
#include "data_receiver.h"
#include "buttonhandlers.h"
#include <windows.h>
#include <QTextEdit>
#include <QString>
#include <QCheckBox>
#include <QTimer>
#include <thread>
#include <chrono>
#include <QByteArray>

extern HANDLE hECE0206_1;
extern QTextEdit* terminal_down;
extern DWORD nOutput;
extern bool isTerminalPause;
extern bool clickedPreparation;
extern bool clickedButton1;
extern bool clickedButton2;
extern bool clickedButton3;
extern bool clickedButton4;
extern bool clickedButton5;
extern bool clickedButton6;
extern bool clickedButton7;
extern bool clickedButton8;
extern bool clickedButton9;
extern bool clickedButton10;
extern bool clickedButton11;
extern bool clickedButton12;
extern bool clickedButton13;
extern bool clickedButton14;
extern bool clickedButton15;

ULONG KS(ULONG *array, int size);

// Массив для сохранения посылки
ULONG IN_KPA[11] = {0};
ULONG OUT_AD9M2[7] = {0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE5};
ULONG OUT_KPA[2] = {0};
ULONG IN_AD9M2[5] = {0};

HANDLE hSerialPort = INVALID_HANDLE_VALUE;
QTimer* readTimer = new QTimer();
QTimer* portCheckTimer = new QTimer();
bool isReconnecting = false;
// Глобальный буфер для накопления данных
QByteArray packetBuffer;

// Структура для получения данных
typedef struct {
    DWORD param;  // данные
    DWORD timer;  // значение таймера
    UCHAR error;  // код ошибки
} INPUTPARAM;

INPUTPARAM ParamCod;  // Переменная для получения данных

bool isPortOpen() {
    if (hSerialPort == INVALID_HANDLE_VALUE) {
        //  terminal_down->append("COM порт не открыт: INVALID_HANDLE_VALUE");
        return false;
    }
    DWORD errors;
    COMSTAT status;
    if (!ClearCommError(hSerialPort, &errors, &status)) {
        //  terminal_down->append("Ошибка проверки состояния COM порта через ClearCommError.");
        return false;
    }
    // terminal_down->append("COM порт открыт и доступен.");
    return true;
}

void reconnectSerialPortAsync() {
    if (isReconnecting) return;

    isReconnecting = true;
    std::thread reconnectThread([]() {
        if (!openSerialPort("COM2")) {
            //   terminal_down->append("Не удалось переподключить COM порт.");
        } else {
            //   terminal_down->append("COM порт успешно переподключен.");
        }
        isReconnecting = false;
    });
    reconnectThread.detach();  // Освобождаем поток
}

void initializePortCheckTimer() {
    QObject::connect(portCheckTimer, &QTimer::timeout, []() {
        //  terminal_down->append("Таймер проверки состояния COM порта сработал.");
        if (!isPortOpen()) {
            //  terminal_down->append("COM порт не подключен. Попытка переподключения...");
            reconnectSerialPortAsync();
        } else {
            //  terminal_down->append("COM порт подключен.");
        }
    });

    portCheckTimer->start(15000);  // Устанавливаем интервал в 15 секунд
    terminal_down->append("Таймер проверки состояния COM порта запущен.");
}

// Функция открытия COM-порта
bool openSerialPort(LPCSTR portName) {
    wchar_t wPortName[256];
    MultiByteToWideChar(CP_ACP, 0, portName, -1, wPortName, 256);

    hSerialPort = CreateFileW(wPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hSerialPort == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        // terminal_down->append("Не удалось открыть COM порт. Код ошибки: " + QString::number(error));
        return false;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerialPort, &dcbSerialParams)) {
        // terminal_down->append("Ошибка получения состояния COM порта");
        return false;
    }

    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = EVENPARITY;

    if (!SetCommState(hSerialPort, &dcbSerialParams)) {
        terminal_down->append("Ошибка настройки COM порта");
        return false;
    }

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 10;  // Максимальный интервал между байтами (в миллисекундах)
    timeouts.ReadTotalTimeoutConstant = 10;  // Общий таймаут на чтение
    timeouts.ReadTotalTimeoutMultiplier = 0;  // Нет зависимости от количества байт
    SetCommTimeouts(hSerialPort, &timeouts);

    if (!SetCommTimeouts(hSerialPort, &timeouts)) {
        terminal_down->append("Ошибка установки тайм-аутов");
        return false;
    }

    // terminal_down->append("COM порт успешно открыт");
    return true;
}

// Функция чтения данных с COM-порта
OVERLAPPED osReader = {0};  // Структура для асинхронных операций

bool isValidPacket(const QByteArray& packet) {
    // Проверяем длину пакета
    if (packet.size() != 24) {
        return false;
    }

    // Проверяем, что первый байт равен ожидаемому значению
    if (static_cast<unsigned char>(packet[0]) != 0x80) {
        return false;
    }

    // Дополнительные проверки (контрольная сумма, формат и т.д.)
    return true;
}

void readSerialPortAsync() {
    if (hSerialPort == INVALID_HANDLE_VALUE) {
        return;
    }

    const size_t PACKET_SIZE = 24;  // Размер одной посылки
    char tempBuffer[24] = {0};    // Временный буфер для чтения
    DWORD bytesRead = 0;

    // Чтение данных с COM-порта
    if (ReadFile(hSerialPort, tempBuffer, sizeof(tempBuffer), &bytesRead, NULL)) {
        // Добавляем прочитанные данные в общий буфер
        packetBuffer.append(tempBuffer, bytesRead);

        // Обработка данных из буфера
        while (packetBuffer.size() >= PACKET_SIZE) {
            // Проверяем, начинается ли посылка с правильного байта
            if (static_cast<unsigned char>(packetBuffer[0]) != 0x80) {
                // Удаляем байт до синхронизации
                packetBuffer.remove(0, 1);
                continue;
            }

            // Извлекаем полный пакет
            QByteArray fullPacket = packetBuffer.left(PACKET_SIZE);
            packetBuffer.remove(0, PACKET_SIZE);  // Удаляем обработанный пакет

            // Выводим пакет в терминал
            if (!isTerminalPause) {
                QString hexData;
                for (char byte : fullPacket) {
                    hexData += QString::asprintf("%02X ", static_cast<unsigned char>(byte));
                }
                terminal_down->append("TM: " + hexData.trimmed());
            }
        }
    } else {
        DWORD error = GetLastError();
        terminal_down->append("Ошибка чтения данных с COM порта. Код ошибки: " + QString::number(error));
    }
}

// Функция закрытия COM-порта
void closeSerialPort() {
    if (hSerialPort != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerialPort);
        hSerialPort = INVALID_HANDLE_VALUE;
        // terminal_down->append("COM порт закрыт");
    }
}

void startAsyncReading() {
    std::thread readerThread([]() {
        while (true) {
            readSerialPortAsync();  // Асинхронная функция чтения
            std::this_thread::sleep_for(std::chrono::milliseconds(200));  // Периодический опрос
        }
    });
    readerThread.detach();  // Отсоединяем поток
}

void startCommunication() {
    // Проверяем, открыт ли COM-порт
    if (hSerialPort == INVALID_HANDLE_VALUE) {
        // Пытаемся открыть COM-порт
        if (!openSerialPort("COM1")) {
            //  terminal_down->append("Не удалось открыть COM порт.");
            return;
        }
    }

    // Запускаем таймер для проверки состояния порта
    initializePortCheckTimer();
    // Запуск асинхронного чтения в отдельном потоке
    startAsyncReading();
}

// Функция остановки работы с COM-портом
void stopCommunication() {
    if (portCheckTimer->isActive()) {
        portCheckTimer->stop();
    }

    if (readTimer->isActive()) {
        readTimer->stop();
    }

    // Закрываем COM-порт
    closeSerialPort();
}

// Основная логика работы
void processSerialCommunication() {
    // Чтение с COM-порта и вывод данных
    if (TM -> isChecked() && priemCheckBox -> isChecked()) {
        readSerialPortAsync();  // Выполняем чтение данных при активных чек-боксах
    }
}

// Функция для получения посылки и вывода её в терминал
void receiveDataAndDisplay()
{
    QString strout;

    // Читаем данные по каналу 2
    for (int i = 0; i < 11; i++) {
        int temp = i + 1;

        // Используем DeviceIoControl для получения посылки
        DeviceIoControl(hECE0206_1, ECE02061_XP_READ_PARAM_AP2, &temp, 1, &ParamCod, sizeof(ParamCod), &nOutput, NULL);

        // Сохраняем данные в массив
        IN_KPA[i] = ParamCod.param;
    }

    // Формируем строку для вывода в терминал
    strout = "IN : ";
    for (int i = 0; i < 11; i++) {
        QString str = QString("%1 ").arg((DWORD)IN_KPA[i], 8, 16, QChar('0')).toUpper();
        str.resize(6);
        strout += str + " ";
    }

    // Выводим строку в terminal_down
    if (!isTerminalPause && terminal_down && kpaCheckBox->isChecked() && priemCheckBox->isChecked()) {
        terminal_down->append(strout);  // Выводим в текстовый виджет
    }
}

// функция для литеров
void coder_CH1(void) {
    ULONG sum = 0;

    OUT_AD9M2[0] |= 0x1 << 20;

    OUT_AD9M2[1] |= ((0x1 & (out_lt1_checkbox->isChecked())) << 9) |
                    ((0x1 & (out_lt2_checkbox->isChecked())) << 10) |
                    ((0x1 & (out_lt3_checkbox->isChecked())) << 11) |
                    ((0x1 & (out_lt4_checkbox->isChecked())) << 12) |
                    ((0x1 & (out_lt5_checkbox->isChecked())) << 13) |
                    ((0x1 & (out_lt6_checkbox->isChecked())) << 14) |
                    ((0x1 & (out_lt7_checkbox->isChecked())) << 15);


    OUT_AD9M2[1] |= ((0x1 & (out_lk1_checkbox->isChecked())) << 16) |
                    ((0x1 & (out_lk2_checkbox->isChecked())) << 17) |
                    ((0x1 & (out_lk3_checkbox->isChecked())) << 18) |
                    ((0x1 & (out_lk4_checkbox->isChecked())) << 19);

    // Добавление состояния чекбоксов КК в третий элемент массива
    OUT_AD9M2[1] |= ((0x1 & (out_kk1_checkbox->isChecked())) << 20) |
                    ((0x1 & (out_kk2_checkbox->isChecked())) << 21) |
                    ((0x1 & (out_kk3_checkbox->isChecked())) << 22) |
                    ((0x1 & (out_kk4_checkbox->isChecked())) << 23) |
                    ((0x1 & (out_kk5_checkbox->isChecked())) << 24) |
                    ((0x1 & (out_kk6_checkbox->isChecked())) << 25) |
                    ((0x1 & (out_kk7_checkbox->isChecked())) << 26) |
                    ((0x1 & (out_kk8_checkbox->isChecked())) << 27);

    sum = KS(OUT_AD9M2, 6);

    //OUT_AD9M2[6] = 0xE5;
    OUT_AD9M2[6] |= ((sum & 0xFFFF) << 8);
}

void checkAndSendAD9M2Broadcast() {
    coder_CH1();
    // Отправляем данные через канал (здесь `BUF256x32_write` и `SO_pusk` - функции для работы с каналом)
    BUF256x32_write(0, OUT_AD9M2, 7);  // Отправляем данные, 7 - это количество элементов массива

    // Выводим данные в textEdit, если чекбокс для вывода активирован
    if (!isTerminalPause && terminal_down && AD9M2->isChecked() && broadcast->isChecked()) {
        QString strout = "OUT: ";
        QString str;
        // Формируем строку для вывода в терминал
        for (int i = 0; i < 7; i++) {
            str = QString("%1 ").arg((DWORD)OUT_AD9M2[i], 8, 16, QChar('0')).toUpper();
            str.resize(6);
            strout += str + "   ";
        }
        terminal_down->append(strout);  // Выводим строку в текстовый виджет
    }
    // Отправка данных на канал
    SO_pusk(0);  // 0 - это номер канала
}

ULONG KS(ULONG *array, int size) // функция подсчета КС/ size - число ячеек массива, для которых нужно посчитать КС
{
    int i;
    ULONG summ=0;

    for (i=0; i<size; i++)
    {
        summ+=((*(array+i)>>16)&0x7FFF); // старшие 16 раз без 32 разряда
        summ+=(summ>>16) & 0x1;          // если вышла 1 за 16 разрядов, прибавляем ее в мл. разр
        summ&=0xFFFF;                    // обнуляем вышедшую 1.

        summ+=((*(array+i)&0xFF00)|((i+1)&0xFF));   // аналогично младшие 16 разрядов , но адрес при этом считается прямой, хотя отправляется\принимается перевернутый
        summ+=(summ>>16) & 0x1;
        summ&=0xFFFF;
    }
    return summ;
}

void coder_CH2() {
    OUT_KPA[0] = 0x0;
    OUT_KPA[0] |= (0x40 << 8);  // было & (0xFF)
    OUT_KPA[0] |= (0x40 << 16); // было & (0x7F)
    // OUT_KPA[0] |= ((0x1 << 28));
    // OUT_KPA[0] |= ((0x1 << 29));
    // OUT_KPA[0] |= ((0x1 << 30));
    OUT_KPA[1] = (0x80 | (OUT_KPA[0] & 0xFFFFFF00));
}

void checkAndSendBroadcastKPA() {
    coder_CH2();
    BUF256x32_write(1, OUT_KPA, 2);
    if (!isTerminalPause && terminal_down && kpaCheckBox->isChecked() && broadcast->isChecked()) {
        QString strout = "OUT: ";
        QString str;
        strout="OUT: ";
        for (int i=0; i < 2; i++)
        {
            str=(QString("%1 ").arg((DWORD)OUT_KPA[i], 8, 16, QChar('0'))).toUpper();
            str.resize(6);
            strout+=str + "   ";
        }
        terminal_down->append(strout);
    }
    SO_pusk(1);  // отправка данных на канал 2
}

void receiveDataIN_KPA() {
    int temp;
    QString strout;
    QString str;
    for (int i = 0; i < 5; i++) {
        if (i!=4) temp=i+1; else temp=0x37;

        // переворот адреса
        temp= (temp & 0x0F)<<4 | (temp>>4) & 0x0F;
        temp= (temp & 0x33)<<2 | (temp>>2) & 0x33;
        temp= (temp & 0x55)<<1 | (temp>>1) & 0x55;

        DeviceIoControl(hECE0206_0, ECE02061_XP_READ_PARAM_AP1, &temp, 1, &ParamCod, 9, &nOutput, NULL);
        IN_AD9M2[i]=ParamCod.param;
    }

    if (!isTerminalPause && terminal_down && AD9M2->isChecked() && priemCheckBox->isChecked()) {
        strout="IN:  ";
        for (int i = 0; i < 5; i++)
        {
            str=(QString("%1 ").arg((DWORD)IN_AD9M2[i], 8, 16, QChar('0'))).toUpper();
            str.resize(6);
            strout+=str + "   ";
        }
        terminal_down->append(strout);
    }
}

void Timer_Event() {
    receiveDataAndDisplay();
    checkAndSendAD9M2Broadcast();
    checkAndSendBroadcastKPA();
    receiveDataIN_KPA();
    processSerialCommunication();
}
