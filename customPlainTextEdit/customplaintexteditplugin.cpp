#include "customplaintextedit.h"
#include "customplaintexteditplugin.h"

#include <QtCore/QtPlugin>

customPlainTextEditPlugin::customPlainTextEditPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void customPlainTextEditPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool customPlainTextEditPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *customPlainTextEditPlugin::createWidget(QWidget *parent)
{
    return new customPlainTextEdit(parent);
}

QString customPlainTextEditPlugin::name() const
{
    return QLatin1String("customPlainTextEdit");
}

QString customPlainTextEditPlugin::group() const
{
    return QLatin1String("");
}

QIcon customPlainTextEditPlugin::icon() const
{
    return QIcon();
}

QString customPlainTextEditPlugin::toolTip() const
{
    return QLatin1String("");
}

QString customPlainTextEditPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool customPlainTextEditPlugin::isContainer() const
{
    return false;
}

QString customPlainTextEditPlugin::domXml() const
{
    return QLatin1String("<widget class=\"customPlainTextEdit\" name=\"customPlainTextEdit\">\n</widget>\n");
}

QString customPlainTextEditPlugin::includeFile() const
{
    return QLatin1String("customplaintextedit.h");
}

Q_EXPORT_PLUGIN2(customplaintexteditplugin, customPlainTextEditPlugin)
