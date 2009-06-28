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
#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>

/**
	@author Michael Forney <mforney@trickleproject.org>
*/

#include "trickle_export.h"
#include "server.h"

#include "file.h"
#include "torrent.h"
#include "peer.h"
#include "tracker.h"

#include <QSet>

class InterfaceConfigWidget;
class InterfaceConfig;
class Server;
class ByteSize;

class QTimer;
class QAuthenticator;

class TRICKLE_EXPORT Interface : public QObject
{
	Q_OBJECT
	public:
		Interface(InterfaceConfig * config, QObject * parent = 0);
        Interface(QObject * parent = 0);
		virtual ~Interface();
		virtual QString title() const = 0;
		virtual QString description() const = 0;
		
		//enum UpdateType { TorrentList, TorrentFiles, Trackers };
	public slots:
		virtual void setServer(const Server & server);
		
		virtual void setDownloadLimit(const ByteSize & size) = 0;
		virtual void setUploadLimit(const ByteSize & size) = 0;

		virtual void startTorrent(const QString & hash) = 0;
		virtual void stopTorrent(const QString & hash) = 0;

    
		virtual void setInterval(int interval);
		virtual bool start();
		virtual bool stop();
		virtual void reset();

        virtual void watchTorrent(const QString & hash);
        virtual void stopWatchingTorrent(const QString & hash);
        virtual void updateWatchedTorrent(const QString & hash);
        virtual void updateWatchedTorrents();

        virtual void update();
		//virtual bool connectToServer() = 0;
		virtual void clear() = 0;
	protected slots:
		virtual void updateTorrentList() = 0;
        virtual void updateFileInfo(const QString & hash) = 0;
        virtual void updatePeerInfo(const QString & hash) = 0;
        virtual void updateTrackerInfo(const QString & hash) = 0;
        //virtual void updateChunkInfo(const QString & hash) = 0;

        virtual void setConfig(InterfaceConfig * config);
	signals:
		void torrentsUpdated(const QMap<QString, Torrent> & torrents);
        void filesUpdated(const QString & hash, const QMap<QString, File> & files);
        void peersUpdated(const QString & hash, const QSet<Peer> & peers);
        void trackersUpdated(const QString & hash, const QMap<int, Tracker> & trackers);
        void watchedTorrentUpdated(const Torrent & torrent);
        //void chunksUpdated(const QString & hash, QList<)
        
        void torrentPriorityChanged(const QString & hash, Torrent::TorrentState priority);
		void filePriorityChanged(const QString & hash, const QStringList & path, File::FilePriority priority);
		void uploadLimitChanged(const ByteSize & limit);
		void downloadLimitChanged(const ByteSize & limit);
		void torrentStarted(const QString & hash);
		void torrentStopped(const QString & hash);
		void cleared();
	protected:
		Server server() const;
        InterfaceConfig * genericConfig() const;
        bool configValid() const;
        QMap<QString, int> watchedTorrents() const;
	private:
		QTimer * m_timer;
        InterfaceConfig * m_config;
		Server m_server;
        QMap<QString, int> m_watchedTorrents;
};

#endif
