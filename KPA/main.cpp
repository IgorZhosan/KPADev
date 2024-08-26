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

// Объявляем глобальные переменные для чекбоксов
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

// Функция для создания группы чекбоксов с именами
QGroupBox* createCheckboxGroup(const QString& title, const QStringList& labels, const QStringList& objectNames, QList<QCheckBox*>& checkboxes) {
    QGroupBox* groupBox = new QGroupBox(title);  // Создаем группу с заголовком
    QHBoxLayout* hbox = new QHBoxLayout;  // Горизонтальный layout для чекбоксов

    // Создаем чекбоксы с названиями из списка labels и присваиваем им objectName
    for (int i = 0; i < labels.size(); ++i) {
        QCheckBox* checkbox = new QCheckBox(labels[i]);  // Чекбокс с текстом
        checkbox->setObjectName(objectNames[i]);  // Устанавливаем уникальное имя для каждого чекбокса
        checkboxes.append(checkbox);  // Добавляем чекбокс в список
        hbox->addWidget(checkbox);  // Добавляем чекбокс в layout
    }

    groupBox->setLayout(hbox);  // Устанавливаем layout в группу
    return groupBox;
}

// Функция для создания меню с чекбоксами
QMenu* createCheckboxMenu(QWidget* parent) {
    QMenu* menu = new QMenu(parent);

    // Создаем группы чекбоксов для остальных чекбоксов с нужными именами
    QStringList otherCheckboxLabels = {"Отказ канала", "ПО", "K0", "ТК", "КВС", "ТСН", "НКК", "МГн", "ПП", "ППП", "НК", "ИТП", "ПР"};
    QStringList otherCheckboxNamesOut = {"out_otkaz", "out_po", "out_k0", "out_tk", "out_kvs", "out_tsn", "out_nkk", "out_mgn", "out_pp", "out_ppp", "out_nk", "out_itp", "out_pr"};

    // Листы для чекбоксов
    QList<QCheckBox*> outCheckboxes;

    // Создаем группу для отправляемых данных
    QGroupBox* outGroup = createCheckboxGroup("Прочие (отправляемые)", otherCheckboxLabels, otherCheckboxNamesOut, outCheckboxes);

    // Сохраняем ссылки на чекбоксы в глобальные переменные
    out_otkaz_checkbox = outCheckboxes[0];
    out_po_checkbox = outCheckboxes[1];
    out_k0_checkbox = outCheckboxes[2];
    out_tk_checkbox = outCheckboxes[3];
    out_kvs_checkbox = outCheckboxes[4];
    out_tsn_checkbox = outCheckboxes[5];
    out_nkk_checkbox = outCheckboxes[6];
    out_mgn_checkbox = outCheckboxes[7];
    out_pp_checkbox = outCheckboxes[8];
    out_ppp_checkbox = outCheckboxes[9];
    out_nk_checkbox = outCheckboxes[10];
    out_itp_checkbox = outCheckboxes[11];
    out_pr_checkbox = outCheckboxes[12];

    // Упаковываем группы в действия меню
    QWidgetAction* outAction = new QWidgetAction(menu);
    outAction->setDefaultWidget(outGroup);
    menu->addAction(outAction);

    return menu;
}

// Функция для обработки изменения состояния чекбокса
void setupOutNkkCheckboxBehavior() {
    if (out_nkk_checkbox) {
        QObject::connect(out_nkk_checkbox, &QCheckBox::stateChanged, [](int state) {
            if (state == Qt::Checked) {
                if (terminal_up) {
                    terminal_up->append("Чекбокс out_nkk нажат");
                } else {
                    qDebug() << "Чекбокс out_nkk нажат";
                }
            } else {
                if (terminal_up) {
                    terminal_up->append("Чекбокс out_nkk снят");
                } else {
                    qDebug() << "Чекбокс out_nkk снят";
                }
            }
        });
    } else {
        qDebug() << "Чекбокс out_nkk не найден";
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

    QPushButton *handleStartButton = new QPushButton("Старт", widget_2);
    layout->addWidget(handleStartButton);
    handleStartButton->setFixedSize(80, 80);
    layout->setStretchFactor(handleStartButton, 1);

    QObject::connect(handleStartButton, &QPushButton::clicked, &w, &handleStartButtonClick);

    QHBoxLayout* horizontalLayout_8 = w.findChild<QHBoxLayout*>("horizontalLayout_8");

    QPushButton* button = new QPushButton("Выбор литеры");
    QMenu* checkboxMenu = createCheckboxMenu(&w);
    button->setMenu(checkboxMenu);

    if (horizontalLayout_8) {
        horizontalLayout_8->addWidget(button);
    }

    w.show();
    return a.exec();
}
