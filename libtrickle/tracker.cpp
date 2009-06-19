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
    Copyright (C) 2009, Michael Forney <michael@obberon.com>
 */

#include "tracker.h"

Tracker::Tracker()
{

}

Tracker::~Tracker()
{
}

int Tracker::id() const
{
    return m_id;
}

KUrl Tracker::url() const
{
    return m_url;
}

Tracker::Type Tracker::type() const
{
    return m_type;
}

bool Tracker::enabled() const
{
    return m_enabled;
}

bool Tracker::open() const
{
    return m_open;
}

int Tracker::announceInterval() const
{
    return m_announceInterval;
}

int Tracker::peersComplete() const
{
    return m_peersComplete;
}

int Tracker::peersIncomplete() const
{
    return m_peersIncomplete;
}

QDateTime Tracker::lastAnnounce() const
{
    return m_lastAnnounce;
}


void Tracker::setId(int id)
{
    m_id = id;
}

void Tracker::setUrl(const KUrl & url)
{
    m_url = url;
}

void Tracker::setType(Type type)
{
    m_type = type;
}

void Tracker::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

void Tracker::setOpen(bool open)
{
    m_open = open;
}

void Tracker::setAnnounceInterval(int announceInterval)
{
    m_announceInterval = announceInterval;
}

void Tracker::setPeersComplete(int peersComplete)
{
    m_peersComplete = peersComplete;
}

void Tracker::setPeersIncomplete(int peersIncomplete)
{
    m_peersIncomplete = peersIncomplete;
}

void Tracker::setLastAnnounce(const QDateTime & lastAnnounce)
{
    m_lastAnnounce = lastAnnounce;
}
