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
#ifndef FILEITEM_H
#define FILEITEM_H

#include "filemodelitem.h"

/**
	@author Michael Forney <mforney@trickleproject.org>
*/

#include <QObject>

class FileItem : public FileModelItem
{
	Q_OBJECT
	public:
		FileItem(FileModelItem * parent = 0);
		~FileItem();
		
		
		inline int completedChunks() const { return m_completedChunks; };
		int complete() const;
		inline int priority() const { return m_priority; };
		inline void setCompletedChunks(int completedChunks) { m_completedChunks = completedChunks; };
		inline void setSize(ByteSize size) { m_size = size; };
		inline ByteSize size() const { return m_size; };
		ByteSize downloaded() const;
		inline void setIndex(int index) { m_index = index; };
		inline int index() const { return m_index; };
		inline void setPriority(int priority) { m_priority = priority; };
		void changePriority(int priority);
		QString path(FileModelItem * root) const;
		
		ItemType type() const;
	public slots:
		void result(const QString & method, const QVariant & result);
	private:
		int m_priority;
		int m_completedChunks;
		ByteSize m_size;
		int m_index;
};

#endif
