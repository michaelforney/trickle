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
#include "fileinfo.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QHeaderView>
#include <QSettings>

#include "fileview.h"
#include "filemodel.h"
#include "filedelegate.h"

FileInfo::FileInfo()
 : QWidget()
{
	fileView = new FileView();
	fileModel = new FileModel();
	//fileDelegate = new FileDelegate();
	fileView->setModel(fileModel);
	//fileView->setItemDelegateForColumn(FileModel::Priority, fileDelegate);
	
	QHBoxLayout * mainLayout = new QHBoxLayout();
	{
		mainLayout->addWidget(fileView);
	}
	setLayout(mainLayout);
	
	readSettings();
}


FileInfo::~FileInfo()
{
	writeSettings();
}

void FileInfo::update()
{
	fileModel->update();
}

void FileInfo::writeSettings()
{
	QSettings settings;
	settings.setValue("fileinfo/headerstate", fileView->header()->saveState());
}

void FileInfo::readSettings()
{
	QSettings settings;
	fileView->header()->restoreState(settings.value("fileinfo/headerstate").toByteArray());
}

#include "fileinfo.moc"
