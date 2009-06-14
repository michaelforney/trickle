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
#ifndef BYTESIZE_H
#define BYTESIZE_H

/**
	@author Michael Forney <michael@obberon.com>
*/

#include "trickle_export.h"

#include <QString>

class TRICKLE_EXPORT ByteSize
{
	public:
		ByteSize();
		ByteSize(qint64 bytes);
		ByteSize(qint64 units, qint64 unitSize);
		ByteSize(qint64 units, ByteSize unitSize);
		~ByteSize();
		
		enum Units { B, KB, MB, GB, TB };
		
		QString toString();
		QString unitString(int unit);
		
		inline void setBytes(qint64 bytes) { m_bytes = bytes; };
		inline qint64 bytes() const { return m_bytes; };
		
		ByteSize operator*(const ByteSize & other);
        qreal operator/(const ByteSize & other) const;
		ByteSize operator-(const ByteSize & other);
		ByteSize operator+(const ByteSize & other);
		void operator+=(const ByteSize & other);
		bool operator<(const ByteSize & other);
		bool operator>(const ByteSize & other);
	private:
		qint64 m_bytes;
};

#endif
