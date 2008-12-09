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
#include <QDebug>

#include "xmlrpc.h"
#include "updatetimer.h"
#include "selectedtorrent.h"
#include "serverstatus.h"

TorrentModel::TorrentModel()
 : QAbstractItemModel()
{
	headers << "Name" << "State" << "Size" << "Seeders" << "Leechers" << "Down Rate" << "Downloaded" << "Up Rate" << "Uploaded" << "Ratio" << "Priority" << "Hash";
	connect(this, SIGNAL(layoutChanged()), SelectedTorrent::instance(), SLOT(update()));
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
	TorrentItem * item = static_cast<TorrentItem *>(index.internalPointer());
	switch (role)
	{
		case Qt::DisplayRole:
		{
			if (item)
			{
				switch (index.column())
				{
					case Name:
						return item->name();
					case State:
					{
						switch (item->state())
						{
							case TorrentItem::Downloading:
								return QString("Downloading");
							case TorrentItem::Seeding:
								return QString("Seeding");
							case TorrentItem::Stopped:
								return QString("Stopped");
							case TorrentItem::Completed:
								return QString("Completed");
							default:
								return QVariant();
						}
					}
					case Size:
						return ByteSize(item->chunks(), item->chunkSize()).toString();
					case DownloadRate:
						return QString("%1/s").arg(item->downloadRate().toString());
					case Downloaded:
						return item->downloaded().toString();
					case UploadRate:
						return QString("%1/s").arg(item->uploadRate().toString());
					case Uploaded:
						return item->uploaded().toString();
					case Seeders:
						return QString("%1 (%2)").arg(item->seedsConnected()).arg(item->seedsTotal());
					case Leechers:
						return QString("%1 (%2)").arg(item->leechsConnected()).arg(item->leechsTotal());
					case Ratio:
						return QString::number(item->ratio(), 'f', 3);
					case Priority:
					{
						switch (item->priority())
						{
							case TorrentItem::Off:
								return QString("Off");
							case TorrentItem::Low:
								return QString("Low");
							case TorrentItem::Medium:
								return QString("Medium");
							case TorrentItem::High:
								return QString("High");
							default:
								return QVariant();
						}
					}
					case Hash:
						return item->hash();
					default:
						return QVariant();
				}
			}
			else
			{
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
					if (item->ratio() < 0.85)
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
					switch(item->state())
					{
						case TorrentItem::Downloading:
							return Qt::darkBlue;
						case TorrentItem::Seeding:
							return Qt::darkGreen;
						case TorrentItem::Stopped:
							return QVariant();
						case TorrentItem::Completed:
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
	
	TorrentItem * item = torrents[row];

	return createIndex(row, column, item);
}

bool TorrentModel::canFetchMore() const
{
	return false;
}

void TorrentModel::clear()
{
	torrents.clear();
	emit layoutChanged();
}

TorrentItem * TorrentModel::item(const QString & hash) const
{
	foreach(TorrentItem * item, torrents)
	{
		if (item->hash() == hash)
		{
			return item;
		}
	}
	return 0;
}

void TorrentModel::update()
{
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
		foreach(TorrentItem * item, torrents)
		{
			item->update();
		}
		emit logInfo(QString("Updating torrent information"));
	}
	else
	{
		emit logInfo(QString("XmlRpc busy with %1 requests").arg(XmlRpc::instance()->requests()));
	}*/
}

void TorrentModel::result(const QString & method, const QVariant & result)
{
	if (method == "d.multicall")
	{
		QVariantList resultList = result.value<QVariantList>();
		//clear();
		int i = 0;
		for (QList<TorrentItem *>::iterator it = torrents.begin(); it != torrents.end(); ++it)
		{
			bool found = false;
			foreach (QVariant downloadVariant, resultList)
			{
				if ((*it)->hash() == downloadVariant.value<QVariantList>()[10].toString())
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				beginRemoveRows(QModelIndex(), i, i);
				torrents.erase(it);
				endRemoveRows();
			}
			i++;
		}
		QList<TorrentItem *> newTorrents;
		foreach (QVariant downloadVariant, resultList)
		{

			QVariantList download = downloadVariant.value<QVariantList>();
			
			QString name = download[0].toString();
			int state = download[1].toInt();
			int chunks = download[2].toInt();
			ByteSize chunkSize = download[3].toInt();
			ByteSize downloadRate = download[4].toInt();
			int completedChunks = download[5].toInt();
			ByteSize downloadedInt = download[6].toInt();
			ByteSize uploadRate = download[7].toInt();
			ByteSize uploadedInt = download[8].toInt();
			double ratio = static_cast<double>(download[9].toInt()) / 1000;
			QString hash = download[10].toString();
			int leechsConnected = download[11].toInt();
			int seedsConnected = download[12].toInt();
			int priority = download[13].toInt();
			
			TorrentItem * item;
			bool found = false;
			foreach (TorrentItem * oldItem, torrents)
			{
				if (hash == oldItem->hash())
				{
					item = oldItem;
					found = true;
					break;
				}
			}
			if (!found)
			{
				item = new TorrentItem(hash);
				newTorrents.append(item);
			}
			item->setName(name);
			item->setState(state);
			item->setChunkSize(chunkSize);
			item->setChunks(chunks);
			item->setDownloadRate(downloadRate);
			
			ByteSize downloaded;
			if (qAbs(ByteSize(downloadedInt - ByteSize(completedChunks, chunkSize)).bytes()) < chunkSize.bytes())
			{
				downloaded = downloadedInt;
			}
			else
			{
				downloaded = ByteSize(completedChunks, chunkSize);
			}
			item->setDownloaded(downloaded);
			
			item->setCompletedChunks(completedChunks);
			item->setUploadRate(uploadRate);
			
			ByteSize uploaded;
			if (qAbs(uploadedInt.bytes() - (ratio * downloaded.bytes())) < chunkSize.bytes())
			{
				uploaded = uploadedInt;
			}
			else
			{
				uploaded = static_cast<qint64>(ratio * downloaded.bytes());
			}
			item->setUploaded(uploaded);
			
			item->setRatio(ratio);
			item->setLeechsConnected(leechsConnected);
			item->setSeedsConnected(seedsConnected);
			item->setPriority(priority);
			
			
			//connect(item, SIGNAL(dataChanged(TorrentItem *)), this, SLOT(updateItem(TorrentItem *)));
		}
		if (torrents.size() > 0)
		{
			emit dataChanged(createIndex(0, 0, torrents.first()), createIndex(torrents.size() - 1, headers.size() - 1, torrents.last()));
		}
		if (newTorrents.size() > 0)
		{
			beginInsertRows(QModelIndex(), rowCount(), rowCount() + newTorrents.size() - 1);
			torrents += newTorrents;
			endInsertRows();
		}
		//emit layoutChanged();
	}
}

ByteSize TorrentModel::totalDownloadRate() const
{
	ByteSize total;
	foreach(TorrentItem * item, torrents)
	{
		total += item->downloadRate();
	}
	return total;
}

ByteSize TorrentModel::totalUploadRate() const
{
	ByteSize total;
	foreach(TorrentItem * item, torrents)
	{
		total += item->uploadRate();
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

void TorrentModel::updateItem(TorrentItem * item)
{
	int row = torrents.indexOf(item);
	//emit dataChanged(createIndex(row, 0, item), createIndex(row, headers.count(), item));
	//emit layoutChanged();
}

#include "torrentmodel.moc"
