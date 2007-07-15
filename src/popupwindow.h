#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QFrame>
#include "dictcore.h"

class QAction;
class QEvent;
class QTimer;
class QClipboard;

class DictWidget;

class PopupWindow: public QFrame
{
        Q_OBJECT

    public:
        PopupWindow(DictCore *dict = NULL, QWidget *parent = 0);
        ~PopupWindow();

        bool isScan() const;
        int modifierKey() const;
        bool showIfNotFound() const;
        int timeoutBeforeHide() const;
        const QSize& defaultSize() const;
        DictCore::TranslationFlags translationFlags() const;

        DictCore* dict() const;

    public slots:
        void setScan(bool scan);
        void setModifierKey(int key);
        void setShowIfNotFound(bool mode);
        void setTimeoutBeforeHide(int timeoutBeforeHide);
        void setDefaultSize(const QSize &defaultSize);
        void setTranslationFlags(DictCore::TranslationFlags translationFlags);

    signals:
        void scanChanged(bool);

    protected:
        void enterEvent(QEvent*);
        void leaveEvent(QEvent*);
        void timerEvent(QTimerEvent*);

    private slots:
        void xSelectionChanged();

    private:
        DictCore* m_dict;
        DictWidget *translationView;
        QTimer *closeTimer;
        QString m_source;
        bool m_scan;
        int m_modifierKey;
        bool m_showIfNotFound;
        QString lastSelection;
        int timerId;
        int m_timeoutBeforeHide;
        QSize m_defaultSize;
};

#endif // POPUPWINDOW_H

