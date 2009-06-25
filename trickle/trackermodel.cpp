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

#include <KDebug>
#include <QModelIndex>

#include "trackeritem.h"
#include "torrent.h"
#include "interfacemanager.h"
#include "interface.h"

TrackerModel::TrackerModel()
 : QAbstractItemModel()
{
	headers << "Tracker Url";
    //connect(SelectedTorrent::instance(), SIGNAL(torrentChanged(TorrentItem*)), this, SLOT(update()));
    connect(InterfaceManager::self(), SIGNAL(interfaceChanged(Interface *)), this, SLOT(setupInterfaceConnections(Interface *)));
    if (InterfaceManager::interface())
    {
        setupInterfaceConnections(InterfaceManager::interface());
    }
}


TrackerModel::~TrackerModel()
{
}

void TrackerModel::setTorrentHash(const QString & hash)
{
    if (m_hash == hash)
    {
        return;
    }
    
    Q_ASSERT(InterfaceManager::interface());
    InterfaceManager::interface()->stopWatchingTorrent(hash);
    m_hash = hash;
    InterfaceManager::interface()->watchTorrent(hash);
}

void TrackerModel::setupInterfaceConnections(Interface * interface)
{
    kDebug() << interface;
    connect(interface, SIGNAL(trackersUpdated(const QString &, const QMap<int, Tracker> &)), this, SLOT(trackersUpdated(const QString &, const QMap<int, Tracker> &)));
}

void TrackerModel::trackersUpdated(const QString & hash, const QMap<int, Tracker> & trackers)
{
    kDebug() << "trackers updated";
    if (m_hash == hash)
    {
        QList<int> currentIds = trackerMap.keys();
        for (int index = 0; index < trackerMap.size(); index++)
        {
            int id = currentIds.at(index);
            if (!trackers.contains(id))
            {
                beginRemoveRows(QModelIndex(), index, index);
                trackerMap.remove(id);
                endRemoveRows();
            }
        }
        for (int index = 0; index < trackers.size(); index++)
        {
            int id = trackers.keys().at(index);
            if (trackerMap.contains(id))
            {
                beginInsertRows(QModelIndex(), index, index);
            }
            else
            {
                beginInsertRows(QModelIndex(), index, index);
                trackerMap.insert(id, trackers.value(id));
                endInsertRows();
            }
        }
        if (trackerMap.size() > 0)
        {
            emit dataChanged(createIndex(0, 0), createIndex(trackerMap.size() - 1, headers.size() - 1));
        }
    }
}

QModelIndex TrackerModel::index(int row, int column, const QModelIndex & parent) const
{
	if (parent.isValid())
	{
		return QModelIndex();
	}
	else
	{
		return createIndex(row, column, trackerMap.keys().at(row));
	}
}

QModelIndex TrackerModel::parent(const QModelIndex & index) const
{
    Q_UNUSED(index);
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
		return trackerMap.size();
	}
}

int TrackerModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
	return headers.size();
}

QVariant TrackerModel::data(const QModelIndex & index, int role) const
{
	if (index.isValid())
	{
        //TODO: Put into switch statement
		if (role == Qt::DisplayRole)
		{
			if (index.column() == TrackerModel::TrackerUrl)
			{
				return trackerMap.value(index.internalId()).url().url();
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

#include "trackermodel.moc"
