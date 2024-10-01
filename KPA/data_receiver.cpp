#include "library02061.h"
#include "data_receiver.h"
#include "buttonhandlers.h"
#include <windows.h>
#include <QTextEdit>
#include <QString>
#include <QCheckBox>

extern HANDLE hECE0206_1;
extern QTextEdit* terminal_down;
extern DWORD nOutput;
extern bool isTerminalPause;
extern bool clickedPreparation;

ULONG KS(ULONG *array, int size);


// Массив для сохранения посылки
ULONG IN_KPA[11] = {0};
ULONG OUT_AD9M2[7] = {0};
ULONG OUT_KPA[2] = {0};
ULONG IN_AD9M2[5] = {0};

HANDLE hSerialPort = INVALID_HANDLE_VALUE;

// Структура для получения данных
typedef struct {
    DWORD param;  // данные
    DWORD timer;  // значение таймера
    UCHAR error;  // код ошибки
} INPUTPARAM;

INPUTPARAM ParamCod;  // Переменная для получения данных

void initSerialPort(const wchar_t* portName) {
    hSerialPort = CreateFile(portName, GENERIC_READ | GENERIC_WRITE,
                             0,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerialPort == INVALID_HANDLE_VALUE) {
        terminal_down -> append("Не удалось открыть порт");
        return;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if(!GetCommState(hSerialPort, &dcbSerialParams)) {
        terminal_down->append("Не удалось получить состояние порта");
        CloseHandle(hSerialPort);
        hSerialPort = INVALID_HANDLE_VALUE;
        return;
    }

    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerialPort, &dcbSerialParams)) {
        terminal_down->append("Не удалось настроить порт");
        CloseHandle(hSerialPort);
        hSerialPort = INVALID_HANDLE_VALUE;
        return;
    }

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerialPort, &timeouts)) {
        terminal_down -> append("Не удалось настроить таймауты порта");
        CloseHandle(hSerialPort);
        hSerialPort = INVALID_HANDLE_VALUE;
        return;
    }
}

void readFromSerialPort() {

    DWORD bytesRead;
    char buffer[256];
    BOOL result = ReadFile(hSerialPort, buffer, sizeof(buffer) - 1, &bytesRead, NULL);

    if (!result) {
        initSerialPort(L"COM5");
        terminal_down->append("Ошибка чтения порта, функция readFromSerialPort");
        return;
    }

    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        QString data = QString::fromLocal8Bit(buffer);

        if (!isTerminalPause && terminal_down && priemCheckBox -> isChecked() && TM -> isChecked()) {
            terminal_down->append(data);
        }
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


void coder_CH1(void) {
    ULONG sum = 0;

    // Инициализация первого элемента массива OUT_AD9M2
    OUT_AD9M2[0] = 0x80;  // Базовое значение

    OUT_AD9M2[0] |= 0x1 << 20;
    OUT_AD9M2[0] |= (0x1 & clickedPreparation) << 9;

    // Формирование перв ого элемента массива OUT_AD9M2 на основе состояния чекбоксов
 /*  OUT_AD9M2[0] |= ((0x1 & (out_otkaz_checkbox->isChecked())) << 8) |
                    ((0x1 & (out_po_checkbox->isChecked())) << 10) |
                    ((0x1 & (out_k0_checkbox->isChecked())) << 11) |
                    ((0x1 & (out_tk_checkbox->isChecked())) << 12) |
                    ((0x1 & (out_kvs_checkbox->isChecked())) << 14) |
                    ((0x1 & (out_tsn_checkbox->isChecked())) << 15) |
                    ((0x1 & (out_nkk_checkbox->isChecked())) << 16) |
                    ((0x1 & (out_mgn_checkbox->isChecked())) << 17) |
                    ((0x1 & (out_pp_checkbox->isChecked())) << 18) |
                    ((0x1 & (out_ppp_checkbox->isChecked())) << 19) |
                    ((0x1 & (out_nk_checkbox->isChecked())) << 20) |
                    ((0x1 & (out_itp_checkbox->isChecked())) << 21) |
                    ((0x1 & (out_pr_checkbox->isChecked())) << 22);
*/
  //  if (out_nkk_checkbox->isChecked()) {
  //      OUT_AD9M2[0] |= 1 << 13;
  //  }

    // Формирование второго элемента массива OUT_AD9M2 на основе состояния чекбоксов ЛК
    OUT_AD9M2[1] = 0x40;  // Базовое значение

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

    // Инициализация остальных элементов массива
    OUT_AD9M2[2] = 0xC0;
    OUT_AD9M2[3] = 0x20;
    OUT_AD9M2[4] = 0xA0;
    OUT_AD9M2[5] = 0x60;

     sum = KS(OUT_AD9M2, 6);

    OUT_AD9M2[6] = 0xE5;
    OUT_AD9M2[6] |= ((sum & 0xFFFF) << 8);
}

void checkAndSendAD9M2Broadcast() {
        // Формируем посылку
        coder_CH1();  //функцию для формирования посылки

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
       // readFromSerialPort();
}



