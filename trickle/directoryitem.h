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
#ifndef DIRECTORYITEM_H
#define DIRECTORYITEM_H

#include "filemodelitem.h"

/**
	@author Michael Forney <mforney@trickleproject.org>
*/
class DirectoryItem : public FileModelItem
{
	public:
		DirectoryItem(FileModelItem * parent = 0);
		~DirectoryItem();
		
		void addChild(FileModelItem * child);
		void removeChild(FileModelItem * child);
		
		inline int childCount() const { return children.size(); };
		inline FileModelItem * child(int row) { return children[row]; };
		inline FileModelItem * child(const QString & name) { foreach(FileModelItem * item, children) { if (item->name() == name) return item; } return 0; };
		inline int childRow(FileModelItem * child) { return children.indexOf(child); };
		
		ByteSize size() const;
		ByteSize downloaded() const;
		int priority() const;
		void changePriority(int priority);
		int complete() const;
		
		FileModelItem * item(const QString & path) const;
		QStringList paths(FileModelItem * root) const;
		DirectoryItem * directory(QStringList folders);
		
		ItemType type() const;
	private:
		QList<FileModelItem *> children;
};

#endif
