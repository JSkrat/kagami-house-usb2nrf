#include "packet.h"
#include <QMap>
#include <QHash>
#include <QString>

const QMap<enum eRFFunction, QString> functionName = {
    {rfNumberUnits, "info"},
    {rfGetDescription, "get description"},
    {rfSetDescription, "set description"},

    {rfSetKey, "set key"},
    {rfSetAddress, "set address"},
    {rfGetStatistics, "get statistics"},
    {rfResetTransactionId, "reset transaction id"},
    {rfNOP, "nop"},
    {rfSetChannel, "set channel"},
    {rfSetMode, "set mode"},
    {rfGetBuildVersion, "get build version"},
};

const QHash<uint8_t, QString> responseCode = {
    {0x00, "ok"},
    {0x90, "bad protocol version"},
    {0xA0, "bad unit number"},
    {0xB0, "bad transaction id"},
    {0xC0, "bad function number"},
    {0xD0, "response is too big"},
    {0xE0, "bad request data"},
    {0x7F, "function is not implemented"},
};

const QHash<uint8_t, QString> dataType = {
    {0x0, "none"},
    {0x1, "bool"},
    {0x2, "byte"},
    {0x3, "int32"},
    {0x4, "string"},
    {0x5, "byte array"},
    {0xF, "unspecified"},
};

Packet::Packet()
{

}

QByteArray Packet::parseHex(QString text)
{
    QByteArray ret;
    if ("" == text) return ret;
    for (QString b: text.split(":")) {
        ret.append(static_cast<char>(b.toInt(nullptr, 16)));
    }
    return ret;
}

QByteArray Packet::serializeRequest(uint8_t version, uint8_t transactionID, uint8_t unit, uint8_t function, QByteArray data)
{
    if (28 < data.length()) throw std::runtime_error("data can not be longer than 28 bytes");
    QByteArray ret;
    ret.append(version);
    ret.append(transactionID);
    ret.append(unit);
    ret.append(function);
    ret.append(data);
    return ret;
}

QString Packet::parseBaseRequest(QByteArray packet)
{
    if (4 > packet.length()) throw std::runtime_error("request packet can not be shorter than 4 bytes");
    if (32 < packet.length()) throw std::runtime_error("request packet can not be longer than 32 bytes");
    QString function = "unknown";
    if (functionName.contains(static_cast<eRFFunction>(packet.at(3)))) {
        function = functionName[static_cast<eRFFunction>(packet.at(3))];
    }
    return QString("V%1 T%2, unit %3, func %5 (0x%4)")
            .arg(static_cast<uint8_t>(packet[0]), 2, 16, QChar('0'))
            .arg(static_cast<uint8_t>(packet[1]), 2, 16, QChar('0'))
            .arg(static_cast<uint8_t>(packet[2]), 2, 16, QChar('0'))
            .arg(static_cast<uint8_t>(packet[3]), 2, 16, QChar('0'))
            .arg(function)
            ;
}

QString Packet::ParseRequest(QByteArray packet)
{
    QString base = Packet::parseBaseRequest(packet);
    QString data;
    switch (packet[3]) {
    case rfSetDescription: {
        data = QString::fromUtf8(packet.mid(4));
        break;
    }
    case rfSetKey: {
        data = QString("type 0x%1 key %2")
                .arg(static_cast<uint8_t>(packet[4]), 2, 16, QChar('0'))
                .arg(QString(packet.mid(5).toHex(':')))
                ;
        break;
    }
    case rfSetAddress: {
        if (4 + 5 != packet.length()) throw std::runtime_error("incorrect payload length");
        data = "addr " + packet.mid(4).toHex(':');
        break;
    }
    case rfSetChannel: {
        if (4 + 1 != packet.length()) throw std::runtime_error("incorrect payload length");
        data = QString("channel 0x%1").arg(static_cast<uint8_t>(packet[4]));
        break;
    }
    case rfSetMode: {
        if (4 + 1 != packet.length()) throw std::runtime_error("incorrect payload length");
        switch (packet[4]) {
        case 1: {
            data = "adv mode";
            break;
        }
        case 2: {
            data = "normal mode";
            break;
        }
        default: {
            data = "incorrect mode";
            break;
        }
        }
        break;
    }
    default: break;
    }
    if ("" != data) data = ", " + data;
    return base + data;
}

QString Packet::parseBaseResponse(QByteArray packet)
{
    if (3 > packet.length()) throw std::runtime_error("response packet can not be shorter than 3 bytes");
    if (32 < packet.length()) throw std::runtime_error("response packet can not be longer than 32 bytes");
    QString code = "";
    if (responseCode.contains(packet[2])) {
        code = " " + responseCode[packet[2]];
    }
    return QString("V%1 T%2, code %4 (0x%3)")
            .arg(static_cast<uint8_t>(packet[0]), 2, 16, QChar('0'))
            .arg(static_cast<uint8_t>(packet[1]), 2, 16, QChar('0'))
            .arg(static_cast<uint8_t>(packet[2]), 2, 16, QChar('0'))
            .arg(code)
            ;
}

QString Packet::ParseResponse(uint8_t unit, eRFFunction function, QByteArray packet)
{
    QString base = Packet::parseBaseResponse(packet);
    if (0x80 <= static_cast<uint8_t>(packet[2])) return base;
    QString data = "";
    switch (function) {
    case rfNumberUnits: {
        if (3 + 2 > packet.length()) throw std::runtime_error("too short payload");
        if (0 == unit) {
            data = QString("units %1, serial %2")
                    .arg(static_cast<uint8_t>(packet[3]))
                    .arg(QString(packet.mid(4).toHex(':')))
                    ;
        } else {
            if ((packet.length()-3) % 2 != 0) throw std::runtime_error("incorrect payload length");
            for (int i = 3; i < packet.length(); i += 2) {
                int input = packet[i+1] >> 4;
                int output = packet[i+1] & 0x0F;
                QString inp = "?"; QString outp = "?";
                if (dataType.contains(input)) inp = dataType[input];
                if (dataType.contains(output)) outp = dataType[output];
                if (3 < i) data += "\n";
                data += QString("func %1(%2): %3")
                        .arg(static_cast<uint8_t>(packet[i]), 2, 16, QChar('0'))
                        .arg(inp)
                        .arg(outp)
                        ;
            }
        }
        break;
    }
    case rfGetDescription: {
        data = QString::fromUtf8(packet.mid(3));
        break;
    }
    case rfGetStatistics: {
        if (3+12 != packet.length()) throw std::runtime_error("incorrect payload length");
        data = QString("Requests %1 \nResponses %2 \nError %3 \nTransaction errors %4 \nAck t/o %5 \nValidation errors %6")
                .arg(packet[3] + packet[4]*256)
                .arg(packet[5] + packet[6]*256)
                .arg(packet[7] + packet[8]*256)
                .arg(packet[9] + packet[10]*256)
                .arg(packet[11] + packet[12]*256)
                .arg(packet[13] + packet[14]*256)
                ;
        break;
    }
    default: break;
    }
    return base + "\n" + data;
}
