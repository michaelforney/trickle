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
#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractItemModel>

/**
	@author Michael Forney <michael@obberon.com>
*/

#include <QStringList>

class FileModelItem;
class TorrentItem;
class FileItem;

class FileModel : public QAbstractItemModel
{
	Q_OBJECT
	public:
		FileModel();
		~FileModel();
		
		enum Column { Name, Size, Complete, Priority };
		
		QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex & index) const;
		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;
		QVariant data(const QModelIndex & index, int role) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		Qt::ItemFlags flags(const QModelIndex & index) const;
		bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
		
		FileModelItem * findItem(const QString & path) const;
	public slots:
		void update();
		void newUpdate();
		void result(const QString & method, const QVariant & result);
	private:
		void updateItem(FileItem * item, const QVariantList & data, int index);

		QStringList headers;
		FileModelItem * mainItem;
};

#endif
