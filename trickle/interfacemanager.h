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
#ifndef INTERFACEMANAGER_H
#define INTERFACEMANAGER_H

#include <QObject>

/**
    @author Michael Forney <mforney@trickleproject.org>
*/

#include "server.h"

#include <QMap>

class Interface;
class InterfaceConfig;
class InterfaceConfigWidget;
class Server;

class KPluginFactory;

class QAuthenticator;

class InterfaceManager : public QObject
{
    Q_OBJECT
    public:
        InterfaceManager();
        ~InterfaceManager();
        static InterfaceManager * self();

        void load();
        QStringList names() const;
        InterfaceConfigWidget * configWidget(const QString & name);
        InterfaceConfig * config(const QString & name);
        QString title(const QString & name);
        QString description(const QString & name);
        
        static Interface * interface();
        static Server server();
    public slots:
        void setServer(const Server & server);
    signals:
        void interfaceChanged(Interface * interface);
    private:
        Interface * m_interface;
        Server m_server;
        QMap<QString, KPluginFactory *> factories;
};

#endif
