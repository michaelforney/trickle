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
#include "serversettings.h"

#include <QTreeView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QSettings>
#include <QMetaType>
#include <QModelIndex>
#include <QHeaderView>
#include <QCheckBox>
#include <QComboBox>
#include <KMessageBox>
#include <QDebug>

#include "server.h"
#include "servereditdialog.h"
#include "serveritem.h"
#include "settings.h"

ServerSettings::ServerSettings(QWidget * parent)
 : QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.addButton, SIGNAL(clicked()), this, SLOT(addServer()));
	connect(ui.editButton, SIGNAL(clicked()), this, SLOT(editServer()));
	connect(ui.removeButton, SIGNAL(clicked()), this, SLOT(removeServer()));
}

ServerSettings::~ServerSettings()
{
}

void ServerSettings::addServer()
{
	ServerEditDialog * dialog = new ServerEditDialog();
	if (dialog->exec() == QDialog::Accepted)
	{
		if (hasServerName(dialog->server().name()))
		{
			KMessageBox::error(this, QString("A server with the name <b>%1</b> already exists.").arg(dialog->server().name()), "Server name exists");
		}
		else
		{
			new ServerItem(ui.serverList, dialog->server());
			servers.append(dialog->server());
			emit widgetModified();
		}
	}
}

void ServerSettings::editServer()
{
	QList<QTreeWidgetItem *> selection = ui.serverList->selectedItems();
	if (selection.size() > 0)
	{
		ServerItem * item = static_cast<ServerItem *>(selection[0]);
		ServerEditDialog * dialog = new ServerEditDialog();
		dialog->setServer(item->server());
		if (dialog->exec() == QDialog::Accepted)
		{
			//ServerManager::self()->save();
			servers.replace(servers.indexOf(item->server()), dialog->server());
			item->setServer(dialog->server());
			emit widgetModified();
		}
	}
}

void ServerSettings::removeServer()
{
	QList<QTreeWidgetItem *> selection = ui.serverList->selectedItems();
	if (selection.size() > 0)
	{
		ServerItem * item = static_cast<ServerItem *>(selection[0]);
		if (KMessageBox::questionYesNo(this, QString("Are you sure you want to remove the server <b>%1</b>?").arg(item->text(0)), "Confirm Remove") == KMessageBox::Yes)
		{
			servers.removeAt(servers.indexOf(item->server()));
			ui.serverList->removeItemWidget(item, 0);
			delete(item);
			emit widgetModified();
		}
	}
}

void ServerSettings::updateWidgets()
{
	servers = Settings::self()->serverList();
	ui.serverList->clear();
	foreach(Server server, servers)
	{
		new ServerItem(ui.serverList, server);
	}
}

void ServerSettings::updateWidgetsDefault()
{
}

void ServerSettings::updateSettings()
{
	//qDebug("ServerSettings: updateSettings()");
	Settings::self()->setServers(servers);
	//kdDebug() << Settings::self()->serverNames();
	Settings::self()->writeConfig();
}

bool ServerSettings::hasChanged() const
{
	QStringList currentServers;
	QStringList oldServers = Settings::self()->serverNames();
	foreach(Server server, servers)
	{
		currentServers.append(server.name());
	}
	qSort(currentServers);
	qSort(oldServers);
	//qDebug() << "new list: " << currentServers;
	//qDebug() << "old list: " << oldServers;
	QList<Server> sortedList = servers;
	QList<Server> oldSortedList = Settings::self()->serverList();
	qSort(sortedList);
	qSort(oldSortedList);
	return !(sortedList == oldSortedList);
}

bool ServerSettings::hasServerName(const QString & name) const
{
	foreach(Server server, servers)
	{
		if (server.name() == name)
		{
			return true;
		}
	}
	return false;
}

#include "serversettings.moc"
