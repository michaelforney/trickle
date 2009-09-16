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

#include <KDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QHeaderView>

#include "fileview.h"
#include "filemodel.h"

FileInfo::FileInfo()
 : QWidget()
{
    fileModel = new FileModel();

    ui.setupUi(this);
    ui.fileView->setModel(fileModel);

    setEnabled(false);
    firstUpdate = true;

    connect(ui.fileView, SIGNAL(fileChanged(const QString &)), this, SLOT(fileChanged(const QString &)));

    restore();
}


FileInfo::~FileInfo()
{
}

void FileInfo::save()
{
    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup group = config->group("FileInfo");
    group.writeEntry("State", ui.fileView->header()->saveState());
}

void FileInfo::restore()
{
    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup group = config->group("FileInfo");
    ui.fileView->header()->restoreState(group.readEntry("State", QByteArray()));
}

void FileInfo::fileChanged(const QString & path)
{
    kDebug() << "file changed";
    Q_UNUSED(path); // TODO: Remove when implemented
}

void FileInfo::setTorrentHash(const QString & hash)
{
    clear();
    setEnabled(false);
    fileModel->setTorrentHash(hash);
}

void FileInfo::clear()
{
    fileModel->clear();
    ui.name->clear();
    ui.progress->setValue(0);
    ui.size->clear();
    ui.priority->clear();
    ui.priorityEdit->setCurrentIndex(0);
}

#include "fileinfo.moc"
