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
#ifndef UTORRENTCONFIG_H
#define UTORRENTCONFIG_H

#include "interfaceconfigwidget.h"

/**
    @author Michael Forney <mforney@trickleproject.org>
*/

#include "ui_utorrentconfig.h"

class uTorrentConfig : public InterfaceConfigWidget
{
    public:
        uTorrentConfig(QWidget * parent = 0);
        ~uTorrentConfig();

        QByteArray save() const;
        void load(const QByteArray & data);
        void clear();
    private:
        Ui::uTorrentConfig ui;
};

#endif
