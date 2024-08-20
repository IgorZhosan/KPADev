#include "mainwindow.h"  // Подключаем заголовочный файл MainWindow
#include "buttonhandlers.h"
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
#include <QVBoxLayout>
#include <QCheckBox>
#include <QMenu>
#include <QWidgetAction>
#include <QDebug>
#include <QGroupBox>

// Функция для создания группы чекбоксов с именами
QGroupBox* createCheckboxGroup(const QString& title, const QStringList& labels, const QStringList& objectNames) {
    QGroupBox* groupBox = new QGroupBox(title);  // Создаем группу с заголовком
    QHBoxLayout* hbox = new QHBoxLayout;  // Горизонтальный layout для чекбоксов

    // Создаем чекбоксы с названиями из списка labels и присваиваем им objectName
    for (int i = 0; i < labels.size(); ++i) {
        QCheckBox* checkbox = new QCheckBox(labels[i]);  // Чекбокс с текстом
        checkbox->setObjectName(objectNames[i]);  // Устанавливаем уникальное имя для каждого чекбокса
        hbox->addWidget(checkbox);  // Добавляем чекбокс в layout
    }

    groupBox->setLayout(hbox);  // Устанавливаем layout в группу
    return groupBox;
}

// Функция для создания общей группы для ЛТ, ЛК, КК
QWidget* createGroupedLT_LK_KK() {
    QWidget* container = new QWidget;  // Контейнер для всех трех групп
    QHBoxLayout* hbox = new QHBoxLayout(container);  // Горизонтальный layout

    // Создаем группы чекбоксов для ЛТ, ЛК и КК без текстов (пустые чекбоксы)
    QGroupBox* ltGroup = createCheckboxGroup("ЛТ [7:1]", QStringList(7, ""), QStringList(7, ""));  // 7 пустых чекбоксов
    QGroupBox* lkGroup = createCheckboxGroup("ЛК [4:1]", QStringList(4, ""), QStringList(4, ""));  // 4 пустых чекбокса
    QGroupBox* kkGroup = createCheckboxGroup("КК [8:1]", QStringList(8, ""), QStringList(8, ""));  // 8 пустых чекбоксов

    // Добавляем группы в горизонтальный layout, чтобы они были рядом
    hbox->addWidget(ltGroup);
    hbox->addWidget(lkGroup);
    hbox->addWidget(kkGroup);

    return container;
}

QMenu* createCheckboxMenu(QWidget* parent) {
    QMenu* menu = new QMenu(parent);

    // Создаем контейнер для ЛТ, ЛК и КК
    QWidget* lt_lk_kk_container = createGroupedLT_LK_KK();

    // Создаем группы для остальных чекбоксов с нужными именами
    QStringList otherCheckboxLabels = {"Отказ канала", "ПО", "K0", "ТК", "КВС", "ТСН", "НКК", "МГн", "ПП", "ППП", "НК", "ИТП", "ПР"};
    QStringList otherCheckboxNames = {"OTKAZ", "PO", "k0", "TK", "KVS", "TCH", "NKK", "MGN", "USPP", "PPP", "NK", "ITP", "PR"};

    // Создаем группу с этими чекбоксами и их именами
    QGroupBox* otherGroup = createCheckboxGroup("Прочие", otherCheckboxLabels, otherCheckboxNames);

    // Упаковываем группы в действия меню
    QWidgetAction* lt_lk_kk_Action = new QWidgetAction(menu);
    lt_lk_kk_Action->setDefaultWidget(lt_lk_kk_container);
    menu->addAction(lt_lk_kk_Action);

    // Добавляем группу с остальными чекбоксами
    QWidgetAction* otherAction = new QWidgetAction(menu);
    otherAction->setDefaultWidget(otherGroup);
    menu->addAction(otherAction);

    return menu;
}

// Функция для проверки состояния чекбоксов
void checkCheckboxStates(QWidget* parent) {
    // Пример: поиск по имени и вывод состояния чекбоксов
    QCheckBox* checkbox = parent->findChild<QCheckBox*>("checkbox_LT");
    if (checkbox && checkbox->isChecked()) {
        qDebug() << "Чекбокс ЛТ[7:1] нажат";
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");


    // Доступ к таблице через класс MainWindow, объект интерфейса создается внутри MainWindow
    terminal_down = w.findChild<QTextEdit*>("terminal_down");
    QTableWidget *turning_on_the_equipment = w.findChild<QTableWidget*>("turning_on_the_equipment");  //

    if (turning_on_the_equipment) {

        turning_on_the_equipment->setSelectionMode(QAbstractItemView::NoSelection);
        turning_on_the_equipment->setSelectionBehavior(QAbstractItemView::SelectItems);

        turning_on_the_equipment->setItem(0, 0, new QTableWidgetItem("Включить тумблер \"Обогрев\""));
        turning_on_the_equipment->setItem(0, 1, new QTableWidgetItem("Гот. ТЕРМ."));
        turning_on_the_equipment->setItem(0, 2, new QTableWidgetItem("Тобогр = (Допуск (2±0.2) мин)"));

        turning_on_the_equipment->setItem(1, 0, new QTableWidgetItem("Включить тумблер \"СПС\""));
        turning_on_the_equipment->setItem(1, 1, new QTableWidgetItem("СПС"));
        turning_on_the_equipment->setItem(1, 2, new QTableWidgetItem("Тспс = (Допуск (2±0.2) мин)"));

        QPushButton *preparation = new QPushButton("Подготовка");
        turning_on_the_equipment->setCellWidget(2, 0, preparation);

        turning_on_the_equipment->setItem(2, 1, new QTableWidgetItem("Т1А ГОТ"));
        turning_on_the_equipment->setItem(2, 2, new QTableWidgetItem("Тгот = (Допуск (0-15)с)"));
        turning_on_the_equipment->horizontalHeader()->setStretchLastSection(true);
        turning_on_the_equipment->resizeColumnsToContents();
        int totalHeight = 0;
        for (int row = 0; row < turning_on_the_equipment->rowCount(); ++row) {
            totalHeight += turning_on_the_equipment->rowHeight(row);  // Суммируем высоту всех строк
        }

        // Добавляем высоту заголовка таблицы
        totalHeight += turning_on_the_equipment->horizontalHeader()->height();

        // Устанавливаем высоту таблицы на основе общего количества строк
        turning_on_the_equipment->setMinimumHeight(totalHeight);
        turning_on_the_equipment->setMaximumHeight(totalHeight);
    }

    QTextEdit *terminal = w.findChild<QTextEdit*>("terminal_up");

    if (terminal) {
        // Отображаем текущий путь для диагностики
        qDebug() << "Текущий каталог:" << QDir::currentPath();

        // Путь к файлу ControlInfo.rtf
        QString filePath = "C:\\KPADev\\KPA\\GUI\\Controlinfo.txt";  // Относительный путь из папки build

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

    QTableWidget *checking_the_operation = w.findChild<QTableWidget*>("checking_the_operation");

    checking_the_operation->setSelectionMode(QAbstractItemView::NoSelection);
    checking_the_operation->setSelectionBehavior(QAbstractItemView::SelectItems);

    if (checking_the_operation) {
        // Устанавливаем количество строк и столбцов (4 столбца)
        checking_the_operation->setRowCount(15);
        checking_the_operation->setColumnCount(4);

        // Устанавливаем заголовки столбцов
        QStringList headers = { "Наименование параметра", "Запуск проверки", "Индикация выполнения", "Дополнительная информация" };
        checking_the_operation->setHorizontalHeaderLabels(headers);

        // Список параметров для первого столбца
        QStringList parameters = {
            "Поле зрения", "Разрешающая способность", "Время вывода \"Т1А готов\" после команды \"Подготовка\"",
            "Градация яркости", "Замещение служебной информации", "Углы разворота ЛВ", "Автосопровождение",
            "Информационный обмен с АД.9М2", "Информационный обмен с ГБМ", "Передача телеметрических данных",
            "Прием команды по КРЛ", "Передача видео по ТРЛ", "Внешнее управление антенной", "Уход антенны", "Зона захвата антенны"
        };

        // Добавляем кнопки вручную для каждой строки

        // Строка 1
        QPushButton *checking_the_operation_button1 = new QPushButton("Запуск");
        checking_the_operation_button1->setObjectName("checking_the_operation_button1");
        QObject::connect(checking_the_operation_button1, &QPushButton::clicked, handleButtonClick1);
        checking_the_operation->setItem(0, 0, new QTableWidgetItem(parameters[0]));
        checking_the_operation->setCellWidget(0, 1, checking_the_operation_button1);
        checking_the_operation->setItem(0, 3, new QTableWidgetItem("Визуальный контроль по изображению"));

        // Строка 2
        QPushButton *checking_the_operation_button2 = new QPushButton("Запуск");
        checking_the_operation_button2->setObjectName("checking_the_operation_button2");
        QObject::connect(checking_the_operation_button2, &QPushButton::clicked, handleButtonClick2);
        checking_the_operation->setItem(1, 0, new QTableWidgetItem(parameters[1]));
        checking_the_operation->setCellWidget(1, 1, checking_the_operation_button2);
        checking_the_operation->setItem(1, 3, new QTableWidgetItem("Визуальный контроль по изображению"));

        // Строка 3
        QPushButton *checking_the_operation_button3 = new QPushButton("Запуск");
        checking_the_operation_button3->setObjectName("checking_the_operation_button3");
        QObject::connect(checking_the_operation_button3, &QPushButton::clicked, handleButtonClick3);
        checking_the_operation->setItem(2, 0, new QTableWidgetItem(parameters[2]));
        checking_the_operation->setCellWidget(2, 1, checking_the_operation_button3);

        // Строка 4
        QPushButton *checking_the_operation_button4 = new QPushButton("Запуск");
        checking_the_operation_button4->setObjectName("checking_the_operation_button4");
        QObject::connect(checking_the_operation_button4, &QPushButton::clicked, handleButtonClick4);
        checking_the_operation->setItem(3, 0, new QTableWidgetItem(parameters[3]));
        checking_the_operation->setCellWidget(3, 1, checking_the_operation_button4);
        checking_the_operation->setItem(3, 3, new QTableWidgetItem("Визуальный контроль по изображению"));

        // Строка 5
        QPushButton *checking_the_operation_button5 = new QPushButton("Запуск");
        checking_the_operation_button5->setObjectName("checking_the_operation_button5");
        QObject::connect(checking_the_operation_button5, &QPushButton::clicked, handleButtonClick5);
        checking_the_operation->setItem(4, 0, new QTableWidgetItem(parameters[4]));
        checking_the_operation->setCellWidget(4, 1, checking_the_operation_button5);
        checking_the_operation->setItem(4, 3, new QTableWidgetItem("Визуальный контроль по изображению"));

        // Строка 6
        QPushButton *checking_the_operation_button6 = new QPushButton("Запуск");
        checking_the_operation_button6->setObjectName("checking_the_operation_button6");
        QObject::connect(checking_the_operation_button6, &QPushButton::clicked, handleButtonClick6);
        checking_the_operation->setItem(5, 0, new QTableWidgetItem(parameters[5]));
        checking_the_operation->setCellWidget(5, 1, checking_the_operation_button6);
        checking_the_operation->setItem(5, 3, new QTableWidgetItem("Визуальный контроль по изображению"));

        // Строка 7
        QPushButton *checking_the_operation_button7 = new QPushButton("Запуск");
        checking_the_operation_button7->setObjectName("checking_the_operation_button7");
        QObject::connect(checking_the_operation_button7, &QPushButton::clicked, handleButtonClick7);
        checking_the_operation->setItem(6, 0, new QTableWidgetItem(parameters[6]));
        checking_the_operation->setCellWidget(6, 1, checking_the_operation_button7);
        checking_the_operation->setItem(6, 3, new QTableWidgetItem("Визуальный контроль по изображению"));

        // Строка 8
        QPushButton *checking_the_operation_button8 = new QPushButton("Запуск");
        checking_the_operation_button8->setObjectName("checking_the_operation_button8");
        QObject::connect(checking_the_operation_button8, &QPushButton::clicked, handleButtonClick8);
        checking_the_operation->setItem(7, 0, new QTableWidgetItem(parameters[7]));
        checking_the_operation->setCellWidget(7, 1, checking_the_operation_button8);

        // Строка 9
        QPushButton *checking_the_operation_button9 = new QPushButton("Запуск");
        checking_the_operation_button9->setObjectName("checking_the_operation_button9");
        QObject::connect(checking_the_operation_button9, &QPushButton::clicked, handleButtonClick9);
        checking_the_operation->setItem(8, 0, new QTableWidgetItem(parameters[8]));
        checking_the_operation->setCellWidget(8, 1, checking_the_operation_button9);
        checking_the_operation->setItem(8, 3, new QTableWidgetItem("Контроль видеосигнала"));

        // Строка 10
        QPushButton *checking_the_operation_button10 = new QPushButton("Запуск");
        checking_the_operation_button10->setObjectName("checking_the_operation_button10");
        QObject::connect(checking_the_operation_button10, &QPushButton::clicked, handleButtonClick10);
        checking_the_operation->setItem(9, 0, new QTableWidgetItem(parameters[9]));
        checking_the_operation->setCellWidget(9, 1, checking_the_operation_button10);

        // Строка 11
        QPushButton *checking_the_operation_button11 = new QPushButton("Запуск");
        checking_the_operation_button11->setObjectName("checking_the_operation_button11");
        QObject::connect(checking_the_operation_button11, &QPushButton::clicked, handleButtonClick11);
        checking_the_operation->setItem(10, 0, new QTableWidgetItem(parameters[10]));
        checking_the_operation->setCellWidget(10, 1, checking_the_operation_button11);

        // Строка 12
        QPushButton *checking_the_operation_button12 = new QPushButton("Запуск");
        checking_the_operation_button12->setObjectName("checking_the_operation_button12");
        QObject::connect(checking_the_operation_button12, &QPushButton::clicked, handleButtonClick12);
        checking_the_operation->setItem(11, 0, new QTableWidgetItem(parameters[11]));
        checking_the_operation->setCellWidget(11, 1, checking_the_operation_button12);
        checking_the_operation->setItem(11, 3, new QTableWidgetItem("Визуальный контроль по изображению"));

        // Строка 13
        QPushButton *checking_the_operation_button13 = new QPushButton("Запуск");
        checking_the_operation_button13->setObjectName("checking_the_operation_button13");
        QObject::connect(checking_the_operation_button13, &QPushButton::clicked, handleButtonClick13);
        checking_the_operation->setItem(12, 0, new QTableWidgetItem(parameters[12]));
        checking_the_operation->setCellWidget(12, 1, checking_the_operation_button13);

        // Строка 14
        QPushButton *checking_the_operation_button14 = new QPushButton("Запуск");
        checking_the_operation_button14->setObjectName("checking_the_operation_button14");
        QObject::connect(checking_the_operation_button14, &QPushButton::clicked, handleButtonClick14);
        checking_the_operation->setItem(13, 0, new QTableWidgetItem(parameters[13]));
        checking_the_operation->setCellWidget(13, 1, checking_the_operation_button14);

        // Строка 15
        QPushButton *checking_the_operation_button15 = new QPushButton("Запуск");
        checking_the_operation_button15->setObjectName("checking_the_operation_button15");
        QObject::connect(checking_the_operation_button15, &QPushButton::clicked, handleButtonClick15);
        checking_the_operation->setItem(14, 0, new QTableWidgetItem(parameters[14]));
        checking_the_operation->setCellWidget(14, 1, checking_the_operation_button15);

        // Подгоняем ширину столбцов под содержимое
        checking_the_operation->resizeColumnsToContents();

        // Автоматически растягиваем последний столбец на всю ширину
        checking_the_operation->horizontalHeader()->setStretchLastSection(true);

    }

    QWidget *widget_2 = w.findChild<QWidget*>("widget_2");
    QVBoxLayout *layout = widget_2->findChild<QVBoxLayout*>("verticalLayout_6");

    // Создаем кнопку и добавляем её в layout
    QPushButton *handleStartButton = new QPushButton("Старт", widget_2);

    // Добавляем кнопку в layout
    layout->addWidget(handleStartButton);
    int size = 80;

    handleStartButton->setFixedSize(size, size);

    // Устанавливаем кнопку растянутой во всю высоту и ширину
    layout->setStretchFactor(handleStartButton, 1); // Фактор растяжения для кнопки

    // Подключаем сигнал на слот handleStartButtonClick в MainWindow
    QObject::connect(handleStartButton, &QPushButton::clicked, &w, &handleStartButtonClick);

    QHBoxLayout* horizontalLayout_8 = w.findChild<QHBoxLayout*>("horizontalLayout_8");

    // Создаем кнопку
    QPushButton* button = new QPushButton("Выбор литеры");

    // Создаем меню с чекбоксами
    QMenu* checkboxMenu = createCheckboxMenu(&w);
    button->setMenu(checkboxMenu);  // Привязываем меню к кнопке

    // Добавляем кнопку в горизонтальный layout
    if (horizontalLayout_8) {
        horizontalLayout_8->addWidget(button);
    }

    // Пример: Проверка состояния чекбоксов
    // Например, проверка состояния после нажатия кнопки
    QObject::connect(button, &QPushButton::clicked, [&]() {
        checkCheckboxStates(&w);
    });


    w.show();
    return a.exec();
}


