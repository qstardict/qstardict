/*****************************************************************************
 * dictcore.cpp - QStarDict, a StarDict clone written with using Qt          *
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

#include "dictcore.h"

namespace QStarDict
{

DictCore::DictCore(QObject *parent)
    : QObject(parent)
{
    
}

DictCore::~DictCore()
{
    
}

bool DictCore::isTranslatable(const QString &word)
{
    
}

QString DictCore::translate(const QString &word)
{
    
}

QStringList DictCore::findSimilarWords(const QString &word)
{
    
}

QStringList DictCore::avilablePlugins() const
{
    
}

void DictCore::setLoadedPlugins(const QStringList &loadedPlugins)
{
    
}

QList<QPair<QString, QString> > DictCore::avialableDicts() const
{
    
}

void DictCore::setLoadedDicts(const QList<QPair<QString, QString> > &loadedDicts)
{
    
}

void DictCore::saveSettings()
{
    
}

void DictCore::loadSettings()
{
    
}

// vim: tabstop=4 softtabstop=4 shiftwidth=4 expandtab cindent textwidth=120 formatoptions=tc

