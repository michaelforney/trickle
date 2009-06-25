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
#include "server.h"

#include <QDebug>

Server::Server()
{
}

Server::Server(const QString & name, const QString & host, int port, const QString & type, const QByteArray & typeConfig)
{
	setName(name);
	setPort(port);
	setHost(host);
	setType(type);
	setTypeConfig(typeConfig);
}


Server::~Server()
{
}

bool Server::operator==(const Server & other) const
{
	bool equal = other.name() == name() && other.host() == host() && other.port() == port() && other.type() == type() && other.typeConfig() == typeConfig();
	/*if (equal)
	{
		qDebug() << "Server " << name() << " == " << other.name();
	}
	else
	{
		qDebug() << "Server " << name() << " != " << other.name();
	}*/
	return equal;
}

bool Server::operator<(const Server & other) const
{
	return name() < other.name();
}


