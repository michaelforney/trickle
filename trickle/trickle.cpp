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
#include "trickle.h"

#include <KMenuBar>
#include <KStandardAction>
#include <KPasswordDialog>
#include <KToggleAction>
#include <KConfigDialog>
#include <KToolBar>
#include <KActionCollection>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <KSystemTrayIcon>
#include <QDebug>
#include <QAuthenticator>
#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QHeaderView>

#include "torrentmodel.h"
#include "torrentview.h"
#include "torrentwidget.h"
#include "torrentdelegate.h"
#include "torrentinfo.h"
#include "log.h"
#include "torrentsortmodel.h"
#include "fileinfo.h"
#include "filemodel.h"
#include "trackerinfo.h"
#include "server.h"
#include "servermodel.h"
#include "generalinfo.h"
#include "serverinfo.h"
#include "settings.h"
#include "serversettings.h"
#include "generalsettings.h"
#include "servermanager.h"
#include "interfacemanager.h"
#include "configdialog.h"
#include "interface.h"

Trickle::Trickle()
 : KXmlGuiWindow()
{
	torrentModel = new TorrentModel();
	torrentSortModel = new TorrentSortModel();
	{
		torrentSortModel->setSourceModel(torrentModel);
	}
	torrentView = new TorrentView();
	{
		TorrentDelegate * priorityDelegate = new TorrentDelegate();
		
		torrentView->setItemDelegateForColumn(TorrentModel::Priority, priorityDelegate);
		torrentView->setModel(torrentSortModel);
		torrentView->setColumnHidden(TorrentModel::Hash, true);
	}
	trayIcon = new KSystemTrayIcon("trickle", this);
	{
		trayIcon->show();
	}
	setCentralWidget(torrentView);
	
	setupActions();
	createStatusBar();
	createDockWidgets();
	setupGUI();
	//connect(torrentModel, SIGNAL(logInfo(QString)), log, SLOT(logInfo(QString)));
	connect(InterfaceManager::self(), SIGNAL(interfaceChanged(Interface *)), this, SLOT(setupInterfaceConnections(Interface *)));
    connect(torrentView, SIGNAL(indexChanged(int)), this, SLOT(torrentIndexChanged(int)));
	
	InterfaceManager::self()->load();
    if (InterfaceManager::interface())
    {
        qDebug() << InterfaceManager::interface()->title();
        InterfaceManager::interface()->start();
    }
}


Trickle::~Trickle()
{
}

void Trickle::setupActions()
{
	KStandardAction::openNew(this, SLOT(newTorrent()), actionCollection());
	KStandardAction::open(this, SLOT(openTorrent()), actionCollection());
	KStandardAction::quit(qApp, SLOT(closeAllWindows()), actionCollection());
	
	KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
	
	KToggleAction * showTorrentInfo = new KToggleAction("&Show Torrent Info", actionCollection());
	showTorrentInfo->setObjectName("view_torrentinfo");
	
	KAction * startAction = new KAction(KIcon("media-playback-start"), "&Start", actionCollection());
	startAction->setObjectName("torrent_start");
	
	KAction * stopAction = new KAction(KIcon("media-playback-stop"), "&Stop", actionCollection());
	stopAction->setObjectName("torrent_stop");
	
	KAction * removeAction = new KAction(KIcon("list-remove"), "&Remove", actionCollection());
	removeAction->setObjectName("torrent_remove");
	
	actionCollection()->addAction(showTorrentInfo->objectName(), showTorrentInfo);
	actionCollection()->addAction(startAction->objectName(), startAction);
	actionCollection()->addAction(stopAction->objectName(), stopAction);
	actionCollection()->addAction(removeAction->objectName(), removeAction);
	connect(startAction, SIGNAL(triggered()), this, SLOT(startTorrent()));
	connect(stopAction, SIGNAL(triggered()), this, SLOT(stopTorrent()));
	connect(removeAction, SIGNAL(triggered()), this, SLOT(removeTorrent()));
}

void Trickle::createStatusBar()
{
	/*totalDownloadRate = new QLabel(statusBar());
	totalUploadRate = new QLabel(statusBar());
	totalDownloadRate->setMinimumWidth(150);
	totalUploadRate->setMinimumWidth(150);
	serverList = new QComboBox(statusBar());
	{
		serverList->setModel(serverModel);
		connect(serverList, SIGNAL(currentIndexChanged(int)), ServerStatus::instance(), SLOT(setCurrentServer0(int)));
		connect(ServerStatus::instance(), SIGNAL(serverChanged0(int)), serverList, SLOT(setCurrentIndex(int)));
	}
	
	statusBar()->showMessage("Ready");
	statusBar()->addPermanentWidget(serverList);
	statusBar()->addPermanentWidget(totalDownloadRate);
	statusBar()->addPermanentWidget(totalUploadRate);*/
}

void Trickle::createDockWidgets()
{
	
	QDockWidget * statsDock = new QDockWidget("Info", this);
	{
		statsDock->setObjectName("Info");
		statsDock->setAllowedAreas(Qt::BottomDockWidgetArea);
		torrentWidget = new TorrentWidget();
		{
			serverInfo = new ServerInfo();
			generalInfo = new GeneralInfo();
			torrentInfo = new TorrentInfo();
			fileInfo = new FileInfo();
			trackerInfo = new TrackerInfo();
			log = new Log();
			
			torrentWidget->addTab(serverInfo, "Server");
			torrentWidget->addTab(generalInfo, "General");
			torrentWidget->addTab(torrentInfo, "Torrent Information");
			torrentWidget->addTab(fileInfo, "Files");
			torrentWidget->addTab(trackerInfo, "Tracker");
			torrentWidget->addTab(log, "Log");
		}
		statsDock->setWidget(torrentWidget);
	}
	addDockWidget(Qt::BottomDockWidgetArea, statsDock);
}

void Trickle::stopTorrent()
{
	QModelIndexList selected = torrentView->selectionModel()->selectedIndexes();
	if (selected.size() > 0)
	{
		//Torrent * item = static_cast<TorrentItem *>(torrentSortModel->mapToSource(selected[0]).internalPointer());
		//item->stop();
	}
}

void Trickle::startTorrent()
{
	QModelIndexList selected = torrentView->selectionModel()->selectedIndexes();
	if (selected.size() > 0)
	{
		//TorrentItem * item = static_cast<TorrentItem *>(torrentSortModel->mapToSource(selected[0]).internalPointer());
		//item->start();
	}
}

void Trickle::torrentIndexChanged(int index)
{
    QString hash = torrentModel->hash(index);
    torrentInfo->setTorrentHash(hash);
    trackerInfo->setTorrentHash(hash);
}

void Trickle::setDownloadLimit(int limit)
{
}

void Trickle::setUploadLimit(int limit)
{
}

void Trickle::optionsPreferences()
{
	ConfigDialog * configDialog = new ConfigDialog(this, "settings", Settings::self());
	connect(configDialog, SIGNAL(settingsChanged(QString)), this, SLOT(settingsChanged()));
	configDialog->setAttribute(Qt::WA_DeleteOnClose);
	configDialog->exec();
}

void Trickle::newTorrent()
{
}

void Trickle::openTorrent()
{
}

void Trickle::setupInterfaceConnections(Interface * interface)
{
}

#include "trickle.moc"
