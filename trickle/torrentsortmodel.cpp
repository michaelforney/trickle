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
#include "torrentsortmodel.h"

#include "torrent.h"
#include "torrentmodel.h"

TorrentSortModel::TorrentSortModel()
 : QSortFilterProxyModel()
{
	setDynamicSortFilter(true);
}


TorrentSortModel::~TorrentSortModel()
{
}

bool TorrentSortModel::lessThan(const QModelIndex & left, const QModelIndex & right) const
{
	Torrent leftItem = static_cast<TorrentModel *>(sourceModel())->torrent(left.row());
	Torrent rightItem = static_cast<TorrentModel *>(sourceModel())->torrent(right.row());
	
	if (left.column() == right.column())
	{
		int column = left.column();
		switch (column)
		{
			case TorrentModel::Name:
				return leftItem.name().toLower() < rightItem.name().toLower();
			case TorrentModel::State:
				return leftItem.state() < rightItem.state();
			case TorrentModel::Size:
				return leftItem.size() < rightItem.size();
			case TorrentModel::Seeders:
				return leftItem.seedsConnected() < rightItem.seedsConnected();
			case TorrentModel::Leechers:
				return leftItem.leechsConnected() < rightItem.leechsConnected();
			case TorrentModel::DownloadRate:
				return leftItem.downloadRate() < rightItem.downloadRate();
			case TorrentModel::Downloaded:
				return leftItem.downloaded() < rightItem.downloaded();
			case TorrentModel::UploadRate:
				return leftItem.uploadRate() < rightItem.uploadRate();
			case TorrentModel::Uploaded:
				return leftItem.uploaded() < rightItem.uploaded();
			case TorrentModel::Ratio:
				return leftItem.ratio() < rightItem.ratio();
			case TorrentModel::Priority:
				return leftItem.priority() < rightItem.priority();
			default:
				return false;
		}
	}
	else
	{
		return false;
	}
}


