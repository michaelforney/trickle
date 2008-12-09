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
#include "generalsettings.h"

#include "interfacemanager.h"
#include "server.h"
#include "settings.h"

#include <QDebug>

GeneralSettings::GeneralSettings(QWidget * parent)
 : QWidget(parent)
{
	ui.setupUi(this);
	updateServerLists();
	
	connect(ui.defaultServer, SIGNAL(currentIndexChanged(const QString &)), this, SIGNAL(widgetModified()));
}

GeneralSettings::~GeneralSettings()
{
}

void GeneralSettings::updateSettings()
{
	if (hasChanged())
	{
		Settings::self()->findItem("defaultServer")->setProperty(ui.defaultServer->currentText());
		Settings::self()->writeConfig();
	}
}

void GeneralSettings::updateServerLists()
{
	kdDebug() << "GeneralSettings::updateServerLists()";
	ui.server->clear();
	ui.defaultServer->clear();
	QString currentServer = InterfaceManager::self()->server().name(); /* FIXME use interface manager */
	foreach(Server server, Settings::self()->serverList())
	{
		ui.server->addItem(server.name());
		ui.defaultServer->addItem(server.name());
		if (server.name() == currentServer)
		{
			ui.server->setCurrentIndex(ui.server->count() - 1);
		}
		if (server.name() == Settings::self()->defaultServer())
		{
			ui.defaultServer->setCurrentIndex(ui.defaultServer->count() - 1);
		}
	}
}

bool GeneralSettings::hasChanged() const
{
	return Settings::self()->defaultServer() != ui.defaultServer->currentText();
}

#include "generalsettings.moc"
