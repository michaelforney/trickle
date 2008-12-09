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
#ifndef FILEMODELITEM_H
#define FILEMODELITEM_H

/**
	@author Michael Forney <michael@obberon.com>
*/

#include <QList>
#include <QString>
#include <QObject>

#include "bytesize.h"

class DirectoryItem;
class FileItem;

class FileModelItem : public QObject
{
	public:
		FileModelItem(FileModelItem * parent = 0);
		FileModelItem(const QString & name, FileModelItem * parent = 0);
		virtual ~FileModelItem();
		
		enum ItemType { File, Directory };
		enum Priority { Skip, Normal, High };
		
		inline void setName(const QString & name) { m_name = name; };
		inline void setParent(FileModelItem * parent) { m_parent = parent; }
		
		inline bool isFile() { return type() == FileModelItem::File; };
		inline bool isDirectory() { return type() == FileModelItem::Directory; };
		
		DirectoryItem * toDirectory();
		FileItem * toFile();
		
		inline QString name() const { return m_name; };
		virtual ByteSize size() const = 0;
		virtual ByteSize downloaded() const = 0;
		virtual FileModelItem::ItemType type() const = 0;
		virtual void changePriority(int priority) = 0;
		virtual int priority() const = 0;
		virtual int complete() const = 0;
		
		int row();
		inline FileModelItem * parent() const { return m_parent; };
	private:
		FileModelItem * m_parent;
		QString m_name;
};

#endif
