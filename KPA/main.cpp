#include "mainwindow.h"  // Подключаем заголовочный файл MainWindow
#include <QApplication>
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QTextEdit>
#include <QIODevice>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QStringDecoder>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // Доступ к таблице через класс MainWindow, объект интерфейса создается внутри MainWindow
    QTableWidget *table = w.findChild<QTableWidget*>("turning_on_the_equipment");  // Найдем таблицу по её имени

    if (table) {
        table->setItem(0, 0, new QTableWidgetItem("Включить тумблер \"Обогрев\""));
        table->setItem(0, 1, new QTableWidgetItem("Гот. ТЕРМ."));
        table->setItem(0, 2, new QTableWidgetItem("Тобогр = (Допуск (2±0.2) мин)"));

        table->setItem(1, 0, new QTableWidgetItem("Включить тумблер \"СПС\""));
        table->setItem(1, 1, new QTableWidgetItem("СПС"));
        table->setItem(1, 2, new QTableWidgetItem("Тспс = (Допуск (2±0.2) мин)"));

        QPushButton *button = new QPushButton("Подготовка");
        table->setCellWidget(2, 0, button);

        table->setItem(2, 1, new QTableWidgetItem("Т1А ГОТ"));
        table->setItem(2, 2, new QTableWidgetItem("Тгот = (Допуск (0-15)с)"));
        table->horizontalHeader()->setStretchLastSection(true);
        table->resizeColumnsToContents();
        int totalHeight = 0;
        for (int row = 0; row < table->rowCount(); ++row) {
            totalHeight += table->rowHeight(row);  // Суммируем высоту всех строк
        }

        // Добавляем высоту заголовка таблицы
        totalHeight += table->horizontalHeader()->height();

        // Устанавливаем высоту таблицы на основе общего количества строк
        table->setMinimumHeight(totalHeight);
        table->setMaximumHeight(totalHeight);
    }

    QTextEdit *terminal = w.findChild<QTextEdit*>("terminal_up");

    if (terminal) {
        // Отображаем текущий путь для диагностики
        qDebug() << "Текущий каталог:" << QDir::currentPath();

        // Путь к файлу ControlInfo.rtf
        QString filePath = "C:\\Users\\zhosa\\Desktop\\KPA\\KPA\\GUI\\Controlinfo.txt";  // Относительный путь из папки build

        QFile file(filePath);

        // Проверим, что файл открылся успешно
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(&w, "Ошибка", "Не удалось открыть файл " + filePath);
            return -1;
        }

        // Чтение содержимого файла
        QStringDecoder decoder(QStringConverter::Utf8);  // Указываем, что файл в кодировке UTF-8
        QString fileContent = decoder.decode(file.readAll());
        file.close();

        // Установка текста в QTextEdit
        terminal->setPlainText(fileContent);  // Если файл в формате текста без форматирования
    }



    w.show();
    return a.exec();
}
