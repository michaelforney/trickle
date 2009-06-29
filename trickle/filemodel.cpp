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
#include "filemodel.h"

#include "torrent.h"
#include "interface.h"
#include "interfacemanager.h"

#include <KDebug>
#include <KMimeType>
#include <KIcon>
//#include <QMenu>

FileModel::FileModel()
: QAbstractItemModel()
{
    headers << "File" << "Size" << "Complete" << "Priority";

    kDebug() << QModelIndex().row();
    kDebug() << QModelIndex().column();
    kDebug() << QModelIndex().internalId();
    connect(InterfaceManager::self(), SIGNAL(interfaceChanged(Interface *)), this, SLOT(setupInterfaceConnections(Interface *)));
    if (InterfaceManager::interface())
    {
        setupInterfaceConnections(InterfaceManager::interface());
    }
}


FileModel::~FileModel()
{
}

QModelIndex FileModel::index(int row, int column, const QModelIndex & parentIndex) const
{
    kDebug() << "START-> row: " << row << ", column: " << column << ", parentIndex: " << parentIndex << ", parentId: " << parentIndex.internalId();
    /*if (!hasIndex(row, column, parentIndex))
    {
        return QModelIndex();
    }*/

    QString parentPath = findPath(parentIndex);
    //kDebug() << "parentPath: " << parentPath;
    QStringList childDirectories = children(parentPath, DirectoryIndex);
    //kDebug() << "directories: " << childDirectories;
    int directoryCount = childDirectories.size();
    if (row >= directoryCount) // File
    {
        QStringList childFiles = children(parentPath, FileIndex);
        //kDebug() << "files: " << childFiles;
        int index = m_files.keys().indexOf(children(parentPath, FileIndex).at(row - directoryCount));
        Q_ASSERT(index != -1);
        kDebug() << "CREATINGINDEX-> row: " << row << ", col: " << column << ", id: " << index;
        return createIndex(row, column, index); // This might be able to be more efficient
    }
    else // Directory
    {
        int index = m_dirs.indexOf(childDirectories.at(row));
        Q_ASSERT(index != -1);
        qint32 id = -(index + 1); // For dirs, 0 -> -1, 1 -> -2, 2 -> -3, etc
        //kDebug() << "dirId: " << id;
        kDebug() << "CREATINGINDEX-> row: " << row << ", col: " << column << ", id: " << id;
        return createIndex(row, column, id); // Same goes for here
    }
}

QModelIndex FileModel::index(const QString & path, IndexType type) const
{
    kDebug() << "FROMPATH-> path: " << path << ", type: " << (type == FileIndex ? "File" : "Directory");
    if (path.isEmpty())
    {
        return QModelIndex();
    }
    switch (type)
    {
        case FileIndex:
        {
            int id = m_files.keys().indexOf(path);
            Q_ASSERT(id != -1);
            QString parentPath = findParentPath(path);
            int row = children(parentPath, DirectoryIndex).indexOf(path);
            return createIndex(row, 0, id);
        }
        case DirectoryIndex:
        {
            int index = m_dirs.indexOf(path);
            Q_ASSERT(index != -1);
            QString parentPath = findParentPath(path);
            int row = children(parentPath, DirectoryIndex).size() + children(parentPath, FileIndex).indexOf(path);
            qint32 id = -(index + 1);
            return createIndex(row, 0, id);
        }
    }
    return QModelIndex();
}

QModelIndex FileModel::parent(const QModelIndex & index) const
{
    kDebug() << index;
    if (index.isValid())
    {
        QString indexPath = findPath(index);
        QString parentPath = findParentPath(indexPath);
        QString parentParentPath = findParentPath(parentPath);
        if (!parentPath.isEmpty())
        {
            QStringList parentSiblingDirectories = children(parentParentPath, DirectoryIndex);
            int index = m_dirs.indexOf(parentPath);
            qint32 id = -(index + 1);
            return createIndex(parentSiblingDirectories.indexOf(parentPath), 0, id);
        }
    }
    return QModelIndex();
}

int FileModel::rowCount(const QModelIndex & parent) const
{
    kDebug() << parent;
    qint32 id = parent.internalId();
    if (!parent.isValid() || id < 0) // Directory
    {
        //kDebug() << "dir";
        QString parentPath = findPath(parent);
        int count = children(parentPath, DirectoryIndex).count() + children(parentPath, FileIndex).count();
        //kDebug() << "count is: " << count;
        return count;
    }
    else // File
    {
        //kDebug() << "file";
        return 0;
    }
}

int FileModel::columnCount(const QModelIndex & index) const
{
    Q_UNUSED(index);
    return headers.size();
}

QVariant FileModel::data(const QModelIndex & index, int role) const
{
    //kDebug() << index << role;
    Q_ASSERT(index.isValid());
    //kDebug() << index.isValid();

    qint32 id = index.internalId();
    IndexType type = id < 0 ? DirectoryIndex : FileIndex;
    int listIndex = id < 0 ? -(id + 1) : id;
    switch (role)
    {
        case Qt::DisplayRole:
        {
            switch (index.column())
            {
                case Name:
                {
                    switch (type)
                    {
                        case FileIndex:
                            return m_files.keys().at(listIndex).split("/").last();
                        case DirectoryIndex:
                            return m_dirs.at(listIndex).split("/").last();
                        default:
                            return QVariant();
                    }
                }
                case Size:
                {
                    switch (type)
                    {
                        case FileIndex:
                            return size(m_files.keys().at(listIndex), type).toString();
                        case DirectoryIndex:
                            return size(m_dirs.at(listIndex), type).toString();
                    }
                    return QVariant();
                }
                case Complete:
                {
                    QString path;
                    switch (type)
                    {
                        case FileIndex:
                        {
                            path = m_files.keys().at(listIndex);
                            break;
                        }
                        case DirectoryIndex:
                        {
                            path = m_dirs.at(listIndex);
                            break;
                        }
                        default:
                            return QVariant();
                    }
                    int percent = bytesComplete(path, type) * 100 / size(path, type);
                    percent = qMin(100, percent);
                    percent = qMax(0, percent);
                    return QString("%1%").arg(percent);
                }
                case Priority:
                {
                    // TODO: Implement this
                    /*switch(item->priority())
                    {
                        case FileItem::Skip:
                            return QString("Skip");
                        case FileItem::Normal:
                            return QString("Normal");
                        case FileItem::High:
                            return QString("High");
                        default:
                            return QVariant();
                    }*/
                }
            }
        }
        case Qt::DecorationRole:
        {
            if (index.column() == Name)
            {
                switch (type)
                {
                    case FileIndex:
                        return KIcon(KMimeType::findByPath(findPath(index), 0, true)->iconName());
                    case DirectoryIndex:
                        return KIcon(KMimeType::mimeType("inode/directory")->iconName());
                }
            }
            return QVariant();
        }
        case Qt::CheckStateRole:
        {
            if (index.column() == Name)
            {
                // TODO: Implement this
            }
            return QVariant();
        }
    }
    return QVariant();
}

QVariant FileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return headers.at(section);
    }
    else
    {
        return QVariant();
    }
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
    //TODO: Implement
    if (!index.isValid())
    {
        return false;
    }

    if (role == Qt::CheckStateRole && index.column() == FileModel::Name)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void FileModel::clear()
{
    if (m_files.size() == 0)
    {
        return;
    }
    beginRemoveRows(QModelIndex(), 0, children(QString(), DirectoryIndex).size() + children(QString(), FileIndex).size() - 1);
    m_dirs.clear();
    m_files.clear();
    endRemoveRows();
    m_chunkSize = 0;
}

void FileModel::filesUpdated(const QString & hash, const QMap<QString, File> & files)
{
    kDebug() << hash;
    if (m_hash != hash)
    {
        return;
    }

    if (m_files.isEmpty())
    {
        kDebug() << "empty, creating new list";
        QStringList rootLevelItems = QStringList(files.keys()).replaceInStrings(QRegExp("^([^/]+)/.*"), QString("\\1"));
        rootLevelItems.removeDuplicates();
        kDebug() << "inserting from 0 to " << rootLevelItems.size() - 1;
        beginInsertRows(QModelIndex(), 0, rootLevelItems.size() - 1);
        m_files = files;
        m_dirs = findDirs(files.keys());
        //kDebug() << "dirs: " << m_dirs;
        endInsertRows();
    }
    else if (m_files.keys() == files.keys())
    {
        kDebug() << "the same, updating list";
        m_files = files;
        emit dataChanged(index(0, 0, QModelIndex()), index(rowCount() - 1, headers.size() - 1, QModelIndex()));
    }
    else
    {
        kWarning() << "The list of files changed with an update. This shouldn't happen.";
        // All this is probably unnecesary because the list of files won't be changing, but I guess its good to prepare for it anyway
        /*QStringList filePaths = files.keys();
        QStringList directoryPaths = findDirs(filePaths);
        for(int i = 0; i < m_dirs.size(); i++)
        {
            QString directoryPath = m_dirs.at(i);
            if (!directoryPaths.contains(directoryPath))
            {
                QStringList siblingDirectories = children(parentPath(directoryPath), DirectoryIndex);
                int row = siblingDirectories.indexOf(directoryPath);
                beginRemoveRows(index(findParentPath(directoryPaths.at(i))), row, row);
                m_dirs.removeAt(i);
                for (int fileIndex = 0; fileIndex <
                     i--;
                endRemoveRows();
            }
        }*/
    }
}

void FileModel::setupInterfaceConnections(Interface * interface)
{
    kDebug() << interface;
    connect(interface, SIGNAL(filesUpdated(const QString &, const QMap<QString, File> &)), this, SLOT(filesUpdated(const QString &, const QMap<QString, File> &)));
    connect(interface, SIGNAL(watchedTorrentUpdated(const Torrent &)), this, SLOT(torrentUpdated(const Torrent &)));
}

QStringList FileModel::findDirs(const QStringList & filePaths)
{
    QStringList paths = filePaths.filter("/"); // Get rid of root files
    QStringList dirPaths;
    foreach(const QString & filePath, paths)
    {
        QStringList filePathComponents = filePath.split("/");
        filePathComponents.takeLast();
        while (!filePathComponents.isEmpty())
        {
            dirPaths.append(filePathComponents.join("/"));
            filePathComponents.removeLast();
        }
    }
    dirPaths.removeDuplicates(); // Remove duplicate directories
    return dirPaths;
}

QStringList FileModel::children(const QString & path, IndexType type) const
{
    kDebug() << path << ", " << (type == FileIndex ? "File" : "Directory");
    QStringList entries;
    switch (type)
    {
        case FileIndex:
            entries = m_files.keys();
            break;
        case DirectoryIndex:
            entries = m_dirs;
            break;
    }
    //Q_ASSERT(path.isEmpty() || entries.contains(path));
    QString subpath = path.isEmpty() ? QString() : QString("%1/").arg(path);
    QRegExp filter(QString("^(%1[^/]+)%2$").arg(subpath).arg(type == DirectoryIndex ? "(:?/[^/]+)?" : QString()));
    //kDebug() << filter.pattern();
    QStringList matchedEntries = entries.filter(filter);
    matchedEntries.replaceInStrings(filter, "\\1");
    if (type == DirectoryIndex)
    {
        matchedEntries.removeDuplicates();
    }
    //kDebug() << "children: " << matchedEntries;
    return matchedEntries;
}

QString FileModel::findPath(const QModelIndex & index) const
{
    kDebug() << index;
    /*if (!hasIndex(index.row(), index.column(), index) || !index.isValid())
    {
        return QString();
    }*/

    if (index.isValid())
    {
        qint32 id = index.internalId();
        if (id < 0) // Directory
        {
            int index = -(id + 1);
            Q_ASSERT(index >= 0 && index < m_dirs.size());
            //kDebug() << "index: " << index << ", size: " << m_dirs.size();
            //kDebug() << "id: " << id << ", path: " << m_dirs.at(index);
            return m_dirs.at(index);
        }
        else // File
        {
            Q_ASSERT(id >= 0 && id < m_files.size());
            //kDebug() << "id: " << id << ", path: " << m_files.keys().at(id);
            return m_files.keys().at(id);
        }
    }
    else
    {
        return QString();
    }
}

QString FileModel::findParentPath(const QString & path) const
{
    return QString(path).remove(QRegExp("(?:^|/)[^/]+$"));
}

ByteSize FileModel::size(const QString & path, IndexType type) const
{
    switch (type)
    {
        case FileIndex:
            return m_files.value(path).size();
        case DirectoryIndex:
        {
            ByteSize totalSize;
            foreach(const QString & directoryPath, children(path, DirectoryIndex))
            {
                totalSize += size(directoryPath, DirectoryIndex);
            }
            foreach(const QString & filePath, children(path, FileIndex))
            {
                totalSize += size(filePath, FileIndex);
            }
            return totalSize;
        }
    }
    return ByteSize();
}

ByteSize FileModel::bytesComplete(const QString & path, IndexType type) const
{
    switch (type)
    {
        case FileIndex:
            return ByteSize(m_chunkSize.bytes() * m_files.value(path).completedChunks());
        case DirectoryIndex:
        {
            ByteSize totalBytesComplete;
            foreach(const QString & directoryPath, children(path, DirectoryIndex))
            {
                totalBytesComplete += bytesComplete(directoryPath, DirectoryIndex);
            }
            foreach(const QString & filePath, children(path, FileIndex))
            {
                totalBytesComplete += bytesComplete(filePath, FileIndex);
            }
            return totalBytesComplete;
        }
    }
}

void FileModel::setTorrentHash(const QString & hash)
{
    if (m_hash == hash)
    {
        return;
    }

    clear();
    Q_ASSERT(InterfaceManager::interface());
    InterfaceManager::interface()->stopWatchingTorrent(hash);
    m_hash = hash;
    InterfaceManager::interface()->watchTorrent(hash);
}

void FileModel::torrentUpdated(const Torrent & torrent)
{
    if (m_hash == torrent.hash())
    {
        m_chunkSize = torrent.chunkSize();
    }
}


#include "filemodel.moc"
