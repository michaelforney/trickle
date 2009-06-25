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
#include "utorrentconfig.h"

#include "bcodec.h"

#include <KDebug>

uTorrentConfig::uTorrentConfig(QWidget * parent)
 : InterfaceConfigWidget(parent)
{
	ui.setupUi(this);
}

uTorrentConfig::~uTorrentConfig()
{
}

QByteArray uTorrentConfig::save() const
{
	VariantHash config;
	config.insert("info", "trickle_utorrent 0.1");
	return BCodec::encode(qVariantFromValue(config));
}

void uTorrentConfig::load(const QByteArray & data)
{
	VariantHash config = BCodec::decode(data).value<VariantHash>();
	if (config.value("info") == "trickle_utorrent 0.1")
	{
	}
}

void uTorrentConfig::clear()
{
}
