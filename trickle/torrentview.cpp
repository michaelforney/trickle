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
#include "torrentview.h"

#include <QHeaderView>

#include "torrentmodel.h"
#include "selectedtorrent.h"
#include "torrentsortmodel.h"

TorrentView::TorrentView()
 : QTreeView()
{
	setUniformRowHeights(true);
	setItemsExpandable(false);
	setAlternatingRowColors(true);
	setSortingEnabled(true);
	setIndentation(0);
	
	connect(this, SIGNAL(clicked(const QModelIndex &)), this, SLOT(setCurrentTorrent(const QModelIndex &)));
	connect(this, SIGNAL(torrentChanged(TorrentItem *)), SelectedTorrent::instance(), SLOT(setTorrent(TorrentItem *)));
}

TorrentView::~TorrentView()
{
}

void TorrentView::setCurrentTorrent(const QModelIndex & index)
{
	emit torrentChanged(static_cast<TorrentItem *>(static_cast<const TorrentSortModel *>(index.model())->mapToSource(index).internalPointer()));
}

#include "torrentview.moc"
