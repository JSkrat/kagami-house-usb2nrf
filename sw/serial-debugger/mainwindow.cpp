#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialthread.h"
#include <QObject>
#include <QMessageBox>
#include <QCheckBox>


enum eModemCommand {
    mcStatus = 0,
    mcAddresses = 1,

    mcSetChannel = 0x10,
    mcSetTXPower = 0x11,
    mcSetBitRate = 0x12,
    mcSetAutoRetransmitDelay = 0x13,
    mcSetAutoRetransmitCount = 0x14,

    mcClearTX = 0x20,
    mcClearRX = 0x21,

    mcListen = 0x30,

    mcSetMode = 0x40,
    mcSetListenAddress = 0x41,

    mcTransmit = 0x7F,

    mcAckFromRF = 0x4C,
    mcReceiveFromRF = 0x6E,
};

const QHash<enum eModemCommand, QString> responseEtcText = {
    {mcListen, "listening"},
    {mcSetMode, "requested mode set (if 0)"},
    {mcClearTX, "clear TX ok"},
    {mcSetListenAddress, "master address set"},
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , StatusUpgradeTimer(this)
    , serial(this)
{
    ui->setupUi(this);
    this->ui->teConsole->clear();

    ui->leSerialPort->setText(this->port);

    this->StatusUpgradeTimer.setInterval(100);
    connect(&(this->StatusUpgradeTimer), &QTimer::timeout, this, &MainWindow::statusUpgrade);
    this->StatusUpgradeTimer.start();

    qRegisterMetaType<uint8_t>("uint8_t");
    connect(&(this->serial), &SerialThread::response, this, &MainWindow::serialResponse);
    connect(&(this->serial), &SerialThread::timeout, this, &MainWindow::serialTimeout);
    connect(&(this->serial), &SerialThread::error, this, &MainWindow::serialError);
}

MainWindow::~MainWindow()
{
    this->StatusUpgradeTimer.stop();
    delete ui;
    //delete this->serial;
}

void MainWindow::setCheckBoxValue(QCheckBox *checkbox, bool value)
{
    if (value) checkbox->setCheckState(Qt::CheckState::Checked);
    else checkbox->setCheckState(Qt::CheckState::Unchecked);
}


void MainWindow::addMessage(QByteArray from, QString decorator, QString message, QColor background) {
    this->ui->teConsole->append(
                QString("<p>&nbsp;</p>"
                        "<span style='background-color:#%1%2%3;'><span style='font-weight:600; background-color:#%1%2%3;'>%6 %4 </span></span>"
                        "<span style='background-color:#%1%2%3;'><span style='font-family:\"monospace\"; background-color:#%1%2%3;'>%5</span></span>"
                        "")
                .arg(background.red(), 2, 16, QChar('0'))
                .arg(background.green(), 2, 16, QChar('0'))
                .arg(background.blue(), 2, 16, QChar('0'))
                .arg(QString(from.toHex(':')).toUpper())
                .arg(message)
                .arg(decorator)
                );
}

void MainWindow::addMessage(QByteArray from, QString decorator, QByteArray message, QColor background) {
    this->addMessage(from, decorator, QString(message.toHex(':').toUpper()), background);
}

void MainWindow::addSendMessage(QByteArray from, QByteArray message) {
    this->addMessage(from, "Send to", message, QColor::fromRgbF(1, 0.9, 0.9));
}

void MainWindow::addACKMessage(QByteArray from, QByteArray message) {
    this->addMessage(from, "ACK from", message, QColor::fromRgbF(1, 1, 0.9));
}

void MainWindow::addReceiveMessage(QByteArray from, QByteArray message) {
    this->addMessage(from, "Receive from", message, QColor::fromRgbF(0.9, 1, 0.9));
}

void MainWindow::addEtcMessage(QByteArray from, QString message) {
    this->addMessage(from, "system message", QString("<i>%1</i>").arg(message), QColor::fromRgbF(0.9, 0.9, 0.9));
}

void MainWindow::statusUpgrade()
{
    /// request modem status upgrade
    this->serial.transaction(this->port, 10, QByteArray(1, '\x00'));
}

void MainWindow::serialResponse(const uint8_t command, const QByteArray &response)
{
    enum eModemCommand eCommand = static_cast<enum eModemCommand>(command);
    switch (eCommand) {
    case mcStatus: {
        // modem status
//        this->ui->statusbar->showMessage(response.toHex(':'));
        // config
        uint8_t modemConfig = static_cast<uint8_t>(response[0]);
        this->setCheckBoxValue(this->ui->cbMASK_RX_DR, 0x40 & modemConfig);
        this->setCheckBoxValue(this->ui->cbMASK_TX_DS, 0x20 & modemConfig);
        this->setCheckBoxValue(this->ui->cbMASK_MAX_RT, 0x10 & modemConfig);
        this->setCheckBoxValue(this->ui->cbEN_CRC, 0x08 & modemConfig);
        this->setCheckBoxValue(this->ui->cbCRCO, 0x04 & modemConfig);
        this->setCheckBoxValue(this->ui->cbPWR_UP, 0x02 & modemConfig);
        this->setCheckBoxValue(this->ui->cbPRIM_RX, 0x01 & modemConfig);
        // EN_AA
        uint8_t modemEN_AA = static_cast<uint8_t>(response[1]);
        this->setCheckBoxValue(this->ui->cbEN_AA_P0, 0x01 & modemEN_AA);
        this->setCheckBoxValue(this->ui->cbEN_AA_P1, 0x02 & modemEN_AA);
        this->setCheckBoxValue(this->ui->cbEN_AA_P2, 0x04 & modemEN_AA);
        this->setCheckBoxValue(this->ui->cbEN_AA_P3, 0x08 & modemEN_AA);
        this->setCheckBoxValue(this->ui->cbEN_AA_P4, 0x10 & modemEN_AA);
        this->setCheckBoxValue(this->ui->cbEN_AA_P5, 0x20 & modemEN_AA);
        // EN_RXADDR
        uint8_t modemEN_RXADDR = static_cast<uint8_t>(response[2]);
        this->setCheckBoxValue(this->ui->cbERX_P0, 0x01 & modemEN_RXADDR);
        this->setCheckBoxValue(this->ui->cbERX_P1, 0x02 & modemEN_RXADDR);
        this->setCheckBoxValue(this->ui->cbERX_P2, 0x04 & modemEN_RXADDR);
        this->setCheckBoxValue(this->ui->cbERX_P3, 0x08 & modemEN_RXADDR);
        this->setCheckBoxValue(this->ui->cbERX_P4, 0x10 & modemEN_RXADDR);
        this->setCheckBoxValue(this->ui->cbERX_P5, 0x20 & modemEN_RXADDR);
        // SETUP_AW
        uint8_t modemAW = static_cast<uint8_t>(response[3]);
        if (0 == (modemAW & 0x03)) {
            this->ui->lSETUP_AW->setText("illegal");
        } else {
            this->ui->lSETUP_AW->setText(QString("%1 bytes").arg((modemAW & 0x03) + 2));
        }
        // SETUP_RETR
        uint8_t modemRETR = static_cast<uint8_t>(response[4]);
        this->ui->lARD->setText(QString("%1μS").arg(((modemRETR >> 4) + 1) * 250));
        if (0 == (modemRETR & 0x0F)) {
            this->ui->lARC->setText("disabled");
        } else {
            this->ui->lARC->setText(QString("up to %1").arg(modemRETR & 0x0F));
        }
        // RF_CH
        uint8_t modemRF_CH = static_cast<uint8_t>(response[5]);
        this->ui->lRF_CH->setText(QString("%1").arg(modemRF_CH & 0x7F, 2, 16));
        // RF_SETUP
        uint8_t modemRF_SETUP = static_cast<uint8_t>(response[6]);
        this->setCheckBoxValue(this->ui->cbCONT_WAVE, 0x80 & modemRF_SETUP);
        this->setCheckBoxValue(this->ui->cbPLL_LOCK, 0x10 & modemRF_SETUP);
        QStringList RF_DR = {"1Mbps", "2Mbps", "250kbps", "reserved"};
        this->ui->lRF_DR->setText(RF_DR[2 * (modemRF_SETUP >> 5 & 1) + (modemRF_SETUP >> 3 & 1)]);
        QStringList RF_PWR = {"-18dBm", "-12dBm", "-6dBm", "0dBm"};
        this->ui->lRF_PWR->setText(RF_PWR[modemRF_SETUP & 0x03]);
        // status
        uint8_t modemStatus = static_cast<uint8_t>(response[7]);
        this->setCheckBoxValue(this->ui->cbRX_DR, 0x40 & modemStatus);
        this->setCheckBoxValue(this->ui->cbTX_DS, 0x20 & modemStatus);
        this->setCheckBoxValue(this->ui->cbMAX_RT, 0x10 & modemStatus);
        this->setCheckBoxValue(this->ui->cbTX_FULL, 0x01 & modemStatus);
        uint8_t RX_P_NO = (modemStatus & 0x0E) >> 1;
        if (7 == RX_P_NO) this->ui->lRX_P_NO->setText("empty");
        else this->ui->lRX_P_NO->setText(QString("%1").arg(RX_P_NO));
        // OBSERVE_TX
        uint8_t modemOBSERVE_TX = static_cast<uint8_t>(response[8]);
        this->ui->lPLOS_CNT->setNum(modemOBSERVE_TX >> 4);
        this->ui->lARC_CNT->setNum(modemOBSERVE_TX & 0x0F);
        // RPD
        this->setCheckBoxValue(this->ui->cbRPD, 0x01 & response[9]);
        // RX_PW
        QLabel *RX_PW[6] = {
            this->ui->lRX_PW_P0,
            this->ui->lRX_PW_P1,
            this->ui->lRX_PW_P2,
            this->ui->lRX_PW_P3,
            this->ui->lRX_PW_P4,
            this->ui->lRX_PW_P5,
        };
        for (int i = 0; i < 6; i++) {
            RX_PW[i]->setText(QString("RX_PW_P%1 %2").arg(i).arg(0x3F & response[0x0A + i]));
        }
        // FIFO_STATUS
        QCheckBox *FIFO_STATUS[8] = {
            this->ui->cbRX_EMPTY,
            this->ui->cbRX_FULL,
            nullptr,
            nullptr,
            this->ui->cbTX_EMPTY,
            this->ui->cbTX_FULL,
            this->ui->cbTX_REUSE,
            nullptr,
        };
        for (int i = 0; i < 8; i++) {
            if (nullptr != FIFO_STATUS[i]) {
                this->setCheckBoxValue(FIFO_STATUS[i], response[0x10] & (1 << i));
            }
        }
        // DYNPD
        QCheckBox *DYNPD[6] = {
            this->ui->cbDPL_P0,
            this->ui->cbDPL_P1,
            this->ui->cbDPL_P2,
            this->ui->cbDPL_P3,
            this->ui->cbDPL_P4,
            this->ui->cbDPL_P5,
        };
        for (int i = 0; i < 6; i++) {
            this->setCheckBoxValue(DYNPD[i], response[0x11] & (1 << i));
        }
        // FEATURE
        QCheckBox *FEATURE[3] = {
            this->ui->cbEN_DYN_ACK,
            this->ui->cbEN_ACK_PAY,
            this->ui->cbEN_DPL,
        };
        for (int i = 0; i < 3; i++) {
            this->setCheckBoxValue(FEATURE[i], response[0x12] & (1 << i));
        }
        ////////////////////////////////////////////////////////////////
        /// trigger second request with addresses //////////////////////
        ////////////////////////////////////////////////////////////////
        this->serial.transaction(this->port, 10, QByteArray(1, '\x01'));
        break;
    }
    case mcAddresses: {
        this->ui->lRX_ADDR_P0->setText(response.mid(0x00, 5).toHex(':'));
        this->ui->lRX_ADDR_P1->setText(response.mid(0x05, 5).toHex(':'));
        this->ui->lTX_ADDR->setText(response.mid(0x0E, 5).toHex(':'));
        QLabel *RX_ADDR_P1[4] = {
            this->ui->lRX_ADDR_P2,
            this->ui->lRX_ADDR_P3,
            this->ui->lRX_ADDR_P4,
            this->ui->lRX_ADDR_P5,
        };
        for (int i = 0; i < 4; i++) {
            RX_ADDR_P1[i]->setText(
                        QString("(%1):%2")
                        .arg(QString(response.mid(0x05, 4).toHex(':')))
                        .arg(static_cast<uint8_t>(response[0x0A + i]), 2, 16)
                        );
        }
        break;
    }
    case mcAckFromRF: {
        this->addACKMessage(response.mid(0, 5), response.mid(5));
        break;
    }
    case mcReceiveFromRF: {
        this->addReceiveMessage(response.mid(0, 5), response.mid(5));
        break;
    }
    case mcTransmit: {
        this->addEtcMessage(
                    response.mid(0, 5),
                    QString("transmission accepted</i> %1<i>").arg(QString(response.mid(5).toHex(':')))
                    );
        break;
    }
    default: {
        if (responseEtcText.keys().contains(eCommand)) {
            this->addEtcMessage(
                        response,
                        responseEtcText[eCommand]
                        );
        } else {
            this->ui->statusbar->showMessage(QString("Unknown response %1").arg(static_cast<uint8_t>(response[0]), 2, 16), 5000);
            this->addEtcMessage(
                        QByteArray(),
                        QString("unknown response</i> %1 %2<i>")
                            .arg(command, 2, 16, QChar('0'))
                            .arg(QString(response.toHex(':')))
                        );
        }
        break;
    }
    }
}

void MainWindow::serialTimeout(const QString &message)
{
    this->ui->statusbar->showMessage("Timeout: " + message, 5000);
    this->addEtcMessage(
                QByteArray(),
                QString("serial timeout %1").arg(message)
                );}

void MainWindow::serialError(const QString &message)
{
    this->ui->statusbar->showMessage("Error: " + message, 5000);
    this->addEtcMessage(
                QByteArray(),
                QString("serial error %1").arg(message)
                );}


void MainWindow::on_pushButton_clicked()
{
    this->port = this->ui->leSerialPort->text();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1) {
        this->StatusUpgradeTimer.start();
    } else {
        this->StatusUpgradeTimer.stop();
    }
}

/**
 * @brief MainWindow::on_pushButton_2_clicked
 * send "listen" command
 */
void MainWindow::on_pushButton_2_clicked()
{
    QByteArray pkt;
    pkt.append(mcListen);
    for (QString b: this->ui->leAddressListen->text().split(":")) {
        pkt.append(static_cast<char>(b.toInt(nullptr, 16)));
    }
    this->serial.transaction(this->port, 10, pkt);
    this->addEtcMessage(pkt.mid(1), "listen");
}

/**
 * @brief MainWindow::on_pushButton_3_clicked
 * send "transmit" command
 */
void MainWindow::on_pushButton_3_clicked()
{
    QByteArray pkt;
    pkt.append(mcTransmit);
    for (QString b: this->ui->leAddressTransmitTo->text().split(":")) {
        pkt.append(static_cast<char>(b.toInt(nullptr, 16)));
    }
    for (QString b: this->ui->lePayloadToTransmit->text().split(":")) {
        pkt.append(static_cast<char>(b.toInt(nullptr, 16)));
    }
    this->serial.transaction(this->port, 10, pkt);
    this->addSendMessage(pkt.mid(1, 5), pkt.mid(6));

}

void MainWindow::on_pushButton_4_clicked()
{
    QByteArray pkt;
    pkt.append(mcClearTX);
    this->serial.transaction(this->port, 10, pkt);
    this->addEtcMessage(pkt, "clear tx queue");
}

void MainWindow::on_pushButton_5_clicked()
{
    QByteArray pkt;
    pkt.append(mcSetMode);
    pkt.append(2);
    this->serial.transaction(this->port, 10, pkt);
    this->addEtcMessage(pkt, "Set mode Master");
}

void MainWindow::on_pushButton_6_clicked()
{
    QByteArray pkt;
    pkt.append(mcSetMode);
    pkt.append(1);
    this->serial.transaction(this->port, 10, pkt);
    this->addEtcMessage(pkt, "Set mode Slave");
}

void MainWindow::on_pushButton_7_clicked()
{
    QByteArray pkt;
    pkt.append(mcSetMode);
    pkt.append(static_cast<char>(0));
    this->serial.transaction(this->port, 10, pkt);
    this->addEtcMessage(pkt, "Set mode Debug");
}

void MainWindow::on_pushButton_8_clicked()
{
    QByteArray pkt;
    pkt.append(mcSetListenAddress);
    for (QString b: this->ui->leAddressListen->text().split(":")) {
        pkt.append(static_cast<char>(b.toInt(nullptr, 16)));
    }
    this->serial.transaction(this->port, 10, pkt);
    this->addEtcMessage(pkt.mid(1), "set master address");
}
