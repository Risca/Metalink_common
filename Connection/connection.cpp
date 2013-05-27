#include "connection.h"
#include <QtNetwork>

static const int TransferTimeout = 30 * 1000;
static const int PongTimeout = 60 * 1000;
static const int PingInterval = 5 * 1000;
static const char SeparatorToken = ' ';

Connection::Connection(QObject *parent) :
        QTcpSocket(parent)
{

    currentState = SendingNick;
    currentDataType = Undefined;

    connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));

    numBytesForCurrentDataType = -1;
    transferTimerId = 0;
    pingTimer.setInterval(PingInterval);
    connect(this, SIGNAL(disconnected()), &pingTimer, SLOT(stop()));
    connect(&pingTimer, SIGNAL(timeout()), this, SLOT(sendPing()));
}

void Connection::timerEvent(QTimerEvent *timerEvent)
{
    if (timerEvent->timerId() == transferTimerId) {
        abort();
        killTimer(transferTimerId);
        transferTimerId = 0;
    }
}

void Connection::processReadyRead()
{
    do {
        if (currentDataType == Undefined) {
            if (!readProtocolHeader())
                return;
        }
        if (!hasEnoughData())
            return;
        processData();
    } while (this->bytesAvailable() > 0);
}

void Connection::sendPing()
{
    if (pongTime.elapsed() > PongTimeout) {
        abort();
        return;
    }

    send(Ping);
}

void Connection::send(DataType type, const QString &message)
{
    QByteArray data(QByteArray::number(message.toUtf8().size()) + " " + message.toUtf8());

    switch(type) {
    case Ping:
        data.prepend("PING ");
        break;
    case Pong:
        data.prepend("PONG ");
        break;
    case Nick:
        data.prepend("NICK ");
        currentState = AwaitingClientList;
        pingTimer.start();
        pongTime.start();
        break;
    case Chat:
        data.prepend("CHAT ");
        break;
    case Audio:
        data.prepend("AUDIO ");
    default:
        break;
    }

    if (this->write(data) == data.size()) {
        if(type!=Ping && type!=Pong) { //Filter out some pings and pongs
            qDebug() << "Sent message: " << data;
        }
    }
}

int Connection::readDataIntoBuffer(int maxSize)
{
    if (maxSize > MaxBufferSize)
        return 0;

    int numBytesBeforeRead = buffer.size();
    if (numBytesBeforeRead == MaxBufferSize) {
        abort();
        return 0;
    }

    while (this->bytesAvailable() > 0 && buffer.size() < maxSize) {
        buffer.append(this->read(1));
        if (buffer.endsWith(SeparatorToken))
            break;
    }
    return buffer.size() - numBytesBeforeRead;
}

int Connection::dataLengthForCurrentDataType()
{
    if (this->bytesAvailable() <= 0 || readDataIntoBuffer() <= 0
            || !buffer.endsWith(SeparatorToken))
        return 0;

    buffer.chop(1);
    int number = buffer.toInt();
    buffer.clear();
    return number;
}

bool Connection::readProtocolHeader()
{
    if (readDataIntoBuffer() <= 0) {
        transferTimerId = startTimer(TransferTimeout);
        return false;
    }

    if (buffer == "PING ") {
        currentDataType = Ping;
    } else if (buffer == "PONG ") {
        currentDataType = Pong;
    } else if (buffer == "NICK ") {
        currentDataType = Nick;
    } else if (buffer == "CHAT ") {
        currentDataType = Chat;
    } else if (buffer == "AUDIO ") {
        currentDataType = Audio;
    } else {
        currentDataType = Undefined;
        abort();
        return false;
    }

    buffer.clear();
    numBytesForCurrentDataType = dataLengthForCurrentDataType();
    return true;
}

bool Connection::hasEnoughData()
{
    if (transferTimerId) {
        QObject::killTimer(transferTimerId);
        transferTimerId = 0;
    }

    if (numBytesForCurrentDataType <= 0)
        numBytesForCurrentDataType = dataLengthForCurrentDataType();

    if (this->bytesAvailable() < numBytesForCurrentDataType
            || numBytesForCurrentDataType <= 0) {
        transferTimerId = startTimer(TransferTimeout);
        return false;
    }

    return true;
}

void Connection::processData()
{
    buffer = this->read(numBytesForCurrentDataType);
    if (buffer.size() != numBytesForCurrentDataType) {
        abort();
        return;
    }

    switch (currentDataType) {
    case Ping:
        send(Pong);
        break;
    case Pong:
        pongTime.restart();
        break;
    case Nick:
        {
            QString nick = QString::fromUtf8(buffer);
            setNick(nick);
            qDebug() << "Received new nick: " << nick;
        }
        break;
    case Chat:
        {
            QString commandString = QString::fromUtf8(buffer);
            _currentChatCommand.retranslateCommand(commandString);
            qDebug() << "New incoming chat command: " << commandString;
            emit newChatCommand();
        }
        break;
    case Audio:
        {
            QString payload = QString::fromUtf8(buffer);
            qDebug() << "Audio available using payload" << payload;
            emit audioAvailableUsingPayload(payload);
        }
    default:
        break;
    }

    currentDataType = Undefined;
    numBytesForCurrentDataType = 0;
    buffer.clear();
}
