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
#include "serveritem.h"

#include "server.h"
#include "interfacemanager.h"

ServerItem::ServerItem(QTreeWidget * parent, const Server & server)
 : QTreeWidgetItem(parent)
{
	setServer(server);
}

ServerItem::~ServerItem()
{
}

QVariant ServerItem::data(int column, int role) const
{
	switch(role)
	{
		case Qt::DisplayRole:
		{
			switch(column)
			{
				case Name:
					return m_server.name();
					break;
				case Host:
					return m_server.host();
					break;
				case Port:
					return m_server.port();
					break;
				case Type:
					return InterfaceManager::self()->title(m_server.type());
					break;
				default:
					return QVariant();
			}
			break;
		}
		default:
		{
			return QVariant();
		}
	}
}

void ServerItem::setData(int column, int role, const QVariant & value)
{
	switch(role)
	{
		case Qt::DisplayRole:
		{
			switch(column)
			{
				case Name:
					m_server.setName(value.toString());
					break;
				case Host:
					m_server.setHost(value.toString());
					break;
				case Port:
					m_server.setPort(value.toInt());
					break;
				case Type:
					m_server.setType(value.toString());
					break;
			}
		}
	}
}
