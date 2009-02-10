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
#include "serverinfo.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

#include "serverstatus.h"
#include "servermodel.h"
#include "updatetimer.h"

ServerInfo::ServerInfo()
 : QWidget()
{
	QVBoxLayout * mainLayout = new QVBoxLayout(this);
	{
		QHBoxLayout * serverNameLayout = new QHBoxLayout();
		{
			serverName = new QLabel("<b>None</b>");
			//serverName->setFrameStyle(QFrame::Box);
			
			serverNameLayout->addWidget(new QLabel("Current Server: "));
			serverNameLayout->addWidget(serverName);
			serverNameLayout->addStretch();
		}
		
		QHBoxLayout * statusLayout = new QHBoxLayout();
		{
			//changeStatusButton = new QPushButton("Start");
			serverUpdateStatus = new QLabel("Stopped");
			//serverUpdateStatus->setFrameStyle(QFrame::Box);
			
			statusLayout->addWidget(new QLabel("Update Status: "));
			statusLayout->addWidget(serverUpdateStatus);
			//statusLayout->addWidget(changeStatusButton);
			statusLayout->addStretch();
		}
		
		mainLayout->addLayout(serverNameLayout);
		mainLayout->addLayout(statusLayout);
		mainLayout->addStretch();
	}
	
	//connect(ServerStatus::instance(), SIGNAL(serverChanged(int)), this, SLOT(setServer(int)));
}

ServerInfo::~ServerInfo()
{
}

void ServerInfo::setServer(int index)
{
	if (index < 0)
	{
		serverName->setText(QString("<b>None</b>"));
		serverUpdateStatus->setText("<b>Stopped</b>");
	}
	
	else
	{
		//Server server = ServerStatus::instance()->server(index);
		//serverName->setText(QString("<b>%1</b>").arg(server.name()));
		serverUpdateStatus->setText(UpdateTimer::instance()->isActive() ? "<b>Started</b>" : "<b>Stopped</b>");
	}
}

#include "serverinfo.moc"
