#include "customplaintextedit.h"

customPlainTextEdit::customPlainTextEdit(QWidget *parent) :
    QPlainTextEdit(parent)
{
}

void customPlainTextEdit::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Return) {
        if (e->modifiers() != Qt::ControlModifier &&
            !e->isAutoRepeat()) {
            emit enterPressed();
            e->ignore();
        } else {
            // Ugly, I know, but appendPlainText("\n")
            // sometimes appends two \n characters
            QString text = document()->toPlainText();
            clear();
            text.append("\n");
            appendPlainText(text);
            e->accept();
        }
    } else {
        QPlainTextEdit::keyPressEvent(e);
    }
}
