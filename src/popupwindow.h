#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QWidget>

class QAction;
class QEvent;
class QTextBrowser;
class QTimer;
class QClipboard;

class DictCore;

class PopupWindow: public QWidget
{
        Q_OBJECT

    public:
        PopupWindow(DictCore *dict = NULL, QWidget *parent = 0);
        ~PopupWindow();

        bool isScan() const;
        int modifierKey() const;
        bool showIfNotFound() const;
        int timeoutBeforeHide() const;

        DictCore* dict() const;

    public slots:
        void setScan(bool scan);
        void setModifierKey(int key);
        void setShowIfNotFound(bool mode);
        void setTimeoutBeforeHide(int timeoutBeforeHide);

    signals:
        void scanChanged(bool);

    protected:
        void enterEvent(QEvent*);
        void leaveEvent(QEvent*);
        void timerEvent(QTimerEvent*);

    private slots:
        void xSelectionChanged();

    private:
        QTextBrowser *translationView;
        QTimer *closeTimer;
        DictCore* m_dict;
        QString m_source;
        bool m_scan;
        int m_modifierKey;
        bool m_showIfNotFound;
        QString lastSelection;
        int timerId;
        int m_timeoutBeforeHide;
};

#endif // POPUPWINDOW_H

