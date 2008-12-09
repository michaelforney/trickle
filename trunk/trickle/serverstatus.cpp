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
#include "serverstatus.h"

Q_GLOBAL_STATIC(ServerStatus, serverStatus);

#include <QSettings>
#include <QDebug>

#include "servermodel.h"
#include "updatetimer.h"
#include "xmlrpc.h"

ServerStatus::ServerStatus()
 : QObject()
{
	readSettings();
}

ServerStatus::~ServerStatus()
{
}

ServerStatus * ServerStatus::instance()
{
	return serverStatus();
}

ServerModel * ServerStatus::createModel()
{
	return new ServerModel(servers());
}

Server ServerStatus::currentServer()
{
	return instance()->m_servers[instance()->m_currentServer];
}

QList<Server> ServerStatus::servers() const
{
	return m_servers;
}

void ServerStatus::readSettings()
{
}

void ServerStatus::setCurrentServer(int currentServer)
{
	m_currentServer = currentServer;
	//qDebug() << m_servers[m_currentServer].interval();
	XmlRpc::instance()->setServer(currentServer);
	if (currentServer >= 0)
	{
		emit connected();
	}
	else
	{
		emit disconnected();
	}
	emit serverChanged(m_currentServer);
	emit serverChanged0(m_currentServer + 1);
}

void ServerStatus::setCurrentServer0(int currentServer)
{
	setCurrentServer(currentServer - 1);
}

Server ServerStatus::server(int index)
{
	if (index >= 0)
	{
		return m_servers[index];
	}
	else
	{
		return Server();
	}
}

#include "serverstatus.moc"


