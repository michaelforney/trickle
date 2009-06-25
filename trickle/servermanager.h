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
#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>

/**
	@author Michael Forney <mforney@trickleproject.org>
*/

class Server;

class ServerManager : public QObject
{
	Q_OBJECT
	public:
		ServerManager();
		~ServerManager();
		
		static ServerManager * self();
		
		Server * registerServer(Server * server);
		Server * editServer(Server * server);
		Server * removeServer(Server * server);
		QList<Server *> servers() const;
		QStringList serverNames() const;
	public slots:
		void load();
		void save();
	private:
		QList<Server *> m_servers;
};



#endif
