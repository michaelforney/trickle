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
#include "bytesize.h"

ByteSize::ByteSize()
{
	setBytes(0);
}

ByteSize::ByteSize(qint64 bytes)
{
	setBytes(bytes);
}

ByteSize::ByteSize(qint64 units, qint64 unitSize)
{
	setBytes(units * unitSize);
}

ByteSize::ByteSize(qint64 units, ByteSize unitSize)
{
	setBytes(units * unitSize.bytes());
}

ByteSize::~ByteSize()
{
}


QString ByteSize::toString()
{
	int factor = 0;
	qint64 tempBytes = bytes() * 100;
	while (tempBytes > 102400)
	{
		factor++;
		tempBytes /= 1024;
	}
	double unitBytes = static_cast<double>(tempBytes) / 100;
	return QString("%1 %2").arg(QString::number(unitBytes, 'f', 2)).arg(unitString(factor));
}

QString ByteSize::unitString(int unit)
{
	switch (unit)
	{
		case B:
			return QString("B");
		case KB:
			return QString("KB");
		case MB:
			return QString("MB");
		case GB:
			return QString("GB");
		case TB:
			return QString("TB");
		default:
			return QString();
	}
}

ByteSize ByteSize::operator*(const ByteSize & other)
{
	return ByteSize(bytes() * other.bytes());
}

qreal ByteSize::operator/(const ByteSize & other) const
{
    return (qreal)(bytes()) / other.bytes();
}

ByteSize ByteSize::operator-(const ByteSize & other)
{
	return ByteSize(bytes() - other.bytes());
}

ByteSize ByteSize::operator+(const ByteSize & other)
{
	return ByteSize(bytes() + other.bytes());
}

bool ByteSize::operator<(const ByteSize & other)
{
	return bytes() < other.bytes();
}

bool ByteSize::operator>(const ByteSize & other)
{
	return bytes() > other.bytes();
}

void ByteSize::operator+=(const ByteSize & other)
{
	m_bytes += other.bytes();
}
