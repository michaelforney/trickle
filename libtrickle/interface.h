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
#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>

/**
	@author Michael Forney <michael@obberon.com>
*/

#include "trickle_export.h"
#include "server.h"

#include "file.h"
#include "torrent.h"

class InterfaceConfigWidget;
class Server;
class ByteSize;

class QTimer;
class QAuthenticator;

class TRICKLE_EXPORT Interface : public QObject
{
	Q_OBJECT
	public:
		Interface(QObject * parent);
		virtual ~Interface();
		virtual QString title() const = 0;
		virtual QString description() const = 0;
		virtual InterfaceConfigWidget * configWidget() = 0;
		
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
		virtual void update() = 0;
		//virtual bool connectToServer() = 0;
		virtual void clear() = 0;
	protected slots:
		virtual void updateTorrentList() = 0;
		virtual void updateFileList(const QString & hash) = 0;
	signals:
		void torrentMapUpdated(const QMap<QString, Torrent> & torrents);
		void filesUpdated(const QString & hash, QList<File> & files);
		void torrentPriorityChanged(const QString & hash, Torrent::TorrentPriority priority);
		void filePriorityChanged(const QString & hash, const QStringList & path, File::FilePriority priority);
		void uploadLimitChanged(const ByteSize & limit);
		void downloadLimitChanged(const ByteSize & limit);
		void torrentStarted(const QString & hash);
		void torrentStopped(const QString & hash);
		void cleared();
		void authenticate(QAuthenticator * authenticator);
		//void torrentsAdded(QStringList hashes); //Need this?
		//void torrentsRemoved(QStringList hashes); //Need this?
		//void filesAdded(const QString & hash, QList<File *>); //Need this?
		//void filesRemoved(const QString & hash, QList<File *>); //Need this?
		//void updated(UpdateType type);
	protected:
		Server server() const;
	private:
		QTimer * m_timer;
		Server m_server;
};

#endif
