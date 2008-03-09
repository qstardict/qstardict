/*****************************************************************************
 * dictinfo.h - QStarDict, a StarDict clone written with using Qt            *
 * Copyright (C) 2008 Alexander Rodin                                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               *
 *****************************************************************************/

#ifndef DICTINFO_H
#define DICTINFO_H

#include <QString>

namespace QStarDict
{

/**
 * This class represents information about dictionary.
 */
class DictInfo
{
    public:
        /**
         * Constructs empty DictInfo object.
         */
        DictInfo()
            : m_wordsCount(-1)
        { }
        /**
         * Constructs DictInfo object from data.
         * @param plugin A plugin name
         * @param name A dictionary name
         * @param fullName A full dictionary name
         * @param author A dictionary author
         * @param email An author e-mail
         * @param webSite A dictionary website
         * @param desription A dictionary description
         * @param wordsCount A count of words that avialable in dictionary
         */
        DictInfo(const QString &plugin,
                 const QString &name,
                 const QString &author = QString(),
                 const QString &email = QString(),
                 const QString &webSite = QString(),
                 const QString &description = QString(),
                 long wordsCount = -1)
            : m_plugin(plugin),
              m_name(name),
              m_author(author),
              m_email(email),
              m_webSite(webSite),
              m_description(description),
              m_wordsCount(wordsCount)
        { }

        const QString &plugin() const
        { return m_plugin; }
        const QString &name() const
        { return m_name; }
        const QString &author() const
        { return m_author; }
        const QString &email() const 
        { return m_email; }
        const QString &webSite() const 
        { return m_webSite; }
        const QString &description() const
        { return m_description; }
        long wordsCount() const
        { return m_wordsCount; }

        void setPlugin(const QString &plugin)
        { m_plugin = plugin; }
        void setName(const QString &name)
        { m_name = name; }
        void setAuthor(const QString &author)
        { m_author = author; }
        void setEmail(const QString &email)
        { m_email = email; }
        void setWebSite(const QString &webSite)
        { m_webSite = webSite; }
        void setDescription(const QString &description)
        { m_description = description; }
        void setWordsCount(long wordsCount)
        { m_wordsCount = wordsCount; }

    private:
        QString m_plugin;
        QString m_name;
        QString m_author;
        QString m_email;
        QString m_webSite;
        QString m_description;
        long m_wordsCount;
};

}

#endif // DICTINFO_H

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent

