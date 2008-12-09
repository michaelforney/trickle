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
#include "filemodelitem.h"

#include "directoryitem.h"
#include "fileitem.h"

FileModelItem::FileModelItem(FileModelItem * parent)
 : QObject(parent)
{
	setParent(parent);
}

FileModelItem::FileModelItem(const QString & name, FileModelItem * parent)
 : QObject(parent)
{
	setName(name);
	setParent(parent);
}

FileModelItem::~FileModelItem()
{
	if (m_parent && m_parent->isDirectory())
	{
		m_parent->toDirectory()->removeChild(this);
	}
}


int FileModelItem::row()
{
	if (m_parent && m_parent->type() == Directory)
	{
		return m_parent->toDirectory()->childRow(this);
	}
	else
	{
		return 0;
	}
}

DirectoryItem * FileModelItem::toDirectory()
{
	if (isDirectory())
	{
		return static_cast<DirectoryItem *>(this);
	}
	return 0;
}

FileItem * FileModelItem::toFile()
{
	if (isFile())
	{
		return static_cast<FileItem *>(this);
	}
	return 0;
}

