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
#include "rtorrentinterface.h"

#include "rtorrentconfig.h"
#include "xmlrpc.h"
#include "bytesize.h"

#include <KGenericFactory>

K_PLUGIN_FACTORY(rTorrentInterfaceFactory, registerPlugin<rTorrentInterface>();)
K_EXPORT_PLUGIN(rTorrentInterfaceFactory("trickle_rtorrent"))

rTorrentInterface::rTorrentInterface(QObject * parent, const QVariantList & /*args*/)
 : Interface(parent), xmlRpc(new XmlRpc())
{
}

rTorrentInterface::~rTorrentInterface()
{
}

InterfaceConfigWidget * rTorrentInterface::configWidget()
{
	return new rTorrentConfig();
}

void rTorrentInterface::setServer(Server * server)
{
}
		
void rTorrentInterface::setDownloadLimit(const ByteSize & size)
{
	xmlRpc->call("set_download_rate", QVariantList() << size.bytes());
}

void rTorrentInterface::setUploadLimit(const ByteSize & size)
{
	xmlRpc->call("set_upload_rate", QVariantList() << size.bytes());
}

void rTorrentInterface::startTorrent(const QString & hash)
{
}

void rTorrentInterface::stopTorrent(const QString & hash)
{
}

void rTorrentInterface::update()
{
}

bool rTorrentInterface::connectToServer()
{
	//xmlRpc->setServer(server());
}

void rTorrentInterface::clear()
{
}

#include "rtorrentinterface.moc"
