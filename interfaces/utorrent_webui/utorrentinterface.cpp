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
#include "bytesize.h"
#include "csvcodec.h"

#include <KGenericFactory>
#include <QAuthenticator>
#include <QHttp>

K_PLUGIN_FACTORY(uTorrentInterfaceFactory, registerPlugin<uTorrentInterface>();)
K_EXPORT_PLUGIN(uTorrentInterfaceFactory("trickle_utorrent"))

uTorrentInterface::uTorrentInterface(QObject * parent, const QVariantList & /*args*/)
 : Interface(parent)
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

void uTorrentInterface::clear()
{
}

void uTorrentInterface::updateTorrentList()
{
    if (!jobs.isEmpty())
    {
        return;
    }

    KUrl url;
    url.setScheme("http");
    url.setHost(m_server.host());
    url.setPort(m_server.port());
    url.setPath("/gui/?list=1");

    KIO::StoredTransferJob * job = KIO::storedGet(url, KIO::Reload, KIO::HideProgressInfo);
    
    jobs.insert(job, TorrentList);
}

void uTorrentInterface::updateFileList(const QString & hash)
{
}

void uTorrentInterface::jobFinished(KJob * job)
{
    KIO::StoredTransferJob * transferJob = qobject_cast<KIO::StoredTransferJob *>(job);
    if (transferJob)
    {
        if (transferJob->error())
        {
            kdDebug() << "ERROR!!! Run for your lives!!!";
            kdDebug() << transferJob->errorString();
            return;
        }
        
        if (jobs.contains(transferJob))
        {
            WebUIRequest requestType = jobs.value(transferJob);
            if (requestType == TorrentList)
            {
                QVariantMap response = CSVCodec::decode(transferJob->data()).toMap();
                QVariantList torrents = response.value("torrents").toList();
                QMap<QString, Torrent> torrentMap;
                foreach(QVariant torrentVariant, torrents)
                {
                    QVariantList webuiTorrent = torrentVariant.toList();
                    Torrent torrent(webuiTorrent.at(0).toString());
                    torrent.setName(webuiTorrent.at(2).toString());
                    torrent.setSize(webuiTorrent.at(3).toLongLong());
                    torrent.setDownloaded(webuiTorrent.at(5).toLongLong());
                    torrent.setUploaded(webuiTorrent.at(6).toLongLong());
                    torrentMap.insert(webuiTorrent.at(0).toString(), torrent);
                }
                qDebug() << "emitting";
                emit torrentMapUpdated(torrentMap);
            }
        }
    }
}

#include "utorrentinterface.moc"
