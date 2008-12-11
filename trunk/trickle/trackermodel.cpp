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
#include "trackermodel.h"

#include <QDebug>
#include <QModelIndex>

#include "trackeritem.h"
#include "torrent.h"
//#include "selectedtorrent.h"
#include "xmlrpc.h"

TrackerModel::TrackerModel()
 : QAbstractItemModel()
{
	headers << "Tracker Url";
	//connect(SelectedTorrent::instance(), SIGNAL(torrentChanged(TorrentItem*)), this, SLOT(update()));
}


TrackerModel::~TrackerModel()
{
}

QModelIndex TrackerModel::index(int row, int column, const QModelIndex & parent) const
{
	if (parent.isValid())
	{
		return QModelIndex();
	}
	else
	{
		return createIndex(row, column, trackers[row]);
	}
}

QModelIndex TrackerModel::parent(const QModelIndex & /*index*/) const
{
	return QModelIndex();
}

int TrackerModel::rowCount(const QModelIndex & parent) const
{
	if (parent.isValid())
	{
		return 0;
	}
	else
	{
		return trackers.size();
	}
}

int TrackerModel::columnCount(const QModelIndex & /*parent*/) const
{
	return headers.size();
}

QVariant TrackerModel::data(const QModelIndex & index, int role) const
{
	if (index.isValid())
	{
		if (role == Qt::DisplayRole)
		{
			if (index.column() == TrackerModel::TrackerUrl)
			{
				return trackers[index.row()]->url();
			}
			else
			{
				return QVariant();
			}
		}
		else
		{
			return QVariant();
		}
	}
	else
	{
		return QVariant();
	}
}

QVariant TrackerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
	{
		if (role == Qt::DisplayRole)
		{
			return headers[section];
		}
		else
		{
			return QVariant();
		}
	}
	else
	{
		return QVariant();
	}
}

void TrackerModel::update()
{
	/*if (SelectedTorrent::instance()->hasTorrent())
	{
		QVariantList args;
		args << SelectedTorrent::torrentInstance()->hash();
		args << "";
		args << "t.get_url=";
		//XmlRpc::instance()->call("t.multicall", args, this, "result");
	}*/
}

void TrackerModel::result(const QString & method, const QVariant & valueVariant)
{
	if (method == "t.multicall")
	{
		QVariantList value = valueVariant.value<QVariantList>();
		trackers.clear();
		foreach(QVariant trackerVariant, value)
		{
			QVariantList tracker = trackerVariant.value<QVariantList>();
			TrackerItem * trackerItem = new TrackerItem();
			{
				trackerItem->setUrl(tracker[0].toString());
			}
			trackers.append(trackerItem);
		}
		emit layoutChanged();
	}
}

#include "trackermodel.moc"
