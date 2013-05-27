#ifndef CHAT_H
#define CHAT_H

#include <QStringListModel>
#include <QtGui/QMainWindow>
#include <QTime>
#include "chatcommand.h"
#include "Chat_global.h"

class CHAT_EXPORT Chat : public QMainWindow
{
    Q_OBJECT

public:
    Chat(QWidget *parent = 0, unsigned int ID = 0);
    Chat(const QStringList &participants, unsigned int ID = 0, QWidget *parent = 0);
    ~Chat();

    int id()                    { return _ID; }
    void setID(int id)          { _ID = id; }
    ChatCommand command()       { return _currentCommand; }
    virtual void appendMessage(QString from, QString message);
    bool operator== (const unsigned int& chatID) const;

    void addParticipant(QString&);
    void removeParticipant(const QString&);
    void removeParticipant(const QStringList&);
    void setParticipants(const QStringList&);
    QStringList participants()  { return _participants.stringList(); }

signals:
    void newCommand();
    void chatEmpty();
    void dataChanged();

protected:
    unsigned int _ID;
    QStringListModel _participants;
    QStringListModel _messages;
    ChatCommand _currentCommand;

    void closeEvent(QCloseEvent *);
};

#endif // CHAT_H
