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
#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractItemModel>

/**
	@author Michael Forney <mforney@trickleproject.org>
*/

#include "file.h"

#include <QStringList>

class FileModelItem;
class TorrentItem;
class FileItem;
class Interface;
class Torrent;

class FileModel : public QAbstractItemModel
{
	Q_OBJECT
    friend class FileView;
	public:
		FileModel();
		~FileModel();

		enum Column { Name, Size, Complete, Priority };
        enum IndexType { DirectoryIndex, FileIndex };

		QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
        QModelIndex index(const QString & path, IndexType type) const;
		QModelIndex parent(const QModelIndex & index) const;
		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;
		QVariant data(const QModelIndex & index, int role) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	public slots:
        void clear();
        void filesUpdated(const QString & hash, const QMap<QString, File> & files);
        void setTorrentHash(const QString & hash);
        void setupInterfaceConnections(Interface * interface);
        void torrentUpdated(const Torrent & torrent);
    protected:
        QStringList children(const QString & path, IndexType type) const;
        QString findPath(const QModelIndex & index = QModelIndex()) const;
        QString findParentPath(const QString & path = QString()) const;

        ByteSize size(const QString & path, IndexType type) const;
        ByteSize bytesComplete(const QString & path, IndexType type) const;
	private:
        QStringList findDirs(const QStringList & filePaths);

		QStringList headers;
        QMap<QString, File> m_files;
        ByteSize m_chunkSize;
        QStringList m_dirs;
        QString m_hash;
};

#endif
