#include "tvk02061.h"
#include "data_receiver.h"
#include <windows.h>
#include <QTextEdit>
#include <QString>

// Определяем extern для объектов, которые уже есть в других файлах
extern HANDLE hECE0206_1;
extern HANDLE hECE0206_0;
extern QTextEdit* terminal_down;
extern DWORD nOutput;

// Массивы для данных с каждого канала
ULONG IN_KPA_CH1[7] = {0};  // Массив для данных канала 1 (7 элементов)
ULONG IN_KPA_CH2[5] = {0};  // Массив для данных канала 2 (5 элементов)
ULONG IN_KPA[11] = {0};     // Общий массив для вывода в терминал (11 элементов)

// Структура для получения данных
typedef struct {
    DWORD param;  // данные
    DWORD timer;  // значение таймера
    UCHAR error;  // код ошибки
} INPUTPARAM;

INPUTPARAM ParamCod;  // Переменная для получения данных

// Функция для получения посылки и вывода её в терминал
void receiveDataAndDisplay()
{
    QString strout;

    // Читаем данные по каналу 1 (посылка состоит из 7 элементов)
    for (int i = 0; i < 7; i++) {
        int temp = i + 1;

        // Используем DeviceIoControl для получения посылки с канала 1
        DeviceIoControl(hECE0206_0, ECE02061_XP_READ_PARAM_AP1, &temp, 1, &ParamCod, sizeof(ParamCod), &nOutput, NULL);

        // Сохраняем данные в массив
        IN_KPA_CH1[i] = ParamCod.param;
    }

    // Читаем данные по каналу 2 (посылка состоит из 5 элементов)
    for (int i = 0; i < 5; i++) {
        int temp = i + 1;

        // Используем DeviceIoControl для получения посылки с канала 2
        DeviceIoControl(hECE0206_1, ECE02061_XP_READ_PARAM_AP2, &temp, 1, &ParamCod, sizeof(ParamCod), &nOutput, NULL);

        // Сохраняем данные в массив
        IN_KPA_CH2[i] = ParamCod.param;
    }

    // Объединяем данные в один массив для вывода (первые 7 элементов из канала 1, остальные 4 из канала 2)
    for (int i = 0; i < 7; i++) {
        IN_KPA[i] = IN_KPA_CH1[i];  // Копируем первые 7 элементов из канала 1
    }
    for (int i = 0; i < 5; i++) {
        IN_KPA[7 + i] = IN_KPA_CH2[i];  // Копируем оставшиеся 4 элемента из канала 2
    }

    // Формируем строку для вывода в терминал
    strout = "IN : ";
    for (int i = 0; i < 11; i++) {
        QString str = QString("%1 ").arg((DWORD)IN_KPA[i], 8, 16, QChar('0')).toUpper();
        str = str.left(6);  // Обрезаем строку до 6 символов
        strout += str + " ";
    }

    // Выводим строку в terminal_down
    if (terminal_down) {
        terminal_down->append(strout);  // Выводим в текстовый виджет
    }
}
