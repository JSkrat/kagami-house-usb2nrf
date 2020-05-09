#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <stdint.h>

class Message {
public:
    explicit Message(const QByteArray &request, const QString &portName, int waitTimeout);
    QByteArray request;
    QString portName;
    int waitTimeout;
};

class SerialThread : public QThread
{
    Q_OBJECT

public:
    explicit SerialThread(QObject *parent = nullptr);
    ~SerialThread();
    void transaction(const QString &portName, int waitTimeout, const QByteArray &request);
    int getQueueSize();
//    void queueMessage(QByteArray request);

signals:
    /// emitted after receiving response.
    /// Response is unstuffed with frame begin, protocol version and length cut off
    /// and command msb reset to 0
    void response(const uint8_t command, const uint8_t code, const QByteArray &response);
    /// emitted at connection errors and at parsing response errors
    void error(const QString &s);
    void timeout(const QString &s);

public slots:
private:
    void run() override;
    QQueue<Message> msgQueue;

    QString m_portName;
    QByteArray m_request;
    int m_waitTimeout = 10000;
    QMutex m_mutex;
    QWaitCondition m_cond;
    bool m_quit = false;

    // queueing
//    QList<QByteArray> queue;
    // parsing
    unsigned int currentPacketLength;
    uint8_t responseCommand;
    uint8_t responseCode;
    QByteArray currentResponse;

    QByteArray stuffByte(int8_t byte);
    QByteArray stuffBytes(const QByteArray &from);
    void parseByte(uint8_t byte);
};

#endif // SERIALTHREAD_H
