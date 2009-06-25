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
#include "configdialog.h"

#include "generalsettings.h"
#include "serversettings.h"

ConfigDialog::ConfigDialog(QWidget * parent, const QString & name, KConfigSkeleton * config)
 : KConfigDialog(parent, name, config)
{
	general = new GeneralSettings();
	server = new ServerSettings();	
	addPage(general, i18n("General"), "trickle");
	addPage(server, i18n("Servers"), "network-server");
	connect(server, SIGNAL(widgetModified()), this, SLOT(updateButtons()));
	connect(general, SIGNAL(widgetModified()), this, SLOT(updateButtons()));
	//connect(this, SIGNAL(widgetModified()), general, SLOT(updateServerLists()));
	resize(600, 400);
}

ConfigDialog::~ConfigDialog()
{
}

bool ConfigDialog::hasChanged()
{
	return server->hasChanged() || general->hasChanged();
}

void ConfigDialog::updateWidgets()
{
	server->updateWidgets();
}

void ConfigDialog::updateWidgetsDefault()
{
}

void ConfigDialog::updateSettings()
{
	if (hasChanged())
	{
		server->updateSettings();
		general->updateSettings();
	}
}

void ConfigDialog::updateButtons()
{
	static bool only_once = false;
	if (only_once) return;
	only_once = true;
	
	bool has_changed = hasChanged();
	
	enableButton(KDialog::Apply, has_changed);
	
	emit widgetModified();
	only_once = false;
}

#include "configdialog.moc"
