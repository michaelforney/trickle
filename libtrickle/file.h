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
#ifndef FILE_H
#define FILE_H

/**
    @author Michael Forney <mforney@trickleproject.org>
*/

#include "trickle_export.h"

#include "bytesize.h"

#include <QStringList>

class TRICKLE_EXPORT File
{
    public:        
        enum FilePriority { Off, Low, Medium, High };

        File(const QString & path, const ByteSize & size, int index, int completedChunks, FilePriority priority);
        File();
        ~File();
        
        
        inline ByteSize size() const { return m_size; }
        inline int index() const { return m_index; }
        inline int completedChunks() const { return m_completedChunks; }
        inline FilePriority priority() const { return m_priority; }
        inline QString path() const { return m_path; }

        inline void setCompletedChunks(int completedChunks) { m_completedChunks = completedChunks; }
        inline void setSize(const ByteSize & size) { m_size = size; }
        inline void setIndex(int index) { m_index = index; }
        inline void setPriority(FilePriority priority) { m_priority = priority; }
        inline void setPath(const QString & path) { m_path = path; }
    private:
        
        FilePriority m_priority;
        int m_completedChunks;
        ByteSize m_size;
        int m_index;
        QString m_path;
};

#endif
