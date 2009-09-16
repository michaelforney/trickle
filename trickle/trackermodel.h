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
#ifndef TRACKERMODEL_H
#define TRACKERMODEL_H

#include <QAbstractItemModel>

/**
    @author Michael Forney <mforney@trickleproject.org>
*/

#include "tracker.h"

#include <QStringList>

class TorrentItem;
class TrackerItem;
class Interface;

class TrackerModel : public QAbstractItemModel
{
    Q_OBJECT
    public:
        TrackerModel();
        ~TrackerModel();
        
        enum Column { TrackerUrl };
        
        QModelIndex index(int row, int column, const QModelIndex & parent) const;
        QModelIndex parent(const QModelIndex & index) const;
        int rowCount(const QModelIndex & parent) const;
        int columnCount(const QModelIndex & parent) const;
        QVariant data(const QModelIndex & item, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    public slots:
        void setupInterfaceConnections(Interface * interface);
        void setTorrentHash(const QString & hash);
        void trackersUpdated(const QString & hash, const QMap<int, Tracker> & trackers);
    private:
        QStringList headers;
        QMap<int, Tracker> trackerMap;
        QString m_hash;
};

#endif
