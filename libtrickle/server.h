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
#ifndef SERVER_H
#define SERVER_H

/**
    @author Michael Forney <mforney@trickleproject.org>
*/

#include <QString>

#include "trickle_export.h"

class TRICKLE_EXPORT Server
{
    public:
        Server();
        Server(const QString & name, const QString & host, int port, const QString & type, const QByteArray & typeConfig);
        ~Server();
        
        inline void setPort(int port) { m_port = port; };
        inline void setHost(const QString & host) { m_host = host; };
        inline void setName(const QString & name) { m_name = name; };
        inline void setType(const QString & type) { m_type = type; };
        inline void setTypeConfig(const QByteArray & typeConfig) { m_typeConfig = typeConfig; };
        
        inline int port() const { return m_port; };
        inline QString host() const { return m_host; };
        inline QString name() const { return m_name; };
        inline QString type() const { return m_type; }
        inline QByteArray typeConfig() const { return m_typeConfig; }
        
        bool operator==(const Server & other) const;
        bool operator<(const Server & other) const;
    private:
        int m_port;
        QString m_host;
        QString m_name;
        QString m_type;
        QByteArray m_typeConfig;
};

#endif
