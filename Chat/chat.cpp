#include "chat.h"

#include <QCloseEvent>

Chat::Chat(QWidget *parent, unsigned int ID) :
    QMainWindow(parent),
    _ID(ID)
{
    _currentCommand.retranslateCommand(ChatCommand::Undefined, _ID);
}

Chat::Chat(const QStringList &participants, unsigned int ID, QWidget *parent) :
        QMainWindow(parent),
        _ID(ID)
{
    _participants.setStringList(participants);
    _currentCommand.retranslateCommand(ChatCommand::Undefined, _ID);
}

Chat::~Chat()
{
}

void Chat::appendMessage(QString from, QString message)
{
    QStringList messages = _messages.stringList();
    QString foo;
    foo.append("(" + QTime::currentTime().toString("hh:mm") + ") ");
    foo.append(from + " says:\n   ");
    foo.append(message);
    messages.append(foo);
    _messages.setStringList(messages);
    emit dataChanged();
}

void Chat::addParticipant(QString &nick)
{
    QStringList nicks = _participants.stringList();
    if (!nicks.contains(nick))
        nicks.append(nick);
    setParticipants(nicks);
}

void Chat::removeParticipant(const QString &nick)
{
    QStringList nicks = _participants.stringList();
    nicks.removeOne(nick);
    setParticipants(nicks);
    if (nicks.isEmpty()) {
        emit chatEmpty();
    }
}

void Chat::removeParticipant(const QStringList &nicks)
{
    foreach (QString nick, nicks) {
        removeParticipant(nick);
    }
}

void Chat::setParticipants(const QStringList &participants)
{
    _participants.setStringList(participants);
}

bool Chat::operator ==(const unsigned int &chatID) const
{
    if (_ID == chatID)
        return true;

    return false;
}

void Chat::closeEvent(QCloseEvent *e)
{
    _currentCommand.retranslateCommand(ChatCommand::Leave,_ID);
    emit newCommand();
    QMainWindow::closeEvent(e);
}
