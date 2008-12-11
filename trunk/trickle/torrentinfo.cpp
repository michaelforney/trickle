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
#include "torrentinfo.h"

#include <QProgressBar>
#include <QVBoxLayout>
#include <QLabel>

//#include "selectedtorrent.h"
#include "torrentitem.h"

TorrentInfo::TorrentInfo()
 : QWidget()
{
	ui.setupUi(this);

	//connect(SelectedTorrent::instance(), SIGNAL(torrentChanged(Torrent *)), this, SLOT(update()));
	//connect(SelectedTorrent::instance(), SIGNAL(torrentUpdated()), this, SLOT(update()));
}

TorrentInfo::~TorrentInfo()
{
}

void TorrentInfo::update() const
{
	/*if (SelectedTorrent::instance()->hasTorrent())
	{
		ui.progress->setMaximum(SelectedTorrent::torrentInstance()->chunks());
		ui.progress->setValue(SelectedTorrent::torrentInstance()->completedChunks());
		ui.torrentName->setText(SelectedTorrent::torrentInstance()->name());
	}*/
}

#include "torrentinfo.moc"


