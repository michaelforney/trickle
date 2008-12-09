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
#include "rtorrentconfig.h"

#include "bcodec.h"

#include <KDebug>

rTorrentConfig::rTorrentConfig(QWidget * parent)
 : InterfaceConfigWidget(parent)
{
	ui.setupUi(this);
}

rTorrentConfig::~rTorrentConfig()
{
}

QByteArray rTorrentConfig::save() const
{
	VariantHash config;
	config.insert("info", "trickle_rtorrent 0.1");
	config.insert("path", ui.pathEdit->text());
	return BCodec::encode(qVariantFromValue(config));
}

void rTorrentConfig::load(const QByteArray & data)
{
	VariantHash config = BCodec::decode(data).value<VariantHash>();
	//kdDebug() << qHash(BCodec::decode(data));
	if (config.value("info") == "trickle_rtorrent 0.1")
	{
		ui.pathEdit->setText(config.value("path").toString());
	}
}

void rTorrentConfig::clear()
{
	ui.pathEdit->setText("/RPC2");
}
