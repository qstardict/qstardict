#include "popupwindow.h"

#include <QApplication>
#include <QClipboard>
#include <QDesktopWidget>
#include <QSettings>
#include <QTextBrowser>
#include <QTimer>
#include "dictcore.h"
#include "keyboard.h"

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
    timerId = 0;

    QSettings config;
    setScan(config.value("PopupWindow/scan", true).toBool());
    setModifierKey(config.value("PopupWindow/modifierKey", 0).toInt());
    setShowIfNotFound(config.value("PopupWindow/showIfNotFound", false).toBool());
    setWindowOpacity(config.value("PopupWindow/opacity", 1.0).toDouble());
}

PopupWindow::~PopupWindow()
{
    QSettings config;
    config.setValue("PopupWindow/scan", m_scan);
    config.setValue("PopupWindow/modifierKey", m_modifierKey);
    config.setValue("PopupWindow/showIfNotFound", m_showIfNotFound);
    config.setValue("PopupWindow/opacity", windowOpacity());
}

void PopupWindow::setScan(bool scan)
{
    if (m_scan == scan)
        return ;
    m_scan = scan;
    if (m_scan)
    {
        lastSelection = QApplication::clipboard()->text(QClipboard::Selection);
        timerId = startTimer(300);
    }
    else
        killTimer(timerId);
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

void PopupWindow::timerEvent(QTimerEvent*)
{
    if (lastSelection != QApplication::clipboard()->text(QClipboard::Selection))
    {
        lastSelection = QApplication::clipboard()->text(QClipboard::Selection);
        xSelectionChanged();
    }
}

void PopupWindow::xSelectionChanged()
{
    if (m_modifierKey && ! Keyboard::activeModifiers().testFlag(static_cast<Qt::KeyboardModifier>(m_modifierKey)))
        return ;
    QString m_source = QApplication::clipboard()->text(QClipboard::Selection);
    m_source.remove(QRegExp("^\\W"));
    m_source.remove(QRegExp("\\W.*$"));
    if (m_showIfNotFound || m_dict->isTranslatable(m_source))
    {
        QString translated = m_dict->translate(m_source, DictCore::Simple | DictCore::Html);
        translationView->setHtml(translated);
        translationView->adjustSize();

        QPoint newPosition = cursor().pos() - QPoint(30, 30);
        if (newPosition.x() < 0)
            newPosition.setX(0);
        else if (newPosition.x() + width() > QApplication::desktop()->width())
            newPosition.setX(QApplication::desktop()->width() - width());
        if (newPosition.y() < 0)
            newPosition.setY(0);
        else if (newPosition.y() + height() > QApplication::desktop()->height())
            newPosition.setY(QApplication::desktop()->height() - height());
        move(newPosition);
        show();
    }
}

void PopupWindow::enterEvent(QEvent*)
{
    closeTimer->stop();
}

void PopupWindow::leaveEvent(QEvent*)
{
    closeTimer->start(200);
}

