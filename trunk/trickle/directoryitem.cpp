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
#include "directoryitem.h"

#include <QStringList>
#include <QDebug>

#include "fileitem.h"
#include "selectedtorrent.h"
#include "torrentitem.h"

DirectoryItem::DirectoryItem(FileModelItem * parent)
 : FileModelItem(parent)
{
}

DirectoryItem::~DirectoryItem()
{
	foreach(FileModelItem * child, children)
	{
		delete child;
	}
}


void DirectoryItem::addChild(FileModelItem * child)
{
	if (child)
	{
		child->setParent(this);
		children.append(child);
	}
}

FileModelItem::ItemType DirectoryItem::type() const
{
	return FileModelItem::Directory;
}

FileModelItem * DirectoryItem::item(const QString & path) const
{
	QStringList folders = path.split("/");
	QString name = folders.last();

	foreach(FileModelItem * child, children)
	{
		if (child->name() == name)
		{
			if (folders.size() == 1 && child->isFile())
			{
				return child;
			}
			else if (folders.size() > 1 && child->isDirectory())
			{
				return child->toDirectory()->item(folders.join("/"));
			}
		}
	}
	
	return 0;
}

ByteSize DirectoryItem::size() const
{
	ByteSize totalSize;
	foreach(FileModelItem * child, children)
	{
		totalSize += child->size();
	}
	return totalSize;
}

int DirectoryItem::priority() const
{
	if (children.size() > 0)
	{
		int testPriority = children[0]->priority();
		foreach(FileModelItem * child, children)
		{
			if (testPriority != child->priority())
			{
				return Skip;
			}
		}
		return testPriority;
	}
	return 0;
}

void DirectoryItem::changePriority(int priority)
{
	foreach(FileModelItem * child, children)
	{
		child->changePriority(priority);
	}
}

void DirectoryItem::removeChild(FileModelItem * child)
{
	children.removeAll(child);
	if (children.size() == 0)
	{
		delete this;
	}
}

DirectoryItem * DirectoryItem::directory(QStringList folders)
{
	if (folders.size() == 0)
	{
		return this;
	}
	
	DirectoryItem * nextDirectory = 0;
	foreach(FileModelItem * child, children)
	{
		if (child->isDirectory() && child->name() == folders[0])
		{
			nextDirectory = child->toDirectory();
			break;
		}
	}
	
	if (!nextDirectory)
	{
		nextDirectory = new DirectoryItem();
		nextDirectory->setName(folders[0]);
		addChild(nextDirectory);
	}
	
	folders.removeFirst();
	
	return nextDirectory->directory(folders);
}

int DirectoryItem::complete() const
{
	return downloaded().bytes() * 100 / size().bytes();
}

ByteSize DirectoryItem::downloaded() const
{
	ByteSize downloadedSize = 0;
	foreach(FileModelItem * child, children)
	{
		downloadedSize += child->downloaded();
	}
	return downloadedSize;
}
