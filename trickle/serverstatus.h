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
#ifndef SERVERSTATUS_H
#define SERVERSTATUS_H

#include <QObject>

/**
	@author Michael Forney <michael@obberon.com>
*/

#include <QList>

#include "server.h"

class ServerModel;

class ServerStatus : public QObject
{
	Q_OBJECT
	public:
		ServerStatus();
		~ServerStatus();
		
		static ServerStatus * instance();
		ServerModel * createModel();
		static Server currentServer();
		QList<Server> servers() const;
		void readSettings();
		void writeSettings();
		Server server(int index);
	public slots:
		void setCurrentServer(int currentServer);
		void setCurrentServer0(int currentServer);
	private:
		QList<Server> m_servers;
		int m_currentServer;
	signals:
		void serverChanged(int serverIndex);
		void serverChanged0(int serverIndex);
		void disconnected();
		void connected();
};

#endif
