/*
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA  02110-1301, USA.

    ---
    Copyright (C) 2009, Michael Forney <mforney@trickleproject.org>
 */

#include "rtorrentconfig.h"

rTorrentConfig::rTorrentConfig(QObject * parent, const QVariantList & args)
 : InterfaceConfig(parent)
{
    Q_UNUSED(args);
    setPath("/RPC2");
}

rTorrentConfig::~rTorrentConfig()
{
}

QString rTorrentConfig::path() const
{
    return m_path;
}

void rTorrentConfig::setPath(const QString & path)
{
    m_path = path;
}

QByteArray rTorrentConfig::data() const
{
    QVariantMap config;
    config.insert("info", "trickle_rtorrent 0.1");
    config.insert("path", m_path);

    QByteArray configData;
    QDataStream stream(configData);
    stream << config;
    return configData;
}

void rTorrentConfig::setData(const QByteArray & data)
{
    QDataStream stream(data);
    QVariantMap config;
    stream >> config;

    Q_ASSERT(config.value("info") == "trickle_rtorrent 0.1");
    setPath(config.value("path").toString());
}
