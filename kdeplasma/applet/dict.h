/*
 *   Copyright (C) 2008 Nick Shaforostoff <shaforostoff@kde.ru>
 *
 *   based on work by:
 *   Copyright (C) 2007 Thomas Georgiou <TAGeorgiou@gmail.com> and Jeff Cooper <weirdsox11@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of 
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DICT_H
#define DICT_H

#include <Plasma/Applet>
#include <Plasma/DataEngine>

class QTimer;
class CheckableStringListModel;

class QTextBrowser;
class QGraphicsProxyWidget;
class QGraphicsLinearLayout;

namespace Plasma
{
    class IconWidget;
    class LineEdit;
}

class QStarDictApplet: public Plasma::Applet
{
    Q_OBJECT
    public:
        QStarDictApplet(QObject *parent, const QVariantList &args);
        ~QStarDictApplet();

    void init();
        void setPath(const QString&);

    public slots:
        void dataUpdated(const QString &name, const Plasma::DataEngine::Data &data);
        void autoDefine(const QString &word);
        void linkDefine(const QString &word);

    protected slots:
        void define();
        void configAccepted();

    protected:
        void createConfigurationInterface(KConfigDialog *parent);

    private:
        QString m_source;
        QTimer* m_timer;
        //QGraphicsPixmapItem *m_graphicsIcon;
        QGraphicsLinearLayout *m_layout;
        QGraphicsLinearLayout *m_horLayout;
        Plasma::LineEdit *m_wordEdit;
        //Plasma::Flash *m_flash;
        QStringList m_defList;
        QStringList::iterator m_i;
        QTextBrowser* m_defBrowser;
        QGraphicsProxyWidget *m_defBrowserProxy;
        Plasma::IconWidget *m_icon;

//         QList< QPair<QString, bool> > m_dicts;
        QPointer<CheckableStringListModel> m_dictsModel;
        QStringList m_dicts;
        QHash<QString,bool> m_activeDicts;
};

K_EXPORT_PLASMA_APPLET(qstardict, QStarDictApplet)

#endif
