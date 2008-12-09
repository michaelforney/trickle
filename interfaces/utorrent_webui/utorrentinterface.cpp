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
#include "utorrentinterface.h"

#include "utorrentconfig.h"
#include "xmlrpc.h"
#include "bytesize.h"

#include <KGenericFactory>

K_PLUGIN_FACTORY(uTorrentInterfaceFactory, registerPlugin<uTorrentInterface>();)
K_EXPORT_PLUGIN(uTorrentInterfaceFactory("trickle_utorrent"))

uTorrentInterface::uTorrentInterface(QObject * parent, const QVariantList & /*args*/)
 : Interface(parent), http(0)
{
}

uTorrentInterface::~uTorrentInterface()
{
}

InterfaceConfigWidget * uTorrentInterface::configWidget()
{
	return new uTorrentConfig();
}

void uTorrentInterface::setServer(const Server & server)
{
	m_server = server;
}
		
void uTorrentInterface::setDownloadLimit(const ByteSize & size)
{
}

void uTorrentInterface::setUploadLimit(const ByteSize & size)
{
}

void uTorrentInterface::startTorrent(const QString & hash)
{
}

void uTorrentInterface::stopTorrent(const QString & hash)
{
}

void uTorrentInterface::update()
{
}

bool uTorrentInterface::connectToServer()
{
	http = new QHttp(server.host(), server.port(), this);
}

void uTorrentInterface::clear()
{
}

#include "utorrentinterface.moc"
