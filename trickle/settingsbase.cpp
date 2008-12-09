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
#include "settingsbase.h"

#include <KDebug>

SettingsBase::SettingsBase(KSharedConfig::Ptr config, QObject * parent)
 : KConfigSkeleton(config, parent)
{
}

SettingsBase::SettingsBase(const QString & configName, QObject * parent)
 : KConfigSkeleton(configName, parent)
{
}

SettingsBase::~SettingsBase()
{
}

void SettingsBase::usrReadConfig()
{
	kdDebug() << "SettingsBase: reading config";
	m_servers.clear();
	QStringList serverList = config()->groupList().filter(QRegExp("server_.*"));
	foreach(QString serverGroupName, serverList)
	{
		KConfigGroup serverGroup(config(), serverGroupName);
		QString serverName = serverGroup.readEntry("name", QString());
		QString serverHost = serverGroup.readEntry("host", QString());
		int serverPort = serverGroup.readEntry("port", 80);
		QString serverType = serverGroup.readEntry("type", QString());
		QByteArray serverTypeConfig = serverGroup.readEntry("typeConfig", QByteArray());
		m_servers.append(Server(serverName, serverHost, serverPort, serverType, serverTypeConfig));
	}
}

void SettingsBase::usrWriteConfig()
{
	kdDebug() << "SettingsBase: writing config";
	QStringList serverGroupNames = config()->groupList().filter(QRegExp("server_.*"));
	foreach(QString serverGroupName, serverGroupNames)
	{
		kdDebug() << "SettingsBase: deleting group " << serverGroupName;
		config()->deleteGroup(serverGroupName);
	}
	foreach(Server server, m_servers)
	{
		qDebug() << "SettingsBase: writing group " << QString("server_%1").arg(server.name());
		KConfigGroup serverGroup(config(), QString("server_%1").arg(server.name()));
		serverGroup.writeEntry("name", server.name());
		serverGroup.writeEntry("host", server.host());
		serverGroup.writeEntry("port", server.port());
		serverGroup.writeEntry("type", server.type());
		serverGroup.writeEntry("typeConfig", server.typeConfig());
	}
}

/*Server * SettingsBase::registerServer(Server * server)
{
	if (m_servers.contains(server))
	{
		delete(server);
		return 0;
	}
	
	foreach(Server * currentServer, m_servers)
	{
		if (server->name() == currentServer->name())
		{
			delete(server);
			return 0;
		}
	}
	m_servers.append(server);
	return server;
}

Server * SettingsBase::editServer(Server * server)
{
	for(int i = 0; i < m_servers.size(); i++)
	{
		if (server->name() == m_servers[i]->name())
		{
			m_servers.replace(i, server);
			return server;
		}
	}
	return 0;
}

Server * SettingsBase::removeServer(Server * server)
{
	for(int i = 0; i < m_servers.size(); i++)
	{
		if (server->name() == m_servers[i]->name())
		{
			m_servers.removeAt(i);
			return server;
		}
	}
	return 0;
}*/

QList<Server> SettingsBase::serverList() const
{
	return m_servers;
}

QStringList SettingsBase::serverNames() const
{
	QStringList names;
	foreach(Server server, m_servers)
	{
		names.append(server.name());
	}
	return names;
}

void SettingsBase::setServers(const QList<Server> & servers)
{
	m_servers = servers;
}

Server SettingsBase::server(const QString & name) const
{
	foreach(Server server, m_servers)
	{
		if (server.name() == name)
		{
			return server;
		}
	}
	return Server();
}
