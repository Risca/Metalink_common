#ifndef CHATCOMMAND_H
#define CHATCOMMAND_H

#include <QString>
#include <QStringList>
#include <QDebug>
#include "ChatCommand_global.h"

class CHATCOMMAND_EXPORT ChatCommand
{
public:
    enum Type {
        Init,
        Invite,
        List,
        Accept,
        Reject,
        Leave,
        Message,
        Undefined
    };

    ChatCommand();
    ChatCommand(int chatID, Type type, QStringList participants = QStringList());
    ChatCommand(QString & commandString);


    void retranslateCommand(Type type,
                            const QStringList &participants,
                            const QString &message,
                            int newChatID);
    void retranslateCommand(Type type,
                            const QStringList &participants = QStringList(),
                            int newChatID = -1) {
        retranslateCommand(type, participants, QString(), newChatID);
    }
    void retranslateCommand(Type type, const QString &message, int newChatID = -1) {
        retranslateCommand(type, QStringList(), message, newChatID);
    }
    void retranslateCommand(Type type, int newChatID) {
        retranslateCommand(type,QStringList(),QString(), newChatID);
    }
    void retranslateCommand(QString & commandString);

    static QString makeMetaLinkList(const QString &nick);
    static QString makeMetaLinkList(const QStringList &nicks);
    static QStringList parseMetaLinkList(QString &list);

    Type type()                           { return _type; }
    const QStringList & participants()    { return _participants; }
    const QString & commandString()       { return _commandString; }
    int chatID()                          { return _chatID; }
    const QString & message()             { return _message; }

private:
    unsigned int _chatID;
    Type _type;
    QString _commandString;
    QStringList _participants;
    QString _message;
    void parseCommandString();
    void makeCommandString();
};

#endif // CHATCOMMAND_H
