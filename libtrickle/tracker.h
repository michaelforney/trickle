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

#ifndef TRACKER_H
#define TRACKER_H

#include "trickle_export.h"

#include <KUrl>
#include <QDateTime>

class TRICKLE_EXPORT Tracker
{
    public:
        enum Type { Http, Udp, Dht };

        Tracker();
        virtual ~Tracker();

        int id() const;
        KUrl url() const;
        Type type() const;
        bool enabled() const;
        bool open() const;
        int announceInterval() const;
        int peersComplete() const;
        int peersIncomplete() const;
        QDateTime lastAnnounce() const;

        void setId(int id);
        void setUrl(const KUrl & url);
        void setType(Type type);
        void setEnabled(bool enabled);
        void setOpen(bool open);
        void setAnnounceInterval(int announceInterval);
        void setPeersComplete(int peersComplete);
        void setPeersIncomplete(int peersIncomplete);
        void setLastAnnounce(const QDateTime & lastAnnounce);
    private:
        int m_id;
        KUrl m_url;
        Type m_type;
        bool m_enabled;
        bool m_open;
        int m_announceInterval;
        int m_peersComplete;
        int m_peersIncomplete;
        QDateTime m_lastAnnounce;
};

#endif // TRACKER_H