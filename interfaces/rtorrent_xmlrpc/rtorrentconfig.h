/***************************************************************************
 *   Copyright (C) 2007 by Michael Forney                                  *
 *   michael@obberon.com                                                   *
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
#ifndef RTORRENTCONFIG_H
#define RTORRENTCONFIG_H

#include "interfaceconfigwidget.h"

/**
	@author Michael Forney <michael@obberon.com>
*/

#include "ui_rtorrentconfig.h"

class rTorrentConfig : public InterfaceConfigWidget
{
	public:
		rTorrentConfig(QWidget * parent = 0);
		~rTorrentConfig();

		QByteArray save() const;
		void load(const QByteArray & data);
		void clear();
	private:
		Ui::rTorrentConfig ui;
};

#endif
