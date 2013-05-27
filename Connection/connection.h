#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QTime>
#include <QTimer>
#include <QtGui>
#include "chatcommand.h"
#include "Connection_global.h"

static const int MaxBufferSize = 1024000;

class CONNECTIONSHARED_EXPORT Connection : public QTcpSocket {
Q_OBJECT

public:
    enum ConnectionState {
        SendingNick,
        AwaitingClientList,
        ReadyForUse
    };
    enum DataType {
        Ping,
        Pong,
        Nick,
        Chat,
        Audio,
        Undefined
    };

    explicit Connection(QObject *parent = 0);

    QString &nick()
    { return _nick; }
    const ChatCommand currentCommand()  { return _currentChatCommand; }

public slots:
    void send(DataType type, const QString &message = "p");
    virtual void setNick(QString nick)
    { _nick = nick; }

signals:
    void newChatCommand();
    void audioAvailableUsingPayload(QString);

protected:
     void timerEvent(QTimerEvent *timerEvent);
     QString _nick;

private slots:
    void processReadyRead();
    void sendPing();

private:
    int readDataIntoBuffer(int maxSize = MaxBufferSize);
    int dataLengthForCurrentDataType();
    bool readProtocolHeader();
    bool hasEnoughData();
    void processData();

    quint16 blockSize;
    QByteArray buffer;
    int numBytesForCurrentDataType;
    ConnectionState currentState;
    DataType currentDataType;
    ChatCommand _currentChatCommand;
    QTimer pingTimer;
    QTime pongTime;
    int transferTimerId;
};

#endif // CONNECTION_H
