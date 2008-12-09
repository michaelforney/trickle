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
#include "interface.h"

#include "server.h"

#include <QTimer>
#include <QStringList>

Interface::Interface(QObject * parent)
 : QObject(parent)
{
	m_timer = new QTimer();
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTorrentList()));
}

Interface::~Interface()
{
}

void Interface::setServer(const Server & server)
{
	reset();
	m_server = server;
}

void Interface::setInterval(int interval)
{
	m_timer->setInterval(interval);
}

bool Interface::start()
{
	if (connectToServer())
	{
		m_timer->start();
		return true;
	}
	else
	{
		return false;
	}
}

bool Interface::stop()
{
	m_timer->stop();
	return true;
}

void Interface::reset()
{
	stop();
	clear();
}

Server Interface::server() const
{
	return m_server;
}

#include "interface.moc"
