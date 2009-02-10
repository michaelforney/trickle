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
#ifndef UTORRENTINTERFACE_H
#define UTORRENTINTERFACE_H

#include "interface.h"

/**
	@author Michael Forney <michael@obberon.com>
*/

#include "webuirequest.h"

#include <QVariantList>

class ByteSize;
class QHttp;

class uTorrentInterface : public Interface
{
	Q_OBJECT
	public:
		uTorrentInterface(QObject * parent, const QVariantList & args);
		~uTorrentInterface();
		InterfaceConfigWidget * configWidget();
		QString title() const { return "uTorrent WebUI Interface"; }
		QString description() const { return "Interface to connect to uTorrent through the WebUI"; }
	public slots:
		void setServer(const Server & server);
		
		void setDownloadLimit(const ByteSize & size);
		void setUploadLimit(const ByteSize & size);
		void startTorrent(const QString & hash);
		void stopTorrent(const QString & hash);
		void updateTorrentList();
		void updateFileList(const QString & hash);
		void update();
		bool connectToServer();
		void clear();
	private slots:
		void requestFinished(int id, bool error);
		void authenticationRequired(QString hostname, quint16 port, QAuthenticator * authenticator);
	private:
		QVariantMap config;
		Server m_server;
		QHttp * http;
		QMap<int, WebUIRequest> requests;
};

#endif
