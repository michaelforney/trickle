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
#include "torrentinfo.h"

#include "interfacemanager.h"
#include "interface.h"

#include <QProgressBar>
#include <QVBoxLayout>
#include <QLabel>
#include <KDebug>

TorrentInfo::TorrentInfo()
 : QWidget()
{
	ui.setupUi(this);

    
	//connect(SelectedTorrent::instance(), SIGNAL(torrentChanged(Torrent *)), this, SLOT(update()));
	//connect(SelectedTorrent::instance(), SIGNAL(torrentUpdated()), this, SLOT(update()));
    
    connect(InterfaceManager::self(), SIGNAL(interfaceChanged(Interface *)), this, SLOT(setupInterfaceConnections(Interface *)));
    if (InterfaceManager::interface())
    {
        setupInterfaceConnections(InterfaceManager::interface());
    }
}

TorrentInfo::~TorrentInfo()
{
}

void TorrentInfo::setTorrentHash(const QString & torrentHash)
{
    if (hash == torrentHash)
    {
        return;
    }

    Q_ASSERT(InterfaceManager::interface());
    InterfaceManager::interface()->stopWatchingTorrent(hash);
    hash = torrentHash;
    InterfaceManager::interface()->watchTorrent(hash);
}

void TorrentInfo::setupInterfaceConnections(Interface * interface)
{
    kDebug() << interface;
    connect(interface, SIGNAL(watchedTorrentUpdated(const Torrent &)), this, SLOT(torrentUpdated(const Torrent &)));
}

void TorrentInfo::torrentUpdated(const Torrent & torrent)
{
    if (hash == torrent.hash())
    {
        ui.progress->setMaximum(torrent.chunks());
        ui.progress->setValue(torrent.completedChunks());
        ui.torrentName->setText(torrent.name());
        ui.chunksCompleted->setText(QString("%1/%2").arg(torrent.completedChunks()).arg(torrent.chunks()));
        ui.chunkSize->setText(torrent.chunkSize().toString());
    }
}

#include "torrentinfo.moc"


