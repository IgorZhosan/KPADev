#include "tvk02061.h"
#include "data_receiver.h"
#include <windows.h>
#include <QTextEdit>
#include <QString>
#include <QCheckBox>

// Определяем extern для объектов, которые уже есть в других файлах
extern HANDLE hECE0206_1;
extern QCheckBox *kpaCheckBox;
extern QCheckBox *priemCheckBox;
extern QTextEdit* terminal_down;
extern DWORD nOutput;


// Массив для сохранения посылки
ULONG IN_KPA[11] = {0};
ULONG OUT_AD9M2[7] = {0};

// Структура для получения данных
typedef struct {
    DWORD param;  // данные
    DWORD timer;  // значение таймера
    UCHAR error;  // код ошибки
} INPUTPARAM;

INPUTPARAM ParamCod;  // Переменная для получения данных

//Функция проверка нажатия чекбоксов
void ifCheckBoxesIsTrue() {
    if (kpaCheckBox->isChecked() && priemCheckBox->isChecked()) {
        receiveDataAndDisplay();
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
    if (terminal_down) {
        terminal_down->append(strout);  // Выводим в текстовый виджет
    }
}

//функция отправки данных по 1-му каналу в AD9M2




