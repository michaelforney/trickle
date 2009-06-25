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
#include "csvcodec.h"

#include <QVariant>
#include <QBuffer>
#include <QByteArray>
#include <KDebug>
#include <QStringList>
#include <QVariantMap>

bool getNonWhiteChar(char * c, QIODevice * device)
{
	do
	{
		if (!device->getChar(c))
		{
			return false;
		}
	}
	while (QChar(*c).isSpace());
	return true;
}

QVariant CSVCodec::decode(const QByteArray & data)
{
	QBuffer buffer;
	buffer.setData(data);
	buffer.open(QIODevice::ReadOnly);
	return decodeHelper(&buffer);
}

QVariant CSVCodec::decodeHelper(QBuffer * data)
{
	char c;
	if (!getNonWhiteChar(&c, data))
	{
		return QVariant();
	}
	if (c == '"')
	{
		QString string;
		while (data->getChar(&c) && c != '"')
		{
			string.append(c);
		}
		return string;
	}
	else if ((c <= '9' && c >= '0') || c == '-')
	{
		QString number(c);
		while (data->getChar(&c) && c <= '9' && c >= '0')
		{
			number.append(c);
		}
		data->ungetChar(c);
		bool ok;
		qint64 num = number.toLongLong(&ok);
		if (ok)
		{
			return num;
		}
		else
		{
			kdDebug() << "Conversion failed: " << number;
			return QVariant();
		}
	}
	else if (c == '[')
	{
		QVariantList list;
		do
		{
			QVariant value = decodeHelper(data);
			if (value == QVariant())
			{
				return list;
			}
			list.append(value);
		}
		while (getNonWhiteChar(&c, data) && c == ',');
		return list;
	}
	else if (c == '{')
	{
		QVariantMap map;
		do
		{
			QVariant key = decodeHelper(data);
			if (key.type() != QVariant::String || !getNonWhiteChar(&c, data) || c != ':')
			{
				kdDebug() << "Something went wrong";
				return QVariant();
			}
			map.insert(key.toString(), decodeHelper(data));
		}
		while (getNonWhiteChar(&c, data) && c == ',');
		return map;
	}
	return QVariant();
}

QByteArray CSVCodec::encode(const QVariant & data)
{
	switch(data.type())
	{
		case QVariant::String:
			return QString("\"%1\"").arg(data.toString()).toAscii();
		case QVariant::Int: case QVariant::LongLong:
			return QByteArray::number(data.toLongLong());
		case QVariant::List:
		{
			QStringList output;
			foreach(QVariant value, data.toList())
			{
				output.append(encode(value));
			}
			return QString("[%1]").arg(output.join(",")).toAscii();
		}
		case QVariant::Map:
		{
			QVariantMap map = data.toMap();
			QStringList output;
			foreach(QString key, map.keys())
			{
				output.append(QString("\"%1\":%2").arg(key).arg(QString(encode(map.value(key)))));
			}
			return QString("{%1}").arg(output.join(",")).toAscii();
		}
		default:
			return QByteArray();
	}
}
