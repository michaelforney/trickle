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
#include "interfacemanager.h"

#include "interface.h"
#include "interfaceconfig.h"
#include "interfaceconfigwidget.h"
#include "server.h"
#include "settings.h"

#include <KServiceTypeTrader>
#include <KService>
#include <KDebug>
#include <KPluginFactory>

Q_GLOBAL_STATIC(InterfaceManager, interfaceManager);

InterfaceManager::InterfaceManager()
{
    m_interface = 0;
}

InterfaceManager::~InterfaceManager()
{
}

InterfaceManager * InterfaceManager::self()
{
    return interfaceManager();
}

void InterfaceManager::load()
{
    KService::List offers = KServiceTypeTrader::self()->query("Trickle/Interface");
    foreach(KService::Ptr service, offers)
    {
        factories.insert(service->desktopEntryName(), KPluginLoader(*service).factory());
    }
    
    qDebug() << "Default server: " << Settings::self()->defaultServer();
    setServer(Settings::self()->server(Settings::self()->defaultServer()));
}

QStringList InterfaceManager::names() const
{
    return factories.keys();
}

InterfaceConfig * InterfaceManager::config(const QString & name)
{
    if (factories.contains(name))
    {
        return factories.value(name)->create<InterfaceConfig>();
    }
    return 0;
}

InterfaceConfigWidget * InterfaceManager::configWidget(const QString & name)
{
    if (factories.contains(name))
    {
        return factories.value(name)->create<InterfaceConfigWidget>();
    }
    return 0;
}

QString InterfaceManager::title(const QString & name)
{
    if (factories.contains(name))
    {
        return factories.value(name)->create<Interface>()->title();
    }
    return QString();
}

QString InterfaceManager::description(const QString & name)
{
    if (factories.contains(name))
    {
        return factories.value(name)->create<Interface>()->description();
    }
    return QString();
}
        
Interface * InterfaceManager::interface()
{
    return self()->m_interface;
}

void InterfaceManager::setServer(const Server & server)
{
    if (factories.contains(server.type()))
    {
        if (m_interface)
        {
            delete m_interface;
        }
        m_interface = factories.value(server.type())->create<Interface>();
        m_interface->setServer(server);
        m_interface->setInterval(Settings::self()->interval());
        m_server = server;
        emit interfaceChanged(m_interface);
    }
}

Server InterfaceManager::server()
{
    return interfaceManager()->m_server;
}

#include "interfacemanager.moc"
