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
#include "serveredit.h"

ServerEdit::ServerEdit(QWidget * parent)
 : QWidget(parent)
{
    ui.setupUi(this);
}

ServerEdit::~ServerEdit()
{
}

Server ServerEdit::server() const
{
    return Server(ui.nameEdit->text(), ui.hostEdit->text(), ui.pathEdit->text(), ui.portEdit->value(), ui.intervalEdit->value());
}

void ServerEdit::clear()
{
    ui.nameEdit->clear();
    ui.intervalEdit->setValue(2000);
    ui.portEdit->setValue(80);
    ui.hostEdit->clear();
    ui.pathEdit->setText("/RPC2");
}

void ServerEdit::setServer(const Server & server)
{
    ui.nameEdit->setText(server.name());
    ui.intervalEdit->setValue(server.interval());
    ui.portEdit->setValue(server.port());
    ui.hostEdit->setText(server.host());
    ui.pathEdit->setText(server.path());
}

#include "serveredit.moc"
