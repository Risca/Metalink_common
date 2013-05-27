#include "chatcommand.h"
#include <QTextStream>

ChatCommand::ChatCommand() :
        _chatID(0),
        _type(Undefined)
{
}

ChatCommand::ChatCommand(QString & commandString) :
        _commandString(commandString)
{
    _chatID = 0;
    _type = Undefined;
    parseCommandString();
}

ChatCommand::ChatCommand(int chatID, Type type, QStringList participants) :
        _chatID(chatID),
        _type(type),
        _message("")
{
    _participants = participants;
    makeCommandString();
}

void ChatCommand::retranslateCommand(Type type, const QStringList &participants, const QString &message, int newChatID)
{
    if(newChatID >= 0)
        _chatID = newChatID;

    _type = type;
    if (!participants.isEmpty()) {
        qDebug() << "Updating ChatCommand participants:" << participants;
        _participants = participants;
    }

    if (!message.isEmpty()) {
        qDebug() << "Updating ChatCommand message:" << message;
        _message = message;
    }

    makeCommandString();
}

void ChatCommand::retranslateCommand(QString & commandString)
{
    _commandString = commandString;
    parseCommandString();
}

QString ChatCommand::makeMetaLinkList(const QString &nick)
{
    QStringList foo(nick);
    return makeMetaLinkList(foo);
}

QString ChatCommand::makeMetaLinkList(const QStringList &nicks)
{
    if(!nicks.isEmpty()) {
        // Number of nicks
        QString message = QString::number(nicks.size());
        // Each nick is prepended with it's size
        for(int i = 0; i < nicks.size(); i++)
        {
            message += " " + QString::number(nicks.at(i).size()) + " " + nicks.at(i);
        }

        return message;
    } else {
        qDebug() << "Sending empty list??";
        return QString();
    }
}

QStringList ChatCommand::parseMetaLinkList(QString &list)
{
    QStringList receivedItems;
    QTextStream message(&list);
    int numberOfItems;
    message >> numberOfItems;
    int lengthOfItem;
    while (numberOfItems) {
        message >> lengthOfItem;
        message.read(1);
        receivedItems.append(message.read(lengthOfItem));
        message.read(1);
        numberOfItems--;
    }
    //Rest is not part of MetaLinkList
    list = message.readAll();
    return receivedItems;
}

void ChatCommand::parseCommandString()
{
    if (!_commandString.isEmpty())
    {
        QTextStream commandStream(&_commandString);

        commandStream >> _chatID;
        commandStream.read(1);

        QString operation;
        commandStream >> operation;
        if (operation == "INIT") {
            _type = Init;
        } else if (operation == "INVITE") {
            _type = Invite;
        } else if (operation == "LIST") {
            _type = List;
        } else if (operation == "ACCEPT") {
            _type = Accept;
        } else if (operation == "REJECT") {
            _type = Reject;
        } else if (operation == "LEAVE") {
            _type = Leave;
        } else if (operation == "MESSAGE") {
            _type = Message;
        } else {
            _type = Undefined;
        }

        QString temp = commandStream.readAll();
        switch (_type) {
        case Init:
        case Invite:
        case List:
            _participants = parseMetaLinkList(temp);
            break;
        case Message:
            // parseMetaLinkList() strips temp of participants
            _participants = parseMetaLinkList(temp);
            // left is only the message
            _message = temp;
            break;
        case Leave:
            _message = temp;
            break;
        default:
            _participants.clear();
        }
    }
}

void ChatCommand::makeCommandString()
{
    _commandString.clear();
    _commandString.append(QString::number(_chatID));
    switch (_type) {
    case Init:
        _commandString.append(" INIT ");
        _commandString.append(makeMetaLinkList(_participants));
        break;
    case Invite:
        _commandString.append(" INVITE ");
        _commandString.append(makeMetaLinkList(_participants));
        break;
    case List:
        _commandString.append(" LIST ");
        _commandString.append(makeMetaLinkList(_participants));
        break;
    case Accept:
        _commandString.append(" ACCEPT ");
        break;
    case Reject:
        _commandString.append(" REJECT ");
        break;
    case Leave:
        _commandString.append(" LEAVE ");
        _commandString.append(_message);
        break;
    case Message:
        _commandString.append(" MESSAGE ");
        _commandString.append(makeMetaLinkList(_participants) + ' ');
        _commandString.append(_message);
        break;
    case Undefined:
    default:
        break;
    }
}
