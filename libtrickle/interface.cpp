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
#include "interface.h"

#include "server.h"

#include <QTimer>
#include <QStringList>
#include <KDebug>

Interface::Interface(InterfaceConfig * config, QObject * parent)
 : QObject(parent), m_timer(new QTimer()), m_config(config)
{
	connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
}

Interface::Interface(QObject * parent)
: QObject(parent), m_timer(new QTimer()), m_config(0)
{
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
}

Interface::~Interface()
{
}

void Interface::setServer(const Server & server)
{
    m_server = server;
    reset();
}

void Interface::setInterval(int interval)
{
	m_timer->setInterval(interval);
    qDebug() << interval;
}

bool Interface::start()
{
    kdDebug() << "Interface::start()";
    updateTorrentList();
    m_timer->start();
    return true;
}

bool Interface::stop()
{
	m_timer->stop();
	return true;
}

void Interface::reset()
{
	stop();
	clear();
}

Server Interface::server() const
{
	return m_server;
}

void Interface::setConfig(InterfaceConfig * config)
{
    m_config = config;
    reset();
}

InterfaceConfig * Interface::genericConfig() const
{
    return m_config;
}

QMap<QString, int> Interface::watchedTorrents() const
{
    return m_watchedTorrents;
}

void Interface::watchTorrent(const QString & hash)
{
    kDebug() << "watching torrent: " << hash;
    int count = m_watchedTorrents.value(hash, 0) + 1;
    m_watchedTorrents.insert(hash, count);
    //updateWatchedTorrent(hash);
}

void Interface::stopWatchingTorrent(const QString & hash)
{
    if (!m_watchedTorrents.contains(hash))
    {
        return;
    }

    int count = m_watchedTorrents.take(hash) - 1;
    if (count > 0)
    {
        m_watchedTorrents.insert(hash, count);
    }
}

void Interface::updateWatchedTorrent(const QString & hash)
{
    if (hash.isNull() || hash.isEmpty())
    {
        kDebug() << "Trying to watch an empty hash!";
        return;
    }
    updateFileInfo(hash);
    updatePeerInfo(hash);
    updateTrackerInfo(hash);
    //updateChunkInfo(hash);
}

void Interface::updateWatchedTorrents()
{
    foreach(QString hash, m_watchedTorrents.keys())
    {
        updateWatchedTorrent(hash);
    }
}

void Interface::update()
{
    updateTorrentList();
    updateWatchedTorrents();
}

#include "interface.moc"
