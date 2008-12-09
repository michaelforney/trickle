/***************************************************************************
 *   Copyright (C) 2007 by Michael Forney                                  *
 *   michael@obberon.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "settingsdelegate.h"

#include <QPainter>
#include <QModelIndex>

SettingsDelegate::SettingsDelegate()
 : QAbstractItemDelegate()
{
}

SettingsDelegate::~SettingsDelegate()
{
}

void SettingsDelegate::paint(QPainter * painter, const QStyleOptionViewItem & style, const QModelIndex & index) const
{
	if (style.state & QStyle::State_Selected || style.state & QStyle::State_Active)
	{
		painter->fillRect(style.rect, style.palette.highlight());
		painter->setPen(style.palette.color(QPalette::HighlightedText));
	}
	painter->save();
	
	QFont pageFont("Sans Serif", 10);
	pageFont.setBold(true);
	
	painter->setFont(pageFont);
	painter->drawText(QRect(style.rect.x() + 40, style.rect.y(), style.rect.width() - 40, style.rect.height()), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, index.model()->data(index).toString());
	index.model()->data(index, Qt::DecorationRole).value<QIcon>().paint(painter, 4, 4, 32, 32);
	painter->restore();
}

QSize SettingsDelegate::sizeHint(const QStyleOptionViewItem & style, const QModelIndex & index) const
{
	return QSize(120, 40);
}
