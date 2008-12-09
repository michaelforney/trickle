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
#ifndef FILEINFO_H
#define FILEINFO_H

#include <QWidget>

/**
	@author Michael Forney <michael@obberon.com>
*/

class QComboBox;

class FileView;
class FileModel;
//class FileDelegate;
class TorrentItem;

class FileInfo : public QWidget
{
	Q_OBJECT
	public:
		FileInfo();
		~FileInfo();
		
		void setTorrent(TorrentItem * torrent);
		
		void writeSettings();
		void readSettings();
	public slots:
		void update();
	private:
		FileView * fileView;
		FileModel * fileModel;
		//FileDelegate * fileDelegate;
		
		QComboBox * filePriority;
};

#endif
