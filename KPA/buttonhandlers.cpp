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
#include <QKeySequence>
#include <QShortcut>
#include <QWidget>
#include <QEvent>
#include <QKeyEvent>

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
// Глобальные переменные для логики "8 раз отправить 0x0X4040"
////////////////////////////////////////////////////////////////////////
int g_commandCounter = 0;         // Сколько раз ещё отправлять
unsigned char g_commandDigit = 0; // Какая "X" в 0x0X4040
unsigned char g_toggleStateF = 6;
unsigned char g_toggleState7 = 0x09;
static unsigned char g_toggleState8 = 0x0B;
static bool g_isAPressed  = false; // Флаг, что A в данный момент зажата
static int  g_aPressCount = 0;     // Счётчик нажатий


static bool isNkkSet()
{
    // Если (1<<16) в OUT_AD9M2[0], значит НКК есть
    return ((OUT_AD9M2[0] & (1UL << 16)) != 0);
}

// === (1) Отправка 0x004040 (8 раз) — в конце оставляем 0x004040
static void send004040_8Times()
{
    // Можно не восстанавливать старое значение — в итоге оно
    // и останется «0x004040».
    for (int i = 0; i < 8; i++)
    {
        // Зануляем биты [23..8] и ставим 0x4040
        // Если нужно сохранять [31..24] и [7..0], меняйте логику.
        OUT_KPA[0] = (OUT_KPA[0] & 0xFF0000FF)
                     | (0x40UL << 16)
                     | (0x40UL << 8);

        OUT_KPA[1] = 0x80 | (OUT_KPA[0] & 0xFFFFFF00);

        BUF256x32_write(1, OUT_KPA, 2);
        SO_pusk(1);
    }
    // После цикла OUT_KPA[0] уже с 0x4040 в [15..8] и [23..16].
    // Больше ничего не делаем — «0x004040» и останется.
}

// === (2) Отправка 0x004747 (8 раз) — в конце восстанавливаем 0x004040
static void send004747_8Times()
{
    unsigned long oldValue = OUT_KPA[0];

    for (int i = 0; i < 16; i++)
    {
        // В младшие 16 бит ставим 0x4747,
        // при желании сохраняя [31..24] и [7..0].
        OUT_KPA[0] = (OUT_KPA[0] & 0xFF0000FF)
                     | (0x47UL << 16)
                     | (0x47UL << 8);

        OUT_KPA[1] = 0x80 | (OUT_KPA[0] & 0xFFFFFF00);

        BUF256x32_write(1, OUT_KPA, 2);
        SO_pusk(1);
    }

    // В конце возвращаем «0x004040»
    OUT_KPA[0] = (OUT_KPA[0] & 0xFF0000FF)
                 | (0x40UL << 16)
                 | (0x40UL << 8);

    OUT_KPA[1] = 0x80 | (OUT_KPA[0] & 0xFFFFFF00);
    BUF256x32_write(1, OUT_KPA, 2);
    SO_pusk(1);
}

// === (3) Отправка nibble=0x0C (8 раз), затем возвращаем 0x004040
static void sendNibble0C_8Times()
{
    // 8 раз nibble=0x0C
    for (int i = 0; i < 16; i++)
    {
        // Ставим 0x0C в [27..24],
        // остальные биты оставляем
        OUT_KPA[0] = (OUT_KPA[0] & 0xF0FFFFFFUL)
                     | (0x0CUL << 24);

        OUT_KPA[1] = 0x80 | (OUT_KPA[0] & 0xFFFFFF00);

        BUF256x32_write(1, OUT_KPA, 2);
        SO_pusk(1);
    }

    // После этих 8 отправок теперь ставим опять 0x4040 в младшие 16 бит
    // (а биты [27..24] можем вернуть в 0, если нужно).
    OUT_KPA[0] = (OUT_KPA[0] & 0xF00000FFUL) // обнуляем [27..24], сохраняем [31..28], [7..0]
                 | (0x40UL << 16)
                 | (0x40UL << 8);

    OUT_KPA[1] = 0x80 | (OUT_KPA[0] & 0xFFFFFF00);
    BUF256x32_write(1, OUT_KPA, 2);
    SO_pusk(1);
}

// === Фильтр клавиши A ===
class AKeyEventFilter : public QObject
{
public:
    explicit AKeyEventFilter(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

protected:
    bool eventFilter(QObject* obj, QEvent* event) override
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* ke = static_cast<QKeyEvent*>(event);
            if (ke->key() == Qt::Key_A && !ke->isAutoRepeat())
            {
                if (!isNkkSet()) {
                    return false;
                }

                if (!g_isAPressed)
                {
                    g_isAPressed = true;
                    g_aPressCount++;

                    // 2-е и последующие нажатия:
                    if (g_aPressCount > 1)
                    {
                        // 8 раз 0x4040
                        send004040_8Times();
                        // 8 раз 0x4747 (в конце -> 0x4040)
                        send004747_8Times();

                    }
                    // На первое нажатие — ничего
                }
                return false;
            }
        }
        else if (event->type() == QEvent::KeyRelease)
        {
            QKeyEvent* ke = static_cast<QKeyEvent*>(event);
            if (ke->key() == Qt::Key_A && !ke->isAutoRepeat())
            {
                if (!isNkkSet()) {
                    return false;
                }
                g_isAPressed = false;

                // При отпускании => 8 раз nibble=0x0C,
                // затем возвращаемся к 0x4040
                sendNibble0C_8Times();

                return false;
            }
        }
        return QObject::eventFilter(obj, event);
    }
};

void installKeyAEventFilter(QWidget* parent)
{
    parent->setFocusPolicy(Qt::StrongFocus);

    static AKeyEventFilter* filter = nullptr;
    if (!filter)
    {
        filter = new AKeyEventFilter(parent);
        parent->installEventFilter(filter);
    }
}


// Клавиша 'Z': отправить 0x0D
void handleKeyZ()
{
  //  qDebug() << "handleKeyZ => nibble=0x0D";
    if (!isNkkSet()) {
     //   qDebug() << "NKK is not set => skipping 0x0D";
        return;
    }

    g_commandDigit = 0x0D;
    g_commandCounter = 8;
}

// Клавиша 'S': отправить 0x0F
void handleKeyS()
{
  //  qDebug() << "handleKeyS => nibble=0x0E";
    if (!isNkkSet()) {
      //  qDebug() << "NKK is not set => skipping 0x0E";
        return;
    }

    g_commandDigit = 0x0F;
    g_commandCounter = 8;
}

// Клавиша '0': всегда посылаем nibble=0x08
void handleKey0()
{

    if (!isNkkSet()) {
      //  qDebug() << "NKK not set => skipping 0x08";
        return;
    }

    g_commandDigit = 0x08;       // nibble [27..24] = 8
    g_commandCounter = 8;        // отправить 8 раз
}

// Клавиша '7': переключаемся между 0x09 и 0x0A
void handleKey7()
{
  //  qDebug() << "handleKey7 pressed => toggling between 0x09 and 0x0A";

    if (!isNkkSet()) {
     //   qDebug() << "NKK not set => skipping toggle 7";
        return;
    }

    // Если текущее состояние = 0x09, то теперь 0x0A, иначе 0x09
    unsigned char newVal = (g_toggleState7 == 0x09) ? 0x0A : 0x09;
    g_toggleState7 = newVal; // запоминаем, чтобы в следующий раз переключить обратно

    g_commandDigit = newVal;
    g_commandCounter = 8;
}

void handleKey8()
{
    if (!isNkkSet()) {
        return;
    }

    // Если текущее состояние = 0x0B, переключаемся на 0x0E; иначе обратно
    unsigned char newVal = (g_toggleState8 == 0x0B) ? 0x0E : 0x0B;
    g_toggleState8 = newVal; // запоминаем для следующего нажатия

    g_commandDigit   = newVal;  // посылаем это значение в биты [27..24]
    g_commandCounter = 8;       // 8 раз отправить
}

void handleKeyF()
{
    // При нажатии F — переключаем с 6 на 7 или обратно:
    // Простейший способ — если =6 => делаем 7, иначе 6
    unsigned char newDigit = (g_toggleStateF == 6) ? 7 : 6;

    // Запоминаем в g_toggleStateF для следующего раза
    g_toggleStateF = newDigit;

    // Проверяем, установлен ли бит [27..24], если нужно
    // Но вы говорили «нужно проверить НКК». Если у вас isNkkSet()
    // зависит от OUT_AD9M2[0], окей:
    if (!isNkkSet()) {
        return;
    }

    // Запускаем тот же механизм, что и при «1..5»
    g_commandCounter = 8;
    g_commandDigit   = newDigit;
}

void setupAdditionalShortcuts(QWidget* parent)
{
    // Клавиша '0'
    auto key0 = new QShortcut(QKeySequence(Qt::Key_0), parent);
    key0->setContext(Qt::ApplicationShortcut);
    QObject::connect(key0, &QShortcut::activated, [](){
        handleKey0();
    });

    // Клавиша '7'
    auto key7 = new QShortcut(QKeySequence(Qt::Key_7), parent);
    key7->setContext(Qt::ApplicationShortcut);
    QObject::connect(key7, &QShortcut::activated, [](){
        handleKey7();
    });

    // Клавиша '8'
    auto key8 = new QShortcut(QKeySequence(Qt::Key_8), parent);
    key8->setContext(Qt::ApplicationShortcut);
    QObject::connect(key8, &QShortcut::activated, [](){
        handleKey8();
    });

    // Клавиша 'S'
    auto keyS = new QShortcut(QKeySequence(Qt::Key_S), parent);
    keyS->setContext(Qt::ApplicationShortcut);
    QObject::connect(keyS, &QShortcut::activated, [](){
        // Вызываем логику обработки нажатия S
        handleKeyS();
    });
}

void setupToggleFShortcut(QWidget* parent)
{
    auto keyF = new QShortcut(QKeySequence(Qt::Key_F), parent);
    keyF->setContext(Qt::ApplicationShortcut);

    QObject::connect(keyF, &QShortcut::activated, [](){
        handleKeyF();
    });
}

////////////////////////////////////////////////////////////////////////
// Функция при нажатии любой цифры '1'..'5'
////////////////////////////////////////////////////////////////////////
void handleDigitKey(int digit)
{
    if (!isNkkSet()) return;

    g_commandCounter = 8;
    g_commandDigit   = (digit & 0xFF);
}

////////////////////////////////////////////////////////////////////////
// Горячие клавиши (кроме стрелок) - '1'..'5'
////////////////////////////////////////////////////////////////////////
void setupDigitShortcuts(QWidget* parent)
{
    // Пример: клавиши '1'..'5'
    auto key1 = new QShortcut(QKeySequence(Qt::Key_1), parent);
    key1->setContext(Qt::ApplicationShortcut);
    QObject::connect(key1, &QShortcut::activated, [](){
        handleDigitKey(1);
    });

    auto key2 = new QShortcut(QKeySequence(Qt::Key_2), parent);
    key2->setContext(Qt::ApplicationShortcut);
    QObject::connect(key2, &QShortcut::activated, [](){
        handleDigitKey(2);
    });

    auto key3 = new QShortcut(QKeySequence(Qt::Key_3), parent);
    key3->setContext(Qt::ApplicationShortcut);
    QObject::connect(key3, &QShortcut::activated, [](){
        handleDigitKey(3);
    });

    auto key4 = new QShortcut(QKeySequence(Qt::Key_4), parent);
    key4->setContext(Qt::ApplicationShortcut);
    QObject::connect(key4, &QShortcut::activated, [](){
        handleDigitKey(4);
    });

    auto key5 = new QShortcut(QKeySequence(Qt::Key_5), parent);
    key5->setContext(Qt::ApplicationShortcut);
    QObject::connect(key5, &QShortcut::activated, [](){
        handleDigitKey(5);
    });
}

////////////////////////////////////////////////////////////////////////
// Функция, вызываемая каждые 40 мс в Timer_Event()
////////////////////////////////////////////////////////////////////////
void checkAndSendDigitCommand()
{
    // Если счётчик = 0, значит ничего не делаем.
    if (g_commandCounter <= 0)
        return;

    // 1) Формируем биты [27..24] = g_commandDigit
    //    (предполагаем, что g_commandDigit <= 0x0F).
    //
    //    Для примера, если у вас логика «1..5»,
    //    то (g_commandDigit << 24) поместит эту цифру
    //    в биты [27..24], а [31..28] станут 0.
    //
    // 2) Предварительно обнуляем те биты, чтобы вставить новые:
    //    ~(0x0F << 24) = 0xF0FFFFFF
    //    Это «очистит» биты [27..24], оставив всё остальное без изменений.
    //
    // ИТОГ:
    //    (OUT_KPA[0] & 0xF0FFFFFF) => обнуляет nibble [27..24]
    //    | ( (g_commandDigit & 0x0F) << 24) => вставляет digit

    OUT_KPA[0] &= 0xF0FFFFFF; // обнуляем биты [27..24]
    OUT_KPA[0] |= ((static_cast<ULONG>(g_commandDigit) & 0x0F) << 24);

    // Теперь OUT_KPA[0] содержит обновлённые биты [27..24],
    // а другие биты (например, [23..0]) остаются нетронутыми.

    // (Если вам нужно *обязательно* иметь 0x40 в байтах [15..8] и [7..0],
    //  то убедитесь, что при инициализации OUT_KPA[0] там уже 0x4040
    //  либо используйте аналогичный подход для их установки.)

    // Формируем второе слово, если требуется:
    OUT_KPA[1] = 0x80 | (OUT_KPA[0] & 0xFFFFFF00);

    // Отправляем в канал 2
    BUF256x32_write(1, OUT_KPA, 2);
    SO_pusk(1);

    // Уменьшаем счётчик (всего нужно 8 раз)
    g_commandCounter--;

    // Если счётчик дошёл до 0, «сбрасываем» только биты [27..24] (цифру),
    // не трогая нижние байты [15..0].
    // То есть убираем digit, возвращая nibble [27..24] в 0:
    if (g_commandCounter == 0)
    {
        OUT_KPA[0] &= 0xF0FFFFFF;   // Снова обнуляем [27..24]
        OUT_KPA[1] = 0x80 | (OUT_KPA[0] & 0xFFFFFF00);

        BUF256x32_write(1, OUT_KPA, 2);
        SO_pusk(1);
    }
}

////////////////////////////////////////////////////////////////////////
//           Горячие клавиши стрелок + Reset (клавиша 'O')
////////////////////////////////////////////////////////////////////////
void setupArrowShortcuts(QWidget* parent)
{
    // Чтобы стрелки работали по всему приложению, а не только при фокусе
    auto upShortcut = new QShortcut(QKeySequence(Qt::Key_Up), parent);
    upShortcut->setContext(Qt::ApplicationShortcut);
    QObject::connect(upShortcut, &QShortcut::activated, [](){
        arrowUp();
    });

    auto downShortcut = new QShortcut(QKeySequence(Qt::Key_Down), parent);
    downShortcut->setContext(Qt::ApplicationShortcut);
    QObject::connect(downShortcut, &QShortcut::activated, [](){
        arrowDown();
    });

    auto leftShortcut = new QShortcut(QKeySequence(Qt::Key_Left), parent);
    leftShortcut->setContext(Qt::ApplicationShortcut);
    QObject::connect(leftShortcut, &QShortcut::activated, [](){
        arrowLeft();
    });

    auto rightShortcut = new QShortcut(QKeySequence(Qt::Key_Right), parent);
    rightShortcut->setContext(Qt::ApplicationShortcut);
    QObject::connect(rightShortcut, &QShortcut::activated, [](){
        arrowRight();
    });

    auto oShortcut = new QShortcut(QKeySequence(Qt::Key_O), parent);
    oShortcut->setContext(Qt::ApplicationShortcut);
    QObject::connect(oShortcut, &QShortcut::activated, [](){
        arrowResetToStart();
    });
}

////////////////////////////////////////////////////////////////////////
//  Извлекаем YMUPIN и ZMUPIN из OUT_KPA[0]:
//  YMUPIN = [15..8],  ZMUPIN = [23..16].
////////////////////////////////////////////////////////////////////////
static void getYmuZmu(unsigned char &ymu, unsigned char &zmu)
{
    unsigned long val = OUT_KPA[0];
    // YMUPIN = биты [15..8]
    ymu = static_cast<unsigned char>((val >> 8) & 0xFF);
    // ZMUPIN = биты [23..16]
    zmu = static_cast<unsigned char>((val >> 16) & 0xFF);
}

////////////////////////////////////////////////////////////////////////
//  Записываем YMUPIN и ZMUPIN обратно в OUT_KPA[0],
//  не трогая биты [7..0] и [31..24].
//  Затем формируем OUT_KPA[1] = 0x80 | (OUT_KPA[0] & 0xFFFFFF00).
//  И отправляем оба слова на канал 2.
////////////////////////////////////////////////////////////////////////
static void setYmuZmuAndSend(unsigned char ymu, unsigned char zmu)
{
    // 1) Сохраняем биты [7..0]
    unsigned long oldLow  = (OUT_KPA[0] & 0x000000FF);

    // 2) Сохраняем биты [31..24]
    unsigned long oldHigh = (OUT_KPA[0] & 0xFF000000);

    // 3) Собираем новые биты для [15..8] (ymu) и [23..16] (zmu)
    unsigned long newVal = oldLow
                           | ((static_cast<unsigned long>(ymu) & 0xFF) << 8)
                           | ((static_cast<unsigned long>(zmu) & 0xFF) << 16);

    // 4) Склеиваем со старыми [31..24]
    OUT_KPA[0] = oldHigh | newVal;

    // 5) Формируем второе слово (верхний байт = 0x80)
    OUT_KPA[1] = 0x80 | (OUT_KPA[0] & 0xFFFFFF00);

    // 6) Отправляем
    BUF256x32_write(1, OUT_KPA, 2);
    SO_pusk(1);
}

////////////////////////////////////////////////////////////////////////
//    Логика стрелок:
//    - Вверх  (Up)   => YMUPIN++ (макс 0x7F)
//    - Вниз   (Down) => YMUPIN-- (мин 0x00)
//    - Влево  (Left) => ZMUPIN-- (мин 0x00)
//    - Вправо (Right)=> ZMUPIN++ (макс 0x7F)
////////////////////////////////////////////////////////////////////////

// "Вверх": YMUPIN++
void arrowUp()
{
    if (!isNkkSet()) return;
    unsigned char ymu, zmu;
    getYmuZmu(ymu, zmu);

    if (ymu < 0x7F) {
        ++ymu;
    }
    setYmuZmuAndSend(ymu, zmu);
}

// "Вниз": YMUPIN--
void arrowDown()
{
    if (!isNkkSet()) return;
    unsigned char ymu, zmu;
    getYmuZmu(ymu, zmu);

    if (ymu > 0x00) {
        --ymu;
    }
    setYmuZmuAndSend(ymu, zmu);
}

// "Влево": ZMUPIN--
void arrowLeft()
{
    if (!isNkkSet()) return;
    unsigned char ymu, zmu;
    getYmuZmu(ymu, zmu);

    if (zmu > 0x00) {
        --zmu;
    }
    setYmuZmuAndSend(ymu, zmu);
}

// "Вправо": ZMUPIN++
void arrowRight()
{
    if (!isNkkSet()) return;
    unsigned char ymu, zmu;
    getYmuZmu(ymu, zmu);

    if (zmu < 0x7F) {
        ++zmu;
    }
    setYmuZmuAndSend(ymu, zmu);
}

////////////////////////////////////////////////////////////////////////
//  Клавиша 'O': сбрасываем YMUPIN=0x40, ZMUPIN=0x40
////////////////////////////////////////////////////////////////////////
void arrowResetToStart()
{
    if (!isNkkSet()) return;
    unsigned char ymu = 0x40;
    unsigned char zmu = 0x40;
    setYmuZmuAndSend(ymu, zmu);
}

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
    OUT_AD9M2[0] |= 0x1 << 20; // задаём НК
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

    if (!clicledStartbutton)
    {
        // --- 1. Сбрасываем фон (не трогаем тексты) ---
        if (turning_on_the_equipment)
        {
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
            if (auto item = turning_on_the_equipment->item(2, 2))
            {
                item->setText("Тгот = (Допуск (0-15)с)");
                item->setBackground(Qt::white);
            }
        }

        bool isDeviceConnected = false;
        bool isReceivingData   = false;

        // --- Подсвечиваем "Гот ТЕРМ." (0,1) синим на 1 сек ---
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

        // --- (1) Проверяем CH1 ---
        if (!State_ECE0206_0) {
            hECE0206_0 = OpenDeviceByIndex(0, &Error);
            if (hECE0206_0 == INVALID_HANDLE_VALUE) {
                State_ECE0206_0 = false;
                if (toolButton_14)
                    toolButton_14->setIcon(createCircleIcon(Qt::red));
                if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                    turning_on_the_equipment->item(1, 0)->setBackground(QColor(255, 0, 0));
            } else {
                DeviceIoControl(hECE0206_0, ECE02061_XP_SET_LONG_MODE, nullptr, 0, nullptr, 0, &nOutput, nullptr);
                DeviceIoControl(hECE0206_0, ECE02061_XP_GET_SERIAL_NUMBER, nullptr, 0, &bufOutput, 10, &nOutput, nullptr);

                s = "ARINC429_CH1  S\\N: " + QString::fromUtf8(reinterpret_cast<const char*>(bufOutput), 5);
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

        if (!State_ECE0206_1) {
            hECE0206_1 = OpenDeviceByIndex(1, &Error);
            if (hECE0206_1 == INVALID_HANDLE_VALUE) {
                State_ECE0206_1 = false;
                if (toolButton_15)
                    toolButton_15->setIcon(createCircleIcon(Qt::red));
                if (turning_on_the_equipment && turning_on_the_equipment->item(1, 0))
                    turning_on_the_equipment->item(1, 0)->setBackground(QColor(255, 0, 0));
            } else {
                DeviceIoControl(hECE0206_1, ECE02061_XP_SET_LONG_MODE, nullptr, 0, nullptr, 0, &nOutput, nullptr);
                DeviceIoControl(hECE0206_1, ECE02061_XP_GET_SERIAL_NUMBER, nullptr, 0, &bufOutput, 10, &nOutput, nullptr);

                s = "ARINC429_CH2  S\\N: " + QString::fromUtf8(reinterpret_cast<const char*>(bufOutput), 5);
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

        if (isDeviceConnected)
        {
            clicledStartbutton = true;
            g_aPressCount = 0;
            if (handleStartButton) {
                handleStartButton->setCheckable(true);
                handleStartButton->setChecked(true);
                handleStartButton->setText("Стоп");
            }

            // Запуск обмена
            Timer->start(40);
            QObject::connect(Timer, &QTimer::timeout, [=, &isDeviceConnected, &isReceivingData]() mutable {
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

            // "Т1А ГОТ" синий
            if (turning_on_the_equipment && turning_on_the_equipment->item(2, 0))
                turning_on_the_equipment->item(2, 0)->setBackground(QColor(0, 0, 255));

            // Разблокируем «Подготовка» через 120 мс
            timerPreparation->setInterval(120);
            timerPreparation->start();
            QObject::connect(timerPreparation, &QTimer::timeout, [=]() {
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

        // Если "Подготовка" была включена — отжимаем
        if (preparationButton && preparationButton->isChecked()) {
            preparationButton->setChecked(false);
            preparation(false);
        }

        // (I) Сбрасываем OUT_AD9M2, кроме литер
        resetOutAD9M2PreservingLiters();

        // (II) Сбрасываем "clickedButton1..15"
        resetAllClickedButtons();

        // (2) Очищаем таблицу "Проверка функционирования"
        if (checking_the_operation)
        {
            int rowCount = checking_the_operation->rowCount();
            for (int r = 0; r < rowCount; r++)
            {
                QTableWidgetItem* indicationItem = checking_the_operation->item(r, 2);
                if (!indicationItem) {
                    indicationItem = new QTableWidgetItem();
                    checking_the_operation->setItem(r, 2, indicationItem);
                }
                indicationItem->setText("");
                indicationItem->setBackground(Qt::white);

                QTableWidgetItem* infoItem = checking_the_operation->item(r, 3);
                if (!infoItem) {
                    infoItem = new QTableWidgetItem();
                    checking_the_operation->setItem(r, 3, infoItem);
                }
                infoItem->setText("");
                infoItem->setBackground(Qt::white);
            }
        }

        // (III) Убираем старые "ошибки" из GUI T1A
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
    // Синхронизируем clickedPreparation
    clickedPreparation = checked;

    g_aPressCount = 0;

    // Если нет readinessTimer, создаём
    if (!readinessTimer) {
        readinessTimer = new QTimer();
        // Периодический опрос (не singleShot)
        readinessTimer->setSingleShot(false);
    }

    if (checked)
    {
        // --- «Подготовка» включена ---
        // (A) Ставим бит 9
        OUT_AD9M2[0] |= (1UL << 9);

        ULONG sum = KS(OUT_AD9M2, 6);
        OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
        BUF256x32_write(0, OUT_AD9M2, 7);
        SO_pusk(0);

        // (B) Запоминаем время начала + крайний срок
        prepStartTime = QTime::currentTime();
        deadlineTime  = prepStartTime.addSecs(20);

        // (C) Настраиваем период 200 мс
        readinessTimer->stop();
        readinessTimer->setInterval(200);

        // (D) Отключаем предыдущие connect
        QObject::disconnect(readinessTimer, nullptr, nullptr, nullptr);

        // (E) Подключаем новый обработчик
        QObject::connect(readinessTimer, &QTimer::timeout, [=]() {
            // 1) Считываем IN_KPA (каждые 200 мс)
            receiveDataAndDisplay();

            // 2) Текущее время
            QTime now = QTime::currentTime();
            int msWaited = prepStartTime.msecsTo(now);
            double secWaited = msWaited / 1000.0;

            // 3) Проверка 20 с
            if (now >= deadlineTime)
            {
                // Останавливаем таймер
                readinessTimer->stop();

                // Ещё раз обновим IN_KPA
                receiveDataAndDisplay();

                // Смотрим бит28, ошибки
                unsigned long val2 = IN_KPA[2] & 0x7FFFFFFF;
                unsigned long val0 = IN_KPA[0] & 0x7FFFFFFF;

                bool bit28 = (val2 & (1UL << 28));
                bool bit27 = (val2 & (1UL << 27));
                bool bit26 = (val2 & (1UL << 26));
                bool bit25 = (val2 & (1UL << 25));
                bool bit24 = (val2 & (1UL << 24));
                bool bit30 = (val0 & (1UL << 30));

                QStringList errorList;
                if (bit27) errorList << "ДГ";
                if (bit26) errorList << "KRL";
                if (bit25) errorList << "Курс ДВ";
                if (bit24) errorList << "Танг ДВ";
                if (!bit30) {
                    errorList << "DM";
                }

                // Ячейки: (2,1) «Т1А Гот», (2,2) текст
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
                    // Бит28=1 ровно в конце
                    if (errorList.isEmpty())
                    {
                        gotItem->setBackground(QColor(0, 128, 0));
                        errItem->setText(
                            QString("Готов (бит28); Tгот = %1 c").arg(secWaited, 0, 'f', 2)
                            );
                    }
                    else
                    {
                        gotItem->setBackground(QColor(255, 0, 0));
                        errItem->setText(
                            QString("Готов, но ошибки: %1; Tгот = %2 c")
                                .arg(errorList.join(", "))
                                .arg(QString::number(secWaited, 'f', 2))
                            );
                    }
                }
                else
                {
                    // Нет готовности
                    gotItem->setBackground(QColor(255, 0, 0));
                    QString msg = QString("Нет готовности (бит28=0); Tгот=%1 c").arg(QString::number(secWaited, 'f', 2));
                    if (!errorList.isEmpty()) {
                        msg += "; Ошибки: " + errorList.join(", ");
                    }
                    errItem->setText(msg);
                }
                return; // Завершаем лямбду
            }

            // 4) Если 20 с ещё не вышли — смотрим bit28
            unsigned long val2 = IN_KPA[2] & 0x7FFFFFFF;
            unsigned long val0 = IN_KPA[0] & 0x7FFFFFFF;

            bool bit28 = (val2 & (1UL << 28));
            bool bit27 = (val2 & (1UL << 27));
            bool bit26 = (val2 & (1UL << 26));
            bool bit25 = (val2 & (1UL << 25));
            bool bit24 = (val2 & (1UL << 24));
            bool bit30 = (val0 & (1UL << 30));

            QStringList errorList;
            if (bit27) errorList << "ДГ";
            if (bit26) errorList << "KRL";
            if (bit25) errorList << "Курс ДВ";
            if (bit24) errorList << "Танг ДВ";
            if (!bit30) {
                errorList << "DM";
            }

            // (NEW) Во время ожидания:
            // Пишем "Ожидание готовности: X.XX c; Ошибки: ..."
            // (2,1) оставляем белым
            QTableWidgetItem* gotItem = turning_on_the_equipment->item(2, 1);
            if (!gotItem) {
                gotItem = new QTableWidgetItem("Т1А Гот");
                turning_on_the_equipment->setItem(2, 1, gotItem);
            }
            gotItem->setBackground(Qt::white);

            QTableWidgetItem* errItem = turning_on_the_equipment->item(2, 2);
            if (!errItem) {
                errItem = new QTableWidgetItem("Тгот = (Допуск (0-15)с)");
                turning_on_the_equipment->setItem(2, 2, errItem);
            }
            errItem->setBackground(Qt::white);

            // Если bit28=1 уже сейчас — завершаем
            if (bit28)
            {
                readinessTimer->stop();

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
                // Продолжаем ждать, но выводим текущее время и ошибки
                QString waitText = QString("Ожидание готовности: %1 c").arg(QString::number(secWaited, 'f', 2));
                if (!errorList.isEmpty()) {
                    waitText += "; Ошибки: " + errorList.join(", ");
                }
                errItem->setText(waitText);
            }
        });

        // Запускаем
        readinessTimer->start();
    }
    else
    {
        // --- «Подготовка» Off ---
        readinessTimer->stop();

        // Снимаем бит 9
        OUT_AD9M2[0] &= ~(1UL << 9);
        ULONG sum = KS(OUT_AD9M2, 6);
        OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
        BUF256x32_write(0, OUT_AD9M2, 7);
        SO_pusk(0);

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

void handleButtonClick3()
{
    // Предполагаем, что для "третьей проверки" нужна уже включённая «Подготовка»
    if (!clickedPreparation) {
        // Если нужно — оповестить пользователя, выходим
        return;
    }

    g_aPressCount = 0;

    // 1) Сбрасываем старую индикацию (строка=2, колонки=2 и 3)
    {
        QTableWidgetItem* indicationItem = checking_the_operation->item(2, 2);
        if (!indicationItem) {
            indicationItem = new QTableWidgetItem();
            checking_the_operation->setItem(2, 2, indicationItem);
        }
        indicationItem->setText("");
        indicationItem->setBackground(Qt::white);

        QTableWidgetItem* infoItem = checking_the_operation->item(2, 3);
        if (!infoItem) {
            infoItem = new QTableWidgetItem();
            checking_the_operation->setItem(2, 3, infoItem);
        }
        infoItem->setText("");
        infoItem->setBackground(Qt::white);
    }

    // 2) Снимаем бит 9 и бит16
    OUT_AD9M2[0] &= ~(1UL << 9);
    OUT_AD9M2[0] &= ~(1UL << 16);

    ULONG sum = KS(OUT_AD9M2, 6);
    OUT_AD9M2[6] = 0xE5 | ((sum & 0xFFFF) << 8);
    BUF256x32_write(0, OUT_AD9M2, 7);
    SO_pusk(0);

    // 3) Создаём таймер на 3 с (задержка)
    QTimer* delayTimer = new QTimer();
    delayTimer->setSingleShot(true);
    delayTimer->setInterval(3000);

    // По истечении 3 с: снова включаем бит9 и начинаем 20-секундное ожидание
    QObject::connect(delayTimer, &QTimer::timeout, [delayTimer]() {

        // (A) Снова ставим бит9
        OUT_AD9M2[0] |= (1UL << 9);
        ULONG sum2 = KS(OUT_AD9M2, 6);
        OUT_AD9M2[6] = 0xE5 | ((sum2 & 0xFFFF) << 8);
        BUF256x32_write(0, OUT_AD9M2, 7);
        SO_pusk(0);

        // (B) Запоминаем момент "старта" + рассчитываем время "до" 20с
        QTime startTime = QTime::currentTime();
        QTime deadlineTime = startTime.addSecs(20); // +20c

        // (C) Создаём «периодический» таймер (200 мс) для динамического вывода
        QTimer* readinessCheck = new QTimer();
        readinessCheck->setSingleShot(false);
        readinessCheck->setInterval(200);

        // Подключаем логику ожидания
        QObject::connect(readinessCheck, &QTimer::timeout, [readinessCheck, startTime, deadlineTime]() {

            // 1) Считываем IN_KPA
            receiveDataAndDisplay();

            // 2) Считаем, сколько времени прошло
            QTime now = QTime::currentTime();
            int msElapsed = startTime.msecsTo(now);
            double secElapsed = msElapsed / 1000.0;

            // Достаём ячейки индикации (строка=2, кол=2) и информации (строка=2, кол=3)
            QTableWidgetItem* indicationItem = checking_the_operation->item(2, 2);
            if (!indicationItem) {
                indicationItem = new QTableWidgetItem();
                checking_the_operation->setItem(2, 2, indicationItem);
            }
            QTableWidgetItem* infoItem = checking_the_operation->item(2, 3);
            if (!infoItem) {
                infoItem = new QTableWidgetItem();
                checking_the_operation->setItem(2, 3, infoItem);
            }

            // 3) Проверяем, закончились ли 20с
            if (now >= deadlineTime)
            {
                // Останавливаем периодический таймер
                readinessCheck->stop();
                readinessCheck->deleteLater();

                // Финально читаем IN_KPA
                receiveDataAndDisplay();

                // Проверяем биты28..24, 30 и формируем результат
                unsigned long val2 = IN_KPA[2] & 0x7FFFFFFF;
                unsigned long val0 = IN_KPA[0] & 0x7FFFFFFF;

                bool bit28 = (val2 & (1UL << 28));
                bool bit27 = (val2 & (1UL << 27));
                bool bit26 = (val2 & (1UL << 26));
                bool bit25 = (val2 & (1UL << 25));
                bool bit24 = (val2 & (1UL << 24));
                bool bit30 = (val0 & (1UL << 30));

                QStringList errorList;
                if (bit27) errorList << "ДГ";
                if (bit26) errorList << "KRL";
                if (bit25) errorList << "Курс ДВ";
                if (bit24) errorList << "Танг ДВ";
                if (!bit30) {
                    errorList << "DM";
                }

                double finalSec = secElapsed; // 20с уже прошло

                if (bit28)
                {
                    // Успех или успех с ошибками
                    if (errorList.isEmpty())
                    {
                        indicationItem->setText("Выполнено");
                        indicationItem->setBackground(QColor(0, 128, 0)); // зелёный
                        infoItem->setText(
                            QString("T = %1 c (без ошибок)").arg(finalSec, 0, 'f', 2)
                            );
                    }
                    else
                    {
                        indicationItem->setText("Выполнено (с ошибками)");
                        indicationItem->setBackground(QColor(255, 165, 0)); // оранжевый
                        infoItem->setText(
                            QString("T = %1 c; Ошибки: %2")
                                .arg(finalSec, 0, 'f', 2)
                                .arg(errorList.join(", "))
                            );
                    }
                }
                else
                {
                    // bit28=0 => невыполнено
                    indicationItem->setText("Не выполнено");
                    indicationItem->setBackground(QColor(255, 0, 0));
                    if (errorList.isEmpty()) {
                        infoItem->setText(
                            QString("Бит28=0; T= %1 c; Ошибок нет").arg(finalSec, 0, 'f', 2)
                            );
                    } else {
                        infoItem->setText(
                            QString("Бит28=0; T= %1 c; Ошибки: %2")
                                .arg(finalSec, 0, 'f', 2)
                                .arg(errorList.join(", "))
                            );
                    }
                }
                return; // Завершили
            }

            // 4) Время ещё не вышло, проверяем bit28
            unsigned long val2 = IN_KPA[2] & 0x7FFFFFFF;
            unsigned long val0 = IN_KPA[0] & 0x7FFFFFFF;

            bool bit28 = (val2 & (1UL << 28));
            bool bit27 = (val2 & (1UL << 27));
            bool bit26 = (val2 & (1UL << 26));
            bool bit25 = (val2 & (1UL << 25));
            bool bit24 = (val2 & (1UL << 24));
            bool bit30 = (val0 & (1UL << 30));

            QStringList errorList;
            if (bit27) errorList << "ДГ";
            if (bit26) errorList << "KRL";
            if (bit25) errorList << "Курс ДВ";
            if (bit24) errorList << "Танг ДВ";
            if (!bit30) {
                errorList << "DM";
            }

            if (bit28)
            {
                // Бит28 пришёл досрочно -> успех (или с ошибками)
                readinessCheck->stop();
                readinessCheck->deleteLater();

                double finalSec = secElapsed;

                if (errorList.isEmpty())
                {
                    indicationItem->setText("Выполнено");
                    indicationItem->setBackground(QColor(0, 128, 0));
                    infoItem->setText(
                        QString("T = %1 c (без ошибок)").arg(finalSec, 0, 'f', 2)
                        );
                }
                else
                {
                    indicationItem->setText("Выполнено (с ошибками)");
                    indicationItem->setBackground(QColor(255, 165, 0));
                    infoItem->setText(
                        QString("T = %1 c; Ошибки: %2")
                            .arg(finalSec, 0, 'f', 2)
                            .arg(errorList.join(", "))
                        );
                }
            }
            else
            {
                // Бит28 ещё 0 => продолжаем ждать
                indicationItem->setText(""); // пустое
                indicationItem->setBackground(Qt::white);

                QString waitStr = QString("Ожидание T= %1 c").arg(secElapsed, 0, 'f', 2);
                if (!errorList.isEmpty()) {
                    waitStr += "; Ошибки: " + errorList.join(", ");
                }
                infoItem->setText(waitStr);
            }
        });

        // Запуск 20-секундного периода
        readinessCheck->start();

        // Останавливаем и удаляем 3‑с таймер
        delayTimer->stop();
        delayTimer->deleteLater();
    });

    // Запуск 3‑с задержки
    delayTimer->start();
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
