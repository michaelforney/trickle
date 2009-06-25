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
#ifndef BCODEC_H
#define BCODEC_H

/**
	@author Michael Forney <mforney@trickleproject.org>
*/

#include "trickle_export.h"

#include <QMetaType>
#include <QHash>

class QVariant;
class QByteArray;
class QBuffer;

typedef QHash<QVariant, QVariant> VariantHash;

Q_DECLARE_METATYPE(VariantHash);

uint TRICKLE_EXPORT qHash(const QVariant & variant);

class TRICKLE_EXPORT BCodec
{
	public:
		static QVariant decode(const QByteArray & data);
		static QByteArray encode(const QVariant & data);
	private:
		static QVariant decodeHelper(QBuffer * data);
};

#endif
