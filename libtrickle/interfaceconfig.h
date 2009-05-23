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

#ifndef INTERFACECONFIG_H
#define INTERFACECONFIG_H

#include "trickle_export.h"

#include <QObject>

class TRICKLE_EXPORT InterfaceConfig : public QObject
{
    Q_OBJECT
    public:
        InterfaceConfig(QObject * parent = 0);
        virtual ~InterfaceConfig();

        virtual QByteArray data() const = 0;
        virtual void setData(const QByteArray & data) = 0;
};

#endif // INTERFACECONFIG_H