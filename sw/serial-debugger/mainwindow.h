#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "serialthread.h"
#include <QCheckBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addSendMessage(QByteArray from, QByteArray message);
    void addMessage(QByteArray from, QString decorator, QString message, QColor background);
    void addMessage(QByteArray from, QString decorator, QByteArray message, QColor background);
    void addReceiveMessage(QByteArray from, QByteArray message);
    void addACKMessage(QByteArray from, QByteArray message);
    void addEtcMessage(QByteArray from, QString message);
public slots:
    void statusUpgrade();

    void serialResponse(const uint8_t command, const QByteArray &response);
    void serialTimeout(const QString &message);
    void serialError(const QString &message);

private slots:
    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    QTimer StatusUpgradeTimer;
    SerialThread serial;
    QString port = "COM8";
    void setCheckBoxValue(QCheckBox *checkbox, bool value);
};
#endif // MAINWINDOW_H
