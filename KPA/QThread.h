#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class SerialPortReader : public QThread
{
    Q_OBJECT

public:
    SerialPortReader(QObject* parent = nullptr) : QThread(parent) {
        serialPort = new QSerialPort(this);
    }

    ~SerialPortReader() {
        stopReading();
        if (serialPort->isOpen()) {
            serialPort->close();
        }
    }

    void run() override {
        // Подключение к COM-порту
        serialPort->setPortName("COM1");
        serialPort->setBaudRate(QSerialPort::Baud115200);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);

        if (!serialPort->open(QIODevice::ReadOnly)) {
            qDebug() << "Не удалось открыть COM-порт!";
            return;
        }

        while (isRunning) {
            if (serialPort->waitForReadyRead(100)) {
                QByteArray data = serialPort->readAll();
                emit dataReceived(QString::fromUtf8(data));
            }
        }
    }

    void stopReading() {
        isRunning = false;
        if (serialPort->isOpen()) {
            serialPort->close();
        }
    }

signals:
    void dataReceived(const QString& data);

private:
    QSerialPort* serialPort;
    bool isRunning = true;
};
