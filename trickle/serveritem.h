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
#ifndef SERVERITEM_H
#define SERVERITEM_H

#include <QTreeWidgetItem>

/**
    @author Michael Forney <mforney@trickleproject.org>
*/

#include "server.h"

class ServerItem : public QTreeWidgetItem
{
    public:
        ServerItem(QTreeWidget * parent, const Server & server);
        ~ServerItem();
        QVariant data(int column, int role) const;
        void setData(int column, int role, const QVariant & value);
        inline Server server() const { return m_server; }
        inline void setServer(const Server & server) { m_server = server; }
        
        enum Column { Name, Host, Port, Type };
    private:
        Server m_server;
};

#endif
