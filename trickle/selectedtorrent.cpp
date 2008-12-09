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
#include "selectedtorrent.h"

#include <QDebug>

#include "torrentitem.h"

Q_GLOBAL_STATIC(SelectedTorrent, selectedTorrent);

SelectedTorrent::SelectedTorrent()
 : QObject()
{
	m_torrent = 0;
}

SelectedTorrent::~SelectedTorrent()
{
}

SelectedTorrent * SelectedTorrent::instance()
{
	return selectedTorrent();
}

TorrentItem * SelectedTorrent::torrentInstance()
{
	return selectedTorrent()->torrent();
}

void SelectedTorrent::setTorrent(TorrentItem * torrent)
{
	if (m_torrent > 0)
	{
		disconnect(m_torrent, SIGNAL(updated()), this, SIGNAL(torrentUpdated()));
	}
	m_torrent = torrent;
	if (torrent > 0)
	{
		connect(m_torrent, SIGNAL(updated()), this, SIGNAL(torrentUpdated()));
		emit torrentChanged(torrent);
		//qDebug("test1");
	}
}

TorrentItem * SelectedTorrent::torrent()
{
	return m_torrent;
}

bool SelectedTorrent::hasTorrent()
{
	return m_torrent > 0;
}

void SelectedTorrent::update()
{
	emit torrentChanged(m_torrent);
}

#include "selectedtorrent.moc"
