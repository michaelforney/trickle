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
#include "servereditdialog.h"

#include "server.h"
#include "interfacemanager.h"
#include "interfaceconfigwidget.h"
#include "interfaceconfig.h"

#include <KDebug>

ServerEditDialog::ServerEditDialog(QWidget * parent)
 : QDialog(parent)
{
    ui.setupUi(this);
    while (ui.interfaceConfig->currentWidget() != 0)
    {
        ui.interfaceConfig->removeWidget(ui.interfaceConfig->currentWidget());
    }
    ui.typeEdit->clear();
    
    QStringList names = InterfaceManager::self()->names();
    foreach(QString name, names)
    {
        QWidget * configWidget = InterfaceManager::self()->configWidget(name);
        if (configWidget)
        {
            ui.interfaceConfig->addWidget(configWidget);
            ui.typeEdit->addItem(InterfaceManager::self()->title(name), name);
        }
    }
    ui.typeEdit->setCurrentIndex(0);
}

ServerEditDialog::~ServerEditDialog()
{
}

Server ServerEditDialog::server() const
{
    InterfaceConfigWidget * widget = static_cast<InterfaceConfigWidget *>(ui.interfaceConfig->currentWidget());
    if (widget && widget->genericConfig())
    {
        return Server(ui.nameEdit->text(), ui.hostEdit->text(), ui.portEdit->value(), InterfaceManager::self()->names().at(ui.typeEdit->currentIndex()), widget->genericConfig()->data());
    }
    return Server();
}

void ServerEditDialog::clear()
{
    ui.nameEdit->clear();
    ui.portEdit->setValue(80);
    ui.hostEdit->clear();
}

void ServerEditDialog::setServer(const Server & server)
{
    ui.nameEdit->setText(server.name());
    ui.portEdit->setValue(server.port());
    ui.hostEdit->setText(server.host());
    ui.typeEdit->setCurrentIndex(InterfaceManager::self()->names().indexOf(server.type()));
    InterfaceConfigWidget * widget = static_cast<InterfaceConfigWidget *>(ui.interfaceConfig->widget(InterfaceManager::self()->names().indexOf(server.type())));
    if (widget)
    {
        InterfaceConfig * config = InterfaceManager::self()->config(server.type());
        config->setData(server.typeConfig());
        widget->setConfig(config);
    }
    else
    {
        qDebug() << ui.interfaceConfig->count();
    }
}

#include "servereditdialog.moc"
