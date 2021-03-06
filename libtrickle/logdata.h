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

#ifndef LOGDATA_H
#define LOGDATA_H

#include <QObject>

#include "trickle_export.h"
#include "logentry.h"

class TRICKLE_EXPORT LogData : public QObject
{
    Q_OBJECT
    public:
        LogData(QObject * parent = 0);
        virtual ~LogData();

        static LogData * self();

        QList<LogEntry> history() const;
    public slots:
        void logMessage(const QString & message);
    signals:
        void entry(const LogEntry & logEntry);
    private:
        QList<LogEntry> logEntries;
};

#endif // LOGDATA_H