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
#include <QLabel>

// Глобальные переменные для всех элементов интерфейса
QTextEdit *terminal_down = nullptr;
QTextEdit *terminal_up = nullptr;
QTableWidget *turning_on_the_equipment = nullptr;
QTableWidget *checking_the_operation = nullptr;
QMenu *checkboxMenu = nullptr;
QPushButton *handleStartButton = nullptr;
QPushButton *button = nullptr;
QPushButton *pushButton_3 = nullptr;

// Добавляем глобальные переменные для чекбоксов ЛТ
QCheckBox *out_lt1_checkbox = nullptr;
QCheckBox *out_lt2_checkbox = nullptr;
QCheckBox *out_lt3_checkbox = nullptr;
QCheckBox *out_lt4_checkbox = nullptr;
QCheckBox *out_lt5_checkbox = nullptr;
QCheckBox *out_lt6_checkbox = nullptr;
QCheckBox *out_lt7_checkbox = nullptr;

// Объявляем глобальные переменные для других чекбоксов
QCheckBox *out_otkaz_checkbox = nullptr;
QCheckBox *out_po_checkbox = nullptr;
QCheckBox *out_k0_checkbox = nullptr;
QCheckBox *out_tk_checkbox = nullptr;
QCheckBox *out_kvs_checkbox = nullptr;
QCheckBox *out_tsn_checkbox = nullptr;
QCheckBox *out_nkk_checkbox = nullptr;
QCheckBox *out_mgn_checkbox = nullptr;
QCheckBox *out_pp_checkbox = nullptr;
QCheckBox *out_ppp_checkbox = nullptr;
QCheckBox *out_nk_checkbox = nullptr;
QCheckBox *out_itp_checkbox = nullptr;
QCheckBox *out_pr_checkbox = nullptr;
QCheckBox *out_lk1_checkbox = nullptr;
QCheckBox *out_lk2_checkbox = nullptr;
QCheckBox *out_lk3_checkbox = nullptr;
QCheckBox *out_lk4_checkbox = nullptr;
QCheckBox *out_kk1_checkbox = nullptr;
QCheckBox *out_kk2_checkbox = nullptr;
QCheckBox *out_kk3_checkbox = nullptr;
QCheckBox *out_kk4_checkbox = nullptr;
QCheckBox *out_kk5_checkbox = nullptr;
QCheckBox *out_kk6_checkbox = nullptr;
QCheckBox *out_kk7_checkbox = nullptr;
QCheckBox *out_kk8_checkbox = nullptr;


// Функция для обработки изменения состояния чекбоксов
void handleCheckboxStateChange(QList<QCheckBox*> checkboxes, QCheckBox* selectedCheckbox) {
    // Проходим по всем чекбоксам и снимаем отметку с тех, которые не были выбраны
    for (QCheckBox* checkbox : checkboxes) {
        if (checkbox != selectedCheckbox) {
            checkbox->setChecked(false);
        }
    }
}

// Функция для создания группы чекбоксов с именами и горизонтальным расположением
QGroupBox* createCheckboxGroup(const QString& title, const QStringList& labels, const QStringList& objectNames, QList<QCheckBox*>& checkboxes) {
    QGroupBox* groupBox = new QGroupBox(title);  // Создаем группу с заголовком
    QHBoxLayout* hbox = new QHBoxLayout;  // Горизонтальный layout для чекбоксов

    // Создаем чекбоксы с названиями из списка labels и присваиваем им objectName
    for (int i = 0; i < labels.size(); ++i) {
        QCheckBox* checkbox = new QCheckBox(labels[i]);  // Чекбокс с текстом
        checkbox->setObjectName(objectNames[i]);  // Устанавливаем уникальное имя для каждого чекбокса
        checkboxes.append(checkbox);  // Добавляем чекбокс в список
        hbox->addWidget(checkbox);  // Добавляем чекбокс в горизонтальный layout
    }

    groupBox->setLayout(hbox);  // Устанавливаем layout в группу
    return groupBox;
}

// Функция для создания меню с чекбоксами
QMenu* createCheckboxMenu(QWidget* parent) {
    QMenu* menu = new QMenu(parent);

    // Группа ЛТ [7:1]
    QStringList ltCheckboxLabels = {"ЛТ7", "ЛТ6", "ЛТ5", "ЛТ4", "ЛТ3", "ЛТ2", "ЛТ1"};
    QStringList ltCheckboxNamesOut = {"out_lt7", "out_lt6", "out_lt5", "out_lt4", "out_lt3", "out_lt2", "out_lt1"};
    QList<QCheckBox*> ltCheckboxes;
    QGroupBox* ltGroup = createCheckboxGroup("ЛТ [7:1]", ltCheckboxLabels, ltCheckboxNamesOut, ltCheckboxes);

    // Группа ЛК [4:1]
    QStringList lkCheckboxLabels = {"ЛК4", "ЛК3", "ЛК2", "ЛК1"};
    QStringList lkCheckboxNamesOut = {"out_lk4", "out_lk3", "out_lk2", "out_lk1"};
    QList<QCheckBox*> lkCheckboxes;
    QGroupBox* lkGroup = createCheckboxGroup("ЛК [4:1]", lkCheckboxLabels, lkCheckboxNamesOut, lkCheckboxes);

    // Группа КК [8:1]
    QStringList kkCheckboxLabels = {"КК8", "КК7", "КК6", "КК5", "КК4", "КК3", "КК2", "КК1"};
    QStringList kkCheckboxNamesOut = {"out_kk8", "out_kk7", "out_kk6", "out_kk5", "out_kk4", "out_kk3", "out_kk2", "out_kk1"};
    QList<QCheckBox*> kkCheckboxes;
    QGroupBox* kkGroup = createCheckboxGroup("КК [8:1]", kkCheckboxLabels, kkCheckboxNamesOut, kkCheckboxes);

    // Сохраняем ссылки на чекбоксы в глобальные переменные
    out_lt1_checkbox = ltCheckboxes[6];
    out_lt2_checkbox = ltCheckboxes[5];
    out_lt3_checkbox = ltCheckboxes[4];
    out_lt4_checkbox = ltCheckboxes[3];
    out_lt5_checkbox = ltCheckboxes[2];
    out_lt6_checkbox = ltCheckboxes[1];
    out_lt7_checkbox = ltCheckboxes[0];

    out_lk1_checkbox = lkCheckboxes[3];
    out_lk2_checkbox = lkCheckboxes[2];
    out_lk3_checkbox = lkCheckboxes[1];
    out_lk4_checkbox = lkCheckboxes[0];

    out_kk1_checkbox = kkCheckboxes[7];
    out_kk2_checkbox = kkCheckboxes[6];
    out_kk3_checkbox = kkCheckboxes[5];
    out_kk4_checkbox = kkCheckboxes[4];
    out_kk5_checkbox = kkCheckboxes[3];
    out_kk6_checkbox = kkCheckboxes[2];
    out_kk7_checkbox = kkCheckboxes[1];
    out_kk8_checkbox = kkCheckboxes[0];

    // Устанавливаем начальное состояние по умолчанию
    out_lt4_checkbox->setChecked(true);  // ЛТ4
    out_lk2_checkbox->setChecked(true);  // ЛК2
    out_kk4_checkbox->setChecked(true);  // КК4

    // Добавляем обработчики для группы ЛТ
    for (QCheckBox* checkbox : ltCheckboxes) {
        QObject::connect(checkbox, &QCheckBox::clicked, [=]() {
            handleCheckboxStateChange(ltCheckboxes, checkbox);
        });
    }

    // Добавляем обработчики для группы ЛК
    for (QCheckBox* checkbox : lkCheckboxes) {
        QObject::connect(checkbox, &QCheckBox::clicked, [=]() {
            handleCheckboxStateChange(lkCheckboxes, checkbox);
        });
    }

    // Добавляем обработчики для группы КК
    for (QCheckBox* checkbox : kkCheckboxes) {
        QObject::connect(checkbox, &QCheckBox::clicked, [=]() {
            handleCheckboxStateChange(kkCheckboxes, checkbox);
        });
    }

    // Упаковываем группы в горизонтальный layout
    QWidget* widget = new QWidget(menu);
    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->addWidget(ltGroup);
    layout->addWidget(lkGroup);
    layout->addWidget(kkGroup);

    widget->setLayout(layout);

    // Упаковываем layout в меню
    QWidgetAction* widgetAction = new QWidgetAction(menu);
    widgetAction->setDefaultWidget(widget);
    menu->addAction(widgetAction);

    return menu;
}
void setupCheckboxSignals() {
        // Список всех чекбоксов
        QList<QCheckBox*> checkboxes = {
            out_otkaz_checkbox, out_po_checkbox, out_k0_checkbox, out_tk_checkbox,
            out_kvs_checkbox, out_tsn_checkbox, out_nkk_checkbox, out_mgn_checkbox,
            out_pp_checkbox, out_ppp_checkbox, out_nk_checkbox, out_itp_checkbox, out_pr_checkbox,
            out_lk1_checkbox, out_lk2_checkbox, out_lk3_checkbox, out_lk4_checkbox,
            out_kk1_checkbox, out_kk2_checkbox, out_kk3_checkbox, out_kk4_checkbox,
            out_kk5_checkbox, out_kk6_checkbox, out_kk7_checkbox, out_kk8_checkbox
        };
        for (auto* checkbox : checkboxes) {
            if (checkbox) {
                QObject::connect(checkbox, &QCheckBox::stateChanged, [checkbox](int state) {
                  //  QString stateStr = (state == Qt::Checked) ? "checked" : "unchecked";
                  //  if (terminal_down) {
                  //      terminal_down->append(checkbox->objectName() + " is " + stateStr);
                 //   }
                });
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");

    // Доступ к таблице через класс MainWindow, объект интерфейса создается внутри MainWindow
    terminal_down = w.findChild<QTextEdit*>("terminal_down");
    QTableWidget *turning_on_the_equipment = w.findChild<QTableWidget*>("turning_on_the_equipment");

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
            totalHeight += turning_on_the_equipment->rowHeight(row);
        }
        totalHeight += turning_on_the_equipment->horizontalHeader()->height();
        turning_on_the_equipment->setMinimumHeight(totalHeight);
        turning_on_the_equipment->setMaximumHeight(totalHeight);
    }

    QTextEdit *terminal = w.findChild<QTextEdit*>("terminal_up");

    if (terminal) {
        qDebug() << "Текущий каталог:" << QDir::currentPath();
        QString filePath = "C:\\KPADev\\KPA\\GUI\\Controlinfo.txt";
        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(&w, "Ошибка", "Не удалось открыть файл " + filePath);
            return -1;
        }

        QStringDecoder decoder(QStringConverter::Utf8);
        QString fileContent = decoder.decode(file.readAll());
        file.close();
        terminal->setPlainText(fileContent);
    }

    QTableWidget *checking_the_operation = w.findChild<QTableWidget*>("checking_the_operation");

    if (checking_the_operation) {
        checking_the_operation->setSelectionMode(QAbstractItemView::NoSelection);
        checking_the_operation->setSelectionBehavior(QAbstractItemView::SelectItems);
        checking_the_operation->setRowCount(15);
        checking_the_operation->setColumnCount(4);

        QStringList headers = { "Наименование параметра", "Запуск проверки", "Индикация выполнения", "Дополнительная информация" };
        checking_the_operation->setHorizontalHeaderLabels(headers);

        QStringList parameters = {
            "Поле зрения", "Разрешающая способность", "Время вывода \"Т1А готов\" после команды \"Подготовка\"",
            "Градация яркости", "Замещение служебной информации", "Углы разворота ЛВ", "Автосопровождение",
            "Информационный обмен с АД.9М2", "Информационный обмен с ГБМ", "Передача телеметрических данных",
            "Прием команды по КРЛ", "Передача видео по ТРЛ", "Внешнее управление антенной", "Уход антенны", "Зона захвата антенны"
        };

        for (int i = 0; i < parameters.size(); ++i) {
            QPushButton *button = new QPushButton("Запуск");
            button->setObjectName(QString("checking_the_operation_button%1").arg(i + 1));
            checking_the_operation->setItem(i, 0, new QTableWidgetItem(parameters[i]));
            checking_the_operation->setCellWidget(i, 1, button);
            if (i % 2 == 0) {
                checking_the_operation->setItem(i, 3, new QTableWidgetItem("Визуальный контроль по изображению"));
            }
        }

        checking_the_operation->resizeColumnsToContents();
        checking_the_operation->horizontalHeader()->setStretchLastSection(true);
    }

    QWidget *widget_2 = w.findChild<QWidget*>("widget_2");
    QVBoxLayout *layout = widget_2->findChild<QVBoxLayout*>("verticalLayout_6");

    handleStartButton = new QPushButton("Старт", widget_2);
    layout->addWidget(handleStartButton);
    handleStartButton->setFixedSize(80, 80);
    layout->setStretchFactor(handleStartButton, 1);



    QHBoxLayout* horizontalLayout_8 = w.findChild<QHBoxLayout*>("horizontalLayout_8");

    QPushButton* button = new QPushButton("Выбор литеры");
    QMenu* checkboxMenu = createCheckboxMenu(&w);
    button->setMenu(checkboxMenu);

    if (horizontalLayout_8) {
        horizontalLayout_8->addWidget(button);
    }
    setupCheckboxSignals();

    QObject::connect(handleStartButton, &QPushButton::clicked, &w, &handleStartButtonClick);
    pushButton_3 = w.findChild<QPushButton*>("pushButton_3");
    QObject::connect(pushButton_3, &QPushButton::clicked, &on_pushButton_3_clicked);
    w.show();

    return a.exec();
}
