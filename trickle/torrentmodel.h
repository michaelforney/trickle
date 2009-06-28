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
#ifndef TORRENTMODEL_H
#define TORRENTMODEL_H

#include <QAbstractItemModel>

/**
	@author Michael Forney <mforney@trickleproject.org>
*/

#include <QList>
#include <QMap>
#include <QStringList>
#include <QtAlgorithms>

#include "torrent.h"

class Interface;

class TorrentModel : public QAbstractItemModel
{
	Q_OBJECT
	Q_ENUMS(Columns)

	public:
		TorrentModel();
		~TorrentModel();

		enum Columns { Name, State, Size, Seeders, Leechers, DownloadRate, Downloaded, UploadRate, Uploaded, Ratio, Priority, Hash };

		//Virtual QAbstractItemModel functions
		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		QModelIndex parent(const QModelIndex & index) const;
		QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
		QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
		Qt::ItemFlags flags(const QModelIndex & index) const;

		ByteSize totalDownloadRate() const;
		ByteSize totalUploadRate() const;

		Torrent torrent(const QString & hash) const;
        Torrent torrent(int row) const;
        Torrent torrent(const QModelIndex & index) const;
        QString hash(int row) const;
        QString hash(const QModelIndex & index) const;
	public slots:
		void clear();
		void torrentsUpdated(const QMap<QString, Torrent> & torrentMap);
		void setupInterfaceConnections(Interface * interface);
	private:
		QList<QVariant> headers;
		QMap<QString, Torrent> torrents;
		QStringList units;
};

#endif
