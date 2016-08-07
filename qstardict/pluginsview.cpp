/*****************************************************************************
 * pluginsview.cpp - Code adapted from QtNote project                        *
 * Copyright (C) 2016 Sergey Il'inykh                                        *
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


#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QPainter>

#include "application.h"
#include "pluginsview.h"

namespace QStarDict {

class ButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    enum ButtonRoles {
        ButtonRole = Qt::UserRole + 1
    };

    QModelIndex sunken;

public:
    explicit ButtonDelegate(QObject *parent = 0) :
        QStyledItemDelegate(parent)
    {
    }

    // painting
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);
        if (opt.icon.isNull()) {
            return;
        }
        painter->save();
        if (opt.state & QStyle::State_Selected) {
            painter->setPen(QPen(Qt::NoPen));
            if (opt.state & QStyle::State_Active) {
              painter->setBrush(QBrush(QPalette().highlight()));
            }
            else {
              painter->setBrush(QBrush(QPalette().color(QPalette::Inactive,
                                                        QPalette::Highlight)));
            }
            painter->drawRect(opt.rect);
          }

        QStyleOptionButton buttonOption;
        buttonOption.icon = opt.icon;
        buttonOption.iconSize = option.decorationSize;
        buttonOption.text = opt.text;
        buttonOption.features = QStyleOptionButton::Flat;
        buttonOption.rect = opt.rect;
        buttonOption.state = QStyle::State_Enabled;
        if (index == sunken) {
            buttonOption.state |= QStyle::State_Sunken;
        }
        if (option.state & QStyle::State_MouseOver) {
            buttonOption.state |= (QStyle::State_Active | QStyle::State_MouseOver);
        }

        QApplication::style()->drawControl(QStyle::CE_PushButton,
                                           &buttonOption,
                                           painter);
        painter->restore();
    }

    bool editorEvent(QEvent *event,
        QAbstractItemModel *model,
        const QStyleOptionViewItem &option,
        const QModelIndex &index)
    {
        Q_UNUSED(model);
        Q_UNUSED(option);

        if(!(event->type() == QEvent::MouseButtonPress ||
            event->type() == QEvent::MouseButtonRelease)) {
            return true;
        }

        sunken = QModelIndex();

        if( event->type() == QEvent::MouseButtonPress) {
            sunken = index;
        }
        return true;
    }
};


PluginsView::PluginsView(QWidget *parent) :
    QTableView(parent)
{
    verticalHeader()->hide();
    horizontalHeader()->hide();
    //setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::NoSelection);
    setEditTriggers(NoEditTriggers);
    setShowGrid(false);
}

void PluginsView::configureColumns()
{
    ButtonDelegate *btnsDelegate = new ButtonDelegate();
    setItemDelegateForColumn(2, btnsDelegate);
    setItemDelegateForColumn(3, btnsDelegate);

#if QT_VERSION >= 0x050000
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
#else
    horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    horizontalHeader()->setResizeMode(3, QHeaderView::ResizeToContents);
#endif
}

} // namespace QStarDict

#include "pluginsview.moc"
