#include "popupwindow.h"

#include <QApplication>
#include <QClipboard>
#include <QSettings>
#include <QTextBrowser>
#include <QTimer>
#include "dictcore.h"

PopupWindow::PopupWindow(DictCore *dict, QWidget *parent)
    : QWidget(parent, Qt::ToolTip)
{
    if (! dict)
        m_dict = new DictCore(this);
    else
        m_dict = dict;
    translationView = new QTextBrowser(this);
    closeTimer = new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), SLOT(close()));
    connect(closeTimer, SIGNAL(timeout()), closeTimer, SLOT(stop()));
    
    QSettings config;
    setScan(config.value("PopupWindow/scan", true).toBool());
    setModifierKey(config.value("PopupWindow/modifierKey", 0).toInt());
    setShowIfNotFound(config.value("PopupWindow/showIfNotFound", true).toBool());
}

PopupWindow::~PopupWindow()
{
    QSettings config;
    config.setValue("PopupWindow/scan", m_scan);
    config.setValue("PopupWindow/modifierKey", m_modifierKey);
    config.setValue("PopupWindow/showIfNotFound", m_showIfNotFound);
}

void PopupWindow::setScan(bool scan)
{
    if (m_scan == scan)
        return;
    m_scan = scan;
    if (m_scan)
        connect(qApp->clipboard(), SIGNAL(selectionChanged()), this, SLOT(xSelectionChanged()));
    else
        disconnect(qApp->clipboard(), SIGNAL(selectionChanged()), this, SLOT(xSelectionChanged()));
    emit scanChanged(scan);
}

bool PopupWindow::isScan() const
{
    return m_scan;
}

void PopupWindow::setModifierKey(int key)
{
    m_modifierKey = key;
}

int PopupWindow::modifierKey() const
{
    return m_modifierKey;
}

bool PopupWindow::showIfNotFound() const
{
    return m_showIfNotFound;
}

void PopupWindow::setShowIfNotFound(bool mode)
{
    m_showIfNotFound = mode;
}

DictCore* PopupWindow::dict() const
{
    return m_dict;
}

void PopupWindow::xSelectionChanged()
{
    if (m_modifierKey && ! qApp->keyboardModifiers().testFlag(static_cast<Qt::KeyboardModifier>(m_modifierKey)))
        return;
    QString m_source = qApp->clipboard()->text(QClipboard::Selection);
    m_source.remove(QRegExp("^\\W"));
    m_source.remove(QRegExp("\\W.*$"));
    if (m_showIfNotFound || m_dict->isTranslatable(m_source))
    {
        QString translated = m_dict->translate(m_source, true);
        translationView->setHtml(translated);
        translationView->adjustSize();
        move(cursor().pos() - QPoint(30, 30));
    }
    show();
}

void PopupWindow::enterEvent(QEvent*)
{
    closeTimer->stop();
}

void PopupWindow::leaveEvent(QEvent*)
{
    closeTimer->start(200);
}

