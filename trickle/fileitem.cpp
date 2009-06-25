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
#include "fileitem.h"

#include "selectedtorrent.h"
#include "torrentitem.h"

FileItem::FileItem(FileModelItem * parent)
 : FileModelItem(parent)
{
}


FileItem::~FileItem()
{
}


FileModelItem::ItemType FileItem::type() const
{
	return FileModelItem::File;
}

void FileItem::changePriority(int priority)
{
	//XmlRpc::instance()->call("f.set_priority", QVariantList() << SelectedTorrent::torrentInstance()->hash() << index() << m_priority, this, "result");
}

void FileItem::result(const QString & method, const QVariant & resultVariant)
{
	
}

int FileItem::complete() const
{
	int percent = SelectedTorrent::torrentInstance()->chunkSize().bytes() * completedChunks() * 100 / size().bytes();
	if (percent > 100)
	{
		return 100;
	}
	else
	{
		return percent;
	}
}


QString FileItem::path(FileModelItem * root) const
{
	const FileModelItem * item = this;
	QStringList folders;
	while (item != root)
	{
		folders.append(item->name());
		item = item->parent();
	}
	return folders.join("/");
}

ByteSize FileItem::downloaded() const
{
	ByteSize downloadedSize = SelectedTorrent::torrentInstance()->chunkSize().bytes() * completedChunks();
	if (downloadedSize > size())
	{
		return size();
	}
	else
	{
		return downloadedSize;
	}
}

#include "fileitem.moc"
