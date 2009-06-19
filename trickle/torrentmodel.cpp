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
#include "torrentmodel.h"

#include <QTimer>
#include <KDebug>

#include "xmlrpc.h"
#include "updatetimer.h"
#include "interfacemanager.h"
#include "serverstatus.h"
#include "interface.h"

TorrentModel::TorrentModel()
 : QAbstractItemModel()
{
	headers << "Name" << "State" << "Size" << "Seeders" << "Leechers" << "Down Rate" << "Downloaded" << "Up Rate" << "Uploaded" << "Ratio" << "Priority" << "Hash";
	//connect(this, SIGNAL(layoutChanged()), SelectedTorrent::instance(), SLOT(update()));
    connect(InterfaceManager::self(), SIGNAL(interfaceChanged(Interface *)), this, SLOT(setupInterfaceConnections(Interface *)));
    if (InterfaceManager::interface())
    {
        setupInterfaceConnections(InterfaceManager::interface());
    }
	//connect(ServerStatus::instance(), SIGNAL(serverChanged(int)), this, SLOT(clear()));
}

TorrentModel::~TorrentModel()
{
}

int TorrentModel::rowCount(const QModelIndex & parent) const
{
	if (parent.isValid())
	{
		return 0;
	}
	else
	{
		return torrents.size();
	}
}

int TorrentModel::columnCount(const QModelIndex &) const
{
	return headers.size();
}

QVariant TorrentModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		return headers[section];
	}
	else
	{
		return QVariant();
	}
}

QModelIndex TorrentModel::parent(const QModelIndex &) const
{
	return QModelIndex();
}

QVariant TorrentModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}
	Torrent torrent = torrents.value(*static_cast<QString *>(index.internalPointer()));
	switch (role)
	{
		case Qt::DisplayRole:
		{
			switch (index.column())
			{
				case Name:
                    return torrent.name();
				case State:
				{
                    switch (torrent.state())
					{
						case Torrent::Downloading:
							return QString("Downloading");
						case Torrent::Seeding:
							return QString("Seeding");
						case Torrent::Stopped:
							return QString("Stopped");
						case Torrent::Completed:
							return QString("Completed");
						default:
							return QVariant();
					}
				}
				case Size:
                    return ByteSize(torrent.size()).toString();
				case DownloadRate:
                    return QString("%1/s").arg(torrent.downloadRate().toString());
				case Downloaded:
                    return torrent.downloaded().toString();
				case UploadRate:
                    return QString("%1/s").arg(torrent.uploadRate().toString());
				case Uploaded:
                    return torrent.uploaded().toString();
				case Seeders:
                    return QString("%1 (%2)").arg(torrent.seedsConnected()).arg(torrent.seedsTotal());
				case Leechers:
                    return QString("%1 (%2)").arg(torrent.leechsConnected()).arg(torrent.leechsTotal());
				case Ratio:
                    return QString::number(torrent.ratio(), 'f', 3);
				case Priority:
				{
                    switch (torrent.priority())
					{
						case Torrent::Off:
							return QString("Off");
						case Torrent::Low:
							return QString("Low");
						case Torrent::Medium:
							return QString("Medium");
						case Torrent::High:
							return QString("High");
						default:
							return QVariant();
					}
				}
				case Hash:
                    return torrent.hash();
				default:
					return QVariant();
			}
		}
		case Qt::TextAlignmentRole:
		{
			if (index.column() == State || index.column() == Name)
			{
				return Qt::AlignLeft;
			}
			else
			{
				return Qt::AlignRight;
			}
		}
		case Qt::ForegroundRole:
		{
			switch(index.column())
			{
				case Ratio:
				{
                    if (torrent.ratio() < 0.85)
					{
						return Qt::red;
					}
					else
					{
						return Qt::darkGreen;
					}
				}
				case State:
				{
                    switch(torrent.state())
					{
						case Torrent::Downloading:
							return Qt::darkBlue;
						case Torrent::Seeding:
							return Qt::darkGreen;
						case Torrent::Stopped:
							return QVariant();
						case Torrent::Completed:
							return QVariant();
						default:
							return QVariant();
					}
				}
				default:
					return QVariant();
			}
		}
		default:
			return QVariant();
	}
}

QModelIndex TorrentModel::index(int row, int column, const QModelIndex & parent) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}
	
	//Torrent * item = torrents[row];
	QString * hash = new QString(torrents.keys().at(row));

	return createIndex(row, column, hash);
}

void TorrentModel::clear()
{
	torrents.clear();
	emit layoutChanged();
}

void TorrentModel::update()
{
	qDebug() << "update";
	/*if (XmlRpc::instance()->isBusy())
	{
		QVariantList args;
		args << "default";
		args << "d.get_name=";
		args << "d.get_state=";
		args << "d.get_size_chunks=";
		args << "d.get_chunk_size=";
		args << "d.get_down_rate=";
		args << "d.get_completed_chunks=";
		args << "d.get_down_total=";
		args << "d.get_up_rate=";
		args << "d.get_up_total=";
		args << "d.get_ratio=";
		args << "d.get_hash=";
		args << "d.get_peers_accounted=";
		args << "d.get_peers_complete=";
		args << "d.get_priority=";
		//XmlRpc::instance()->call("d.multicall", args, this, "result");
		foreach(Torrent * item, torrents)
		{
			item.update();
		}
		emit logInfo(QString("Updating torrent information"));
	}
	else
	{
		emit logInfo(QString("XmlRpc busy with %1 requests").arg(XmlRpc::instance()->requests()));
	}*/
}

ByteSize TorrentModel::totalDownloadRate() const
{
	ByteSize total;
	foreach(Torrent item, torrents.values())
	{
		total += item.downloadRate();
	}
	return total;
}

ByteSize TorrentModel::totalUploadRate() const
{
	ByteSize total;
	foreach(Torrent item, torrents.values())
	{
		total += item.uploadRate();
	}
	return total;
}

Qt::ItemFlags TorrentModel::flags(const QModelIndex & index) const
{
	Qt::ItemFlags itemFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	if (index.column() == Priority)
	{
		itemFlags |= Qt::ItemIsEditable;
	}
	return itemFlags;
}

void TorrentModel::updateItem(Torrent * item)
{
	//int row = torrents.indexOf(item);
	//emit dataChanged(createIndex(row, 0, item), createIndex(row, headers.count(), item));
	//emit layoutChanged();
}

void TorrentModel::torrentsUpdated(const QMap<QString, Torrent> & torrentMap)
{
	kDebug() << "torrentsUpdated()";
	for (int index = 0; index < torrents.keys().size(); index++)
	{
		QString key = torrents.keys().at(index);
		if (torrentMap.keys().contains(key))
		{
			beginRemoveRows(QModelIndex(), index, index);
			torrents.remove(key);
			endRemoveRows();
		}
	}
	foreach(QString hash, torrentMap.keys())
	{
		if (torrents.contains(hash))
		{
			torrents.insert(hash, torrentMap.value(hash));
		}
		else
		{
			torrents.insert(hash, torrentMap.value(hash));
			int index = torrents.keys().indexOf(hash);
			//qDebug() << index;
			beginInsertRows(QModelIndex(), index, index);
			endInsertRows();
		}
	}
	if (torrents.size() > 0)
	{
		emit dataChanged(createIndex(0, 0), createIndex(torrents.size() - 1, headers.size() - 1));
	}
}

void TorrentModel::setupInterfaceConnections(Interface * interface)
{
	kdDebug() << "setupInterfaceConnections()";
	connect(interface, SIGNAL(torrentsUpdated(const QMap<QString, Torrent> &)), this, SLOT(torrentsUpdated(const QMap<QString, Torrent> &)));
}

Torrent TorrentModel::torrent(const QString & hash) const
{
	return torrents.value(hash);
}

#include "torrentmodel.moc"
