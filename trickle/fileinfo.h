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
#ifndef FILEINFO_H
#define FILEINFO_H

#include <QWidget>

/**
	@author Michael Forney <mforney@trickleproject.org>
*/

#include "ui_fileinfo.h"

class QComboBox;

class FileView;
class FileModel;
class TorrentItem;

class FileInfo : public QWidget
{
	Q_OBJECT
	public:
		FileInfo();
		~FileInfo();

        void restore();
        void save();
    public slots:
        void setTorrentHash(const QString & hash);
        void clear();
    protected slots:
        void fileChanged(const QString & path);
	private:
        Ui::FileInfo ui;
        FileModel * fileModel;

        QString m_file;
        bool firstUpdate;
};

#endif
