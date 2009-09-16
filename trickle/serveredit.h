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
#ifndef SERVEREDIT_H
#define SERVEREDIT_H

#include <QWidget>

/**
    @author Michael Forney <mforney@trickleproject.org>
*/

#include "ui_serveredit.h"

#include "server.h"

class ServerEdit : public QWidget
{
    Q_OBJECT
    public:
        ServerEdit(QWidget * parent = 0);
        ~ServerEdit();
        
        Server server() const;
    public slots:
        void clear();
        void setServer(const Server & server);
        
    private:
        Ui::ServerEdit ui;    
};

#endif
