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

#ifndef RTORRENTCONFIG_H
#define RTORRENTCONFIG_H

#include "interfaceconfig.h"

#include <QVariantList>

class rTorrentConfig : public InterfaceConfig
{
    Q_OBJECT
    public:
        rTorrentConfig(QObject * parent, const QVariantList & args);
        rTorrentConfig();
        virtual ~rTorrentConfig();

        QString path() const;
        void setPath(const QString & path);

        QByteArray data() const;
        void setData(const QByteArray & data);
    private:
        QString m_path;
};

#endif // RTORRENTCONFIG_H