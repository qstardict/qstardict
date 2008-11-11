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


#ifndef DICTENGINE_H
#define DICTENGINE_H
#include <Plasma/DataEngine>
#include <QString>
#include <QList>
namespace QStarDict {class DictPlugin;}

/**
 * This class evaluates the basic expressions given in the interface.
 */


class QStarDictEngine : public Plasma::DataEngine
{
    Q_OBJECT

    public:
        QStarDictEngine(QObject* parent, const QVariantList& args );
        ~QStarDictEngine();

    protected:
        bool sourceRequestEvent(const QString &word);
    private:
        /**
         * also loads plugin if it's not loaded
         */
        QStarDict::DictPlugin* dictPlugin(const QString &name);

    private slots:
        void unloadPlugins();
    private:
        class Private;
        Private * const d;
};

K_EXPORT_PLASMA_DATAENGINE(qstardict, QStarDictEngine)

#endif
