#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QWidget>

class QAction;
class QEvent;
class QTextBrowser;
class QTimer;

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

        DictCore* dict() const;

    public slots:
        void setScan(bool scan);
        void setModifierKey(int key);
        void setShowIfNotFound(bool mode);

    signals:
        void scanChanged(bool);

    protected:
        void enterEvent(QEvent*);
        void leaveEvent(QEvent*);

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
};

#endif // POPUPWINDOW_H

