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
#ifndef RTORRENTCONFIGWIDGET_H
#define RTORRENTCONFIGWIDGET_H

#include "interfaceconfigwidget.h"

/**
	@author Michael Forney <michael@obberon.com>
*/

#include "ui_rtorrentconfigwidget.h"

class rTorrentConfig;

class rTorrentConfigWidget : public InterfaceConfigWidget
{
	public:
        rTorrentConfigWidget(QObject * parent, const QVariantList & args);
        ~rTorrentConfigWidget();

		QByteArray data() const;
		void setData(const QByteArray & data);
        void setConfig(InterfaceConfig * config);
        rTorrentConfig * config() const;
		void clear();
    protected:
        void updateWidgets();
	private:
		Ui::rTorrentConfigWidget ui;
};

#endif
