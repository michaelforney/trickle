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
#include "updatetimer.h"

Q_GLOBAL_STATIC(UpdateTimer, updateTimer);

#include <QTimer>
#include <QDebug>

#include "xmlrpc.h"
#include "serverstatus.h"

UpdateTimer::UpdateTimer()
 : QObject()
{
	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(possibleUpdate()));
	//connect(ServerStatus::instance(), SIGNAL(connected()), this, SLOT(start()));
	//connect(ServerStatus::instance(), SIGNAL(disconnected()), this, SLOT(stop()));
}


UpdateTimer::~UpdateTimer()
{
}

UpdateTimer * UpdateTimer::instance()
{
	return updateTimer();
}

void UpdateTimer::start()
{
	qDebug("start()");
	startTimer();
	emit timeout();
}

void UpdateTimer::startTimer()
{
	//timer->start(ServerStatus::instance()->currentServer().interval());
}

void UpdateTimer::stop()
{
	qDebug("stop()");
	timer->stop();
}

void UpdateTimer::possibleUpdate()
{
	/*if (XmlRpc::instance()->isAuthenticating())
	{
		return;
	}
	
	if (XmlRpc::instance()->isBusy())
	{
		qDebug() << QString("Busy with %1 requests").arg(XmlRpc::instance()->requests());
		timer->start(100);
		return;
	}
	
	if (timer->interval() == 100)
	{
		startTimer();
	}
	
	emit timeout();*/
}

bool UpdateTimer::isActive() const
{
	return timer->isActive();
}

#include "updatetimer.moc"
