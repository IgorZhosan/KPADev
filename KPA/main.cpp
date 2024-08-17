#include "mainwindow.h"  // Подключаем заголовочный файл MainWindow
#include <QApplication>
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QHeaderView>

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

    w.show();
    return a.exec();
}
