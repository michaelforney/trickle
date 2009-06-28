/***************************************************************************
 *   Copyright (C) 2007 by Michael Forney                                  *
 *   mforney@trickleproject.org                                            *
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
#include "torrentdelegate.h"

#include <QComboBox>
#include <QModelIndex>
#include <QDebug>

#include "torrent.h"
#include "torrentmodel.h"
#include "torrentsortmodel.h"

TorrentDelegate::TorrentDelegate()
 : QItemDelegate()
{
}


TorrentDelegate::~TorrentDelegate()
{
}

QWidget * TorrentDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & style, const QModelIndex & index) const
{
    Q_UNUSED(style);
	if (index.column() == TorrentModel::Priority)
	{
        Torrent * item = static_cast<Torrent *>(static_cast<const QAbstractProxyModel *>(index.model())->mapToSource(index).internalPointer()); //FIXME: not using torrent pointers anymore
		QComboBox * prioritySelector = new QComboBox(parent);
		prioritySelector->addItems(QStringList() << "Off" << "Low" << "Normal" << "High");
		prioritySelector->setCurrentIndex(item->priority());
		return prioritySelector;
	}
	return 0;
}
