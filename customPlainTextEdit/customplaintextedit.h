#ifndef CUSTOMPLAINTEXTEDIT_H
#define CUSTOMPLAINTEXTEDIT_H

#include <QtGui/QPlainTextEdit>
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT customPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    customPlainTextEdit(QWidget *parent = 0);

signals:
    void enterPressed();

protected:
    void keyPressEvent(QKeyEvent *e);
};

#endif
