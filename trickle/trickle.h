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
#ifndef TRICKLE_H
#define TRICKLE_H

#include <KXmlGuiWindow>

/**
	@author Michael Forney <mforney@trickleproject.org>
*/

#include <QVariant>
#include <QModelIndex>

class QMenuBar;
class QToolBar;
class QMenu;
class QAction;
class QLineEdit;
class QSpinBox;
class QLabel;
class KSystemTrayIcon;
class QAuthenticator;
class QCloseEvent;
class QComboBox;

class TorrentModel;
class TorrentSortModel;
class TorrentView;
class TorrentWidget;
class TorrentInfo;
class Torrent;
class Log;
class FileInfo;
class FileModel;
class TrackerInfo;
class Server;
class ServerModel;
class GeneralInfo;
class ServerInfo;
class Interface;
class ServerSelector;

class Trickle : public KXmlGuiWindow
{
	Q_OBJECT
	public:
		Trickle();
		~Trickle();

		void setupActions();
		void createStatusBar();
		void createDockWidgets();
	public slots:
        void newTorrent();
		void openTorrent();

		void optionsPreferences();

		void stopTorrent();
		void startTorrent();
		void setDownloadLimit(int limit);
        void setUploadLimit(int limit);

        bool queryClose();

		void setupInterfaceConnections(Interface * interface);
	private:
		TorrentView * torrentView;
		TorrentSortModel * torrentSortModel;
		TorrentModel * torrentModel;
		TorrentWidget * torrentWidget;
		Log * log;
		TorrentInfo * torrentInfo;
		FileInfo * fileInfo;
		TrackerInfo * trackerInfo;
		ServerInfo * serverInfo;
        ServerModel * serverModel;
        ServerSelector * serverSelector;

		QDockWidget * settingsDock;

		QLabel * totalUploadRate;
		QLabel * totalDownloadRate;

		KSystemTrayIcon * trayIcon;

		QToolBar * mainToolBar;
		QToolBar * serverBar;
		QToolBar * controlBar;

		QString host;
		int port;
		QString path;
};

#endif
