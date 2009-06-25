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
#include "servermodel.h"

#include <QDebug>

#include "server.h"

ServerModel::ServerModel()
 : QAbstractItemModel()
{
	headers << "Name" << "Host" << "Port" << "Type";
	setStartupServer(-1);
	m_showNoServer = false;
}

ServerModel::ServerModel(QList<Server> servers)
{
	headers << "Name" << "Host" << "Path" << "Port";
	setStartupServer(-1);
	m_servers = servers;
	m_showNoServer = false;
}

ServerModel::~ServerModel()
{
}

int ServerModel::rowCount(const QModelIndex & parent) const
{
	if (parent.isValid())
	{
		return 0;
	}
	else
	{
		if (m_showNoServer)
		{
			return m_servers.size() + 1;
		}
		else
		{
			return m_servers.size();
		}
	}
}

int ServerModel::columnCount(const QModelIndex & /*parent*/) const
{
	return headers.size();
}

QVariant ServerModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}
	
	if (role == Qt::DisplayRole)
	{
		if (m_showNoServer && index.row() ==  0)
		{
			if (index.column() == Name)
			{
				return QString("None");
			}
			else
			{
				return QVariant();
			}
		}
		Server server = m_servers[m_showNoServer ? index.row() - 1 : index.row()];
		if (index.column() == Name)
		{
			return server.name();
		}
		else if (index.column() == Host)
		{
			return server.host();
		}
		else if (index.column() == Port)
		{
			return server.port();
		}
		else if (index.column() == Type)
		{
			return QVariant();
		}
		else
		{
			return QVariant();
		}
	}
	else
	{
		return QVariant();
	}
}

QVariant ServerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
	{
		if (role == Qt::DisplayRole)
		{
			return headers[section];
		}
		else
		{
			return QVariant();
		}
	}
	else
	{
		return QVariant();
	}
}

QModelIndex ServerModel::parent(const QModelIndex & /*index*/) const
{
	return QModelIndex();
}

QModelIndex ServerModel::index(int row, int column, const QModelIndex & parent) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}
	
	if (parent.isValid())
	{
		return QModelIndex();
	}
	
	return createIndex(row, column);
}

void ServerModel::addServer(Server server)
{
	beginInsertRows(QModelIndex(), m_servers.size(), m_servers.size());
	m_servers.append(server);
	endInsertRows();
}

QList<Server> ServerModel::servers()
{
	return m_servers;
}

void ServerModel::clear()
{
	emit layoutAboutToBeChanged();
	m_servers.clear();
	emit layoutChanged();
}

void ServerModel::removeServer(int row)
{
	beginRemoveRows(QModelIndex(), row, row);
	m_servers.removeAt(row);
	endRemoveRows();
}

Server ServerModel::server(int index) const
{
	if (m_servers.size() > index)
	{
		return m_servers[index];
	}
	else
	{
		return Server();
	}
}

void ServerModel::setServer(int pos, Server server)
{
	if (m_servers.size() > pos)
	{
		m_servers[pos] = server;
	}
	emit dataChanged(index(pos, 0), index(pos, headers.size() - 1));
}

#include "servermodel.moc"
