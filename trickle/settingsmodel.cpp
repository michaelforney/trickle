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
#include "settingsmodel.h"

#include "settingspage.h"

SettingsModel::SettingsModel()
 : QAbstractListModel()
{
}

SettingsModel::~SettingsModel()
{
}

QVariant SettingsModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}
	
	if (role == Qt::DisplayRole)
	{
		return pages[index.row()]->title();
	}
	else if (role == Qt::DecorationRole)
	{
		return pages[index.row()]->icon();
	}
	else
	{
		return QVariant();
	}
}

int SettingsModel::rowCount(const QModelIndex & parent) const
{
	if (parent.isValid())
	{
		return 0;
	}
	
	return pages.size();
}

void SettingsModel::addPage(SettingsPage * page)
{
	pages.append(page);
	emit layoutChanged();
}
