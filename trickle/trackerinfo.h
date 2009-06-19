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
#ifndef TRACKERINFO_H
#define TRACKERINFO_H

#include <QWidget>

/**
	@author Michael Forney <michael@obberon.com>
*/

#include "ui_trackerinfo.h"

class QPushButton;

class TrackerModel;
class TrackerView;
class Torrent;

class TrackerInfo : public QWidget
{
	Q_OBJECT
	public:
		TrackerInfo();
		~TrackerInfo();
		
		void setTorrent(Torrent * torrent);
	public slots:
	private:
		Ui::TrackerInfo ui;

		TrackerModel * trackerModel;
		QTreeView * trackerView;
		
		QPushButton * updateTrackerButton;
		QPushButton * forceUpdateTrackerButton;
};

#endif
