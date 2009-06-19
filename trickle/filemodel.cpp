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
#include "filemodel.h"

#include "torrent.h"

#include <KDebug>
#include <QIcon>
#include <QMenu>

FileModel::FileModel()
: QAbstractItemModel()
{
    headers << "File" << "Size" << "Complete" << "Priority";
    
    //connect(SelectedTorrent::instance(), SIGNAL(torrentChanged(Torrent *)), this, SLOT(newUpdate()));
    //connect(SelectedTorrent::instance(), SIGNAL(torrentUpdated()), this, SLOT(update()));
}


FileModel::~FileModel()
{
}

QModelIndex FileModel::index(int row, int column, const QModelIndex & parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    return createIndex(row, column);
}

QModelIndex FileModel::parent(const QModelIndex & index) const
{
    return QModelIndex();
    if (index.isValid())
    {
        QString path = *static_cast<QString *>(index.internalPointer());
        QStringList components = path.split("/");
        components.removeLast();
        QString parentPath = components.join("/");
        if (!parentPath.isEmpty())
        {
            //return createIndex(parent->row(), 0);
        }
        else
        {
            return QModelIndex();
        }
    }
    else
    {
        return QModelIndex();
    }
}

int FileModel::rowCount(const QModelIndex & parent) const
{
    /*if (SelectedTorrent::instance()->hasTorrent())
    {
        if (parent.isValid())
        {
            FileModelItem * item = static_cast<FileModelItem *>(parent.internalPointer());
            if (item->isDirectory())
            {
                return item->toDirectory()->childCount();
            }
            else
            {
                return 0;
            }
        }
        else if (mainItem)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
}*/
    return childDirectories(parent).count() + childFiles(parent).count();
}

int FileModel::columnCount(const QModelIndex &) const
{
    return headers.size();
}

QVariant FileModel::data(const QModelIndex & index, int role) const
{
    /*if (SelectedTorrent::instance()->hasTorrent())
    {
        if (!index.isValid())
        {
            return QVariant();
        }
        
        FileModelItem * item = static_cast<FileModelItem *>(index.internalPointer());
        if (item < 0)
        {
            return QVariant();
        }
        
        if (role == Qt::DisplayRole)
        {
            if (index.column() == FileModel::Name)
            {
                return item->name();
            }
            else if (index.column() == FileModel::Size)
            {
                return item->size().toString();
            }
            else if (index.column() == FileModel::Complete)
            {
                return QString("%1%").arg(item->complete());
            }
            else if (index.column() == FileModel::Priority)
            {
                switch(item->priority())
                {
                    case FileItem::Skip:
                        return QString("Skip");
                    case FileItem::Normal:
                        return QString("Normal");
                    case FileItem::High:
                        return QString("High");
                    default:
                        return QVariant();
                }
            }
            else
            {
                return QVariant();
            }
        }
        else if (role == Qt::DecorationRole && index.column() == Name)
        {
            if (item->isFile())
            {
                return QIcon(":/images/file.png");
            }
            else if (item->isDirectory())
            {
                return QIcon(":/images/folder.png");
            }
        }
        else if (role == Qt::CheckStateRole && index.column() == Name)
        {
            return item->priority();
        }
        else
        {
            return QVariant();
        }
    }*/
    return QVariant();
}

QVariant FileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return headers[section];
    }
    else
    {
        return QVariant();
    }
}

void FileModel::update()
{
    /*if (SelectedTorrent::instance()->hasTorrent())
    {
        QVariantList args;
        //args << SelectedTorrent::torrentInstance()->hash();
        args << "";
        args << "f.get_path=";
        args << "f.get_size_chunks=";
        args << "f.get_completed_chunks=";
        args << "f.get_priority=";
        args << "f.get_size_bytes=";
        //XmlRpc::instance()->call("f.multicall", args, this, "result");
    }*/
}

Qt::ItemFlags FileModel::flags(const QModelIndex & index) const
{
    if (!index.isValid())
    {
        return Qt::ItemIsEnabled;
    }
    else
    {
        return QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable | Qt::ItemIsTristate;
    }
}

bool FileModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    /*if (!index.isValid())
    {
        return false;
    }
    
    FileModelItem * item = static_cast<FileModelItem *>(index.internalPointer());
    if (role == Qt::CheckStateRole && index.column() == FileModel::Name)
    {
        item->changePriority(value.toInt());
        emit dataChanged(createIndex(index.row(), 0, item), createIndex(index.row(), headers.size() - 1, item));
        return true;
    }
    else
    {
        return false;
    }*/
}

/*void FileModel::updateItem(FileItem * item, const QVariantList & data, int index)
{
    if (item)
    {
        QString namePath = data[0].toString();
        int chunks = data[1].toInt();
        int chunksCompleted = data[2].toInt();
        int priority = data[3].toInt();
        ByteSize size = data[4].toLongLong();
        QString name = namePath.section("/", -1);
        
        item->setName(name);
        item->setSize(size);
        item->setCompletedChunks(chunksCompleted);
        item->setPriority(priority);
        item->setIndex(index);
    }
}*/

/*FileModelItem * FileModel::findItem(const QString & path) const
{
    QStringList folders = path.split("/");
    if (folders.size() == 0)
    {
        return mainItem;
    }
    
    return mainItem->toDirectory()->item(path);
}*/

QStringList FileModel::childDirectories(const QModelIndex & index) const
{
    if (!hasIndex(index.row(), index.column(), index))
    {
        return QStringList();
    }

    if (index.isValid())
    {
        QString parentPath = path(index);
        QRegExp filter(QString("^(%1/[^/]+)/.*").arg(parentPath));
        QStringList dirs = QStringList(fileMap.keys()).filter(filter).replaceInStrings(filter, "\\1");
        dirs.removeDuplicates();
        return dirs;
    }
    else
    {
        QRegExp filter("^([^/]+)/.*");
        QStringList dirs = QStringList(fileMap.keys()).filter(filter).replaceInStrings(filter, "\\1");
        dirs.removeDuplicates();
        return dirs;
    }
}

QStringList FileModel::childFiles(const QModelIndex & index) const
{
    if (!hasIndex(index.row(), index.column(), index))
    {
        return QStringList();
    }

    if (index.isValid())
    {
        QString parentPath = path(index);
        QRegExp filter(QString("^%1/[^/]+$").arg(parentPath));
        return QStringList(fileMap.keys()).filter(filter);
    }
    else
    {
        return QStringList(fileMap.keys()).filter(QRegExp("^[^/]+$"));
    }
}

QString FileModel::path(const QModelIndex & index) const
{
    if (!hasIndex(index.row(), index.column(), index))
    {
        return QString();
    }

    if (index.isValid())
    {
        QString parentPath = path(index.parent());
        QStringList entries = fileMap.keys();
        QRegExp filter(QString("^(%1/[^/]+)/.*").arg(parentPath));
        entries.replaceInStrings(filter, "\\1");
        return entries.at(index.row());
    }
    else
    {
        QString parentPath = path(index.parent());
        QStringList entries = fileMap.keys();
        QRegExp filter("^([^/]+)/.*");
        entries.replaceInStrings(filter, "\\1");
        return entries.at(index.row());
    }
}

#include "filemodel.moc"
