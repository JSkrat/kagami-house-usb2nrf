#include "serialthread.h"
#include <QSerialPort>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTime>
#include <algorithm>

SerialThread::SerialThread(QObject *parent) : QThread(parent)
{

}

SerialThread::~SerialThread() {
    this->m_mutex.lock();
    this->m_quit = true;
    this->m_cond.wakeOne();
    this->m_mutex.unlock();
    wait();
}

void SerialThread::transaction(const QString &portName, int waitTimeout, const QByteArray &request)
{
    const QMutexLocker locker(&m_mutex);
    this->m_portName = portName;
    this->m_waitTimeout = waitTimeout;
    this->m_request = request;
    if (!isRunning())
        start();
    else
        this->m_cond.wakeOne();
}

void SerialThread::run()
{
    bool currentPortNameChanged = false;

    this->m_mutex.lock();
    QString currentPortName;
    if (currentPortName != this->m_portName) {
        currentPortName = this->m_portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = this->m_waitTimeout;
    QByteArray currentRequest = this->m_request;
    this->m_mutex.unlock();
    QSerialPort serial;

    if (currentPortName.isEmpty()) {
        emit this->error(tr("No port name specified"));
        return;
    }

    while (!this->m_quit) {
        if (currentPortNameChanged) {
            serial.close();
            serial.setPortName(currentPortName);
            serial.setBaudRate(200000);

            if (!serial.open(QIODevice::ReadWrite)) {
                emit this->error(tr("Can't open %1, error code %2")
                           .arg(this->m_portName).arg(serial.error()));
                return;
            }
        }
        // write request
        serial.write(this->stuffBytes(currentRequest));
        if (serial.waitForBytesWritten(currentWaitTimeout)) {
            // read response
            if (serial.waitForReadyRead(currentWaitTimeout)) {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(10))
                    responseData += serial.readAll();
                // unstuff response and cut off frame begin, protocol version and length
                if (
                        4 > responseData.length()
                        || 0xC0 != static_cast<uint8_t>(responseData[0])
                        || 0x00 != responseData[1]
                        || 0 == (0x80 & responseData[2])
                        || responseData.length() != responseData[3] + 4
                        ) {
                    emit this->error(QString("Bad response from device (%1)").arg(QString(responseData.toHex(':'))));
                } else {
                    responseData[2] = responseData[2] & 0x7F;
                    int bytesCut = 2;
                    bool esc = false; bool error = false;
                    for (int i = bytesCut; i < responseData.length(); i++) {
                        switch (static_cast<uint8_t>(responseData[i])) {
                        case 0xDB: {
                            if (esc) {
                                error = true;
                                break;
                            }
                            esc = true;
                            bytesCut++;
                            break;
                        }
                        case 0xDC: {
                            if (esc) {
                                responseData[i] = static_cast<char>(0xC0);
                                esc = false;
                            }
                            break;
                        }
                        case 0xDD: {
                            if (esc) {
                                responseData[i] = static_cast<char>(0xDB);
                                esc = false;
                            }
                            break;
                        }
                        }
                        if (error) break;
                        if (! esc) responseData[i-bytesCut] = responseData[i];
                        // length can not be so big that we need to stuff it, so length always be at position 3 and 1 byte length
                        if (3 == i) bytesCut++;
                    }
                    if (error) emit this->error(tr("Response parsing error"));
                    else {
                        responseData.resize(responseData.length() - bytesCut);
                        emit this->response(static_cast<uint8_t>(responseData[0]), responseData.mid(1));
                    }
                }
            } else {
                emit this->timeout(tr("Wait read response timeout %1")
                             .arg(QTime::currentTime().toString()));
            }
        } else {
            emit this->timeout(tr("Wait write request timeout %1")
                         .arg(QTime::currentTime().toString()));
        }
        this->m_mutex.lock();
        m_cond.wait(&this->m_mutex);
        if (currentPortName != this->m_portName) {
            currentPortName = this->m_portName;
            currentPortNameChanged = true;
        } else {
            currentPortNameChanged = false;
        }
        currentWaitTimeout = this->m_waitTimeout;
        currentRequest = this->m_request;
        this->m_mutex.unlock();
    }
}

QByteArray SerialThread::stuffByte(int8_t byte)
{
    QByteArray ret;
    if ('\xC0' == byte) {
        ret.append('\xDB');
        ret.append('\xDC');
    } else if ('\xDB' == byte) {
        ret.append('\xDB');
        ret.append('\xDD');
    } else {
        ret.append(byte);
    }
    return ret;
}

QByteArray SerialThread::stuffBytes(const QByteArray &from)
{
    if (255 <= from.length()) {
        throw std::runtime_error("serialthread::stuffBytes attempt to send too big package (more than 255 bytes long)");
    } else if (0 == from.length()) {
        throw std::runtime_error("serialthread::stuffBytes no command specified, it should be 0th byte of the packet");
    }
    QByteArray tmp;
    tmp.append('\xC0');
    tmp.append('\x00');
    tmp.append(this->stuffByte(from[0]));
    tmp.append(this->stuffByte(from.length() - 1));
    for (uint8_t i = 1; i < from.length(); i++) {
        tmp.append(this->stuffByte(from[i]));
    }
    return tmp;
}

