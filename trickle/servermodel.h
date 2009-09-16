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
#ifndef SERVERMODEL_H
#define SERVERMODEL_H

#include <QAbstractItemModel>

/**
    @author Michael Forney <mforney@trickleproject.org>
*/

#include <QList>
#include <QStringList>

class Server;

class ServerModel : public QAbstractItemModel
{
    Q_OBJECT
    public:
        ServerModel();
        ServerModel(QList<Server> servers);
        ~ServerModel();
        
        enum Column { Name, Host, Port, Type };
        
        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        int columnCount(const QModelIndex & parent) const;
        QModelIndex parent(const QModelIndex & index) const;
        QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        
        void removeServer(int row);
        Server server(int pos) const;
        
        inline void setStartupServer(int startupServer) { m_startupServer = startupServer; };
        inline int startupServer() const { return m_startupServer; };
        
        void addServer(Server server);
        void setServer(int index, Server server);
        void clear();
        
        QList<Server> servers();
        inline bool showNoServer() const { return m_showNoServer; };
    public slots:
        inline void setShowNoServer(bool showNoServer) { m_showNoServer = showNoServer; emit layoutChanged(); };
    private:
        QList<Server> m_servers;
        QStringList headers;
        int m_startupServer;
        bool m_showNoServer;
};

#endif
