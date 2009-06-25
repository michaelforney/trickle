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
#include "bcodec.h"

#include <QVariant>
#include <KDebug>
#include <QBuffer>

uint qHash(const QVariant & variant)
{
	switch(variant.type())
	{
		case QVariant::String:
			return qHash(variant.toString());
		case QVariant::Int:
			return qHash(variant.toInt());
		case QVariant::List:
		{
			uint hash = 0;
			foreach(QVariant item, variant.toList())
			{
				hash += qHash(item);
			}
			return hash;
		}
		case QVariant::Map:
		{
			uint hash = 0;
			QVariantMap map = variant.toMap();
			foreach(QString key, map.keys())
			{
				hash += qHash(key) | qHash(map.value(key));
			}
			return hash;
		}
		case QVariant::UserType:
		{
			if (variant.userType() == qMetaTypeId<VariantHash>())
			{
				uint hash = 0;
				VariantHash map = variant.value<VariantHash>();
				foreach(QVariant key, map.keys())
				{
					hash += qHash(key) | qHash(map.value(key));
				}
				return hash;
			}
		}
		default:
		{
			kdDebug() << "Unknown type " << variant.type();
			return qHash(variant.type());
		}
	}
}

QVariant BCodec::decode(const QByteArray & data)
{
	QBuffer * buffer = new QBuffer();
	buffer->setData(data);
	buffer->open(QIODevice::ReadOnly);
	return decodeHelper(buffer);
}

QVariant BCodec::decodeHelper(QBuffer * data)
{
	char c;
	if (!data->getChar(&c))
	{
		return QVariant();
	}
	if (c == 'i')
	{
		//kdDebug() << "Integer";
		QByteArray numberString;
		data->getChar(&c);
		while (c != 'e' && !data->atEnd())
		{
			numberString += c;
			data->getChar(&c);
		}
		bool ok;
		int number = numberString.toInt(&ok);
		if (ok)
		{
			return number;
		}
		else
		{
			return QVariant();
		}
	}
	else if (c < '9' && c >= '0')
	{
		//kdDebug() << "String";
		QByteArray lengthString;
		while (c != ':' && !data->atEnd())
		{
			lengthString += c;
			data->getChar(&c);
		}
		int length = lengthString.toInt();
		return QString(data->read(length));
	}
	else if (c == 'l')
	{
		//kdDebug() << "List";
		QVariantList list;
		data->peek(&c, 1);
		while (c != 'e' && !data->atEnd())
		{
			list.append(decodeHelper(data));
			data->peek(&c, 1);
		}
	}
	else if (c == 'd')
	{
		//kdDebug() << "Dictionary";
		/*QVariantMap map;
		data->peek(&c, 1);
		while (c != 'e' && !data->atEnd())
		{
			QVariant key = decodeHelper(data);
			QVariant value = decodeHelper(data);
			if (key.type() != QVariant::String)
			{
				key = QString(encode(key));
				kdDebug() << QString("Currently only strings are supported as keys in dictionaries. Setting the key to '%1'").arg(key.toString());
			}
			map.insert(key.toString(), value);
			data->peek(&c, 1);
		}
		return map;*/
		VariantHash hash;
		data->peek(&c, 1);
		while (c != 'e' && !data->atEnd())
		{
			QVariant key = decodeHelper(data);
			QVariant value = decodeHelper(data);
			hash.insert(key, value);
			data->peek(&c, 1);
		}
		return qVariantFromValue(hash);
	}
	return QVariant();
}

QByteArray BCodec::encode(const QVariant & data)
{
	switch(data.type())
	{
		case QVariant::String:
		{
			QString string = data.toString();
			return QString("%1:%2").arg(string.size()).arg(string).toAscii();
		}
		case QVariant::Int:
		{
			return QString("i%1e").arg(data.toInt()).toAscii();
		}
		case QVariant::List:
		{
			QByteArray output("l");
			foreach(QVariant value, data.toList())
			{
				output.append(encode(value));
			}
			output.append("e");
			return output;
		}
		case QVariant::Map:
		{
			QByteArray output("d");
			QVariantMap map = data.toMap();
			foreach(QString key, map.keys())
			{
				output.append(encode(key));
				output.append(encode(map.value(key)));
			}
			output.append("e");
			return output;
		}
		case QVariant::UserType:
		{
			if (data.userType() == qMetaTypeId<VariantHash>())
			{
				QByteArray output("d");
				VariantHash hash = data.value<VariantHash>();
				foreach(QVariant key, hash.keys())
				{
					output.append(encode(key));
					output.append(encode(hash.value(key)));
				}
				output.append("e");
				return output;
			}
			else
			{
				kdDebug() << "Unknown type: \"" << data.typeName() << "\", userType(): " << data.userType();
				return QByteArray();
			}
		}
		default:
		{
			kdDebug() << "Unknown type: " << data.type();
			return QByteArray();
		}
	}
}
