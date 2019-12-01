#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class SerialThread : public QThread
{
    Q_OBJECT

public:
    explicit SerialThread(QObject *parent = nullptr);
    ~SerialThread();
    void transaction(const QString &portName, int waitTimeout, const QByteArray &request);

//    void queueMessage(QByteArray request);

signals:
    /// emitted after receiving response.
    /// Response is unstuffed with frame begin, protocol version and length cut off
    /// and command msb reset to 0
    void response(const uint8_t command, const QByteArray &response);
    /// emitted at connection errors and at parsing response errors
    void error(const QString &s);
    void timeout(const QString &s);

public slots:
private:
    void run() override;

    QString m_portName;
    QByteArray m_request;
    int m_waitTimeout = 10;
    QMutex m_mutex;
    QWaitCondition m_cond;
    bool m_quit = false;

    // queueing
//    QList<QByteArray> queue;
    // parsing
    QByteArray currentResponse;

    QByteArray stuffByte(int8_t byte);
    QByteArray stuffBytes(const QByteArray &from);
    void parseByte(uint8_t byte);
};

#endif // SERIALTHREAD_H
