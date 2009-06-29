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
#include "filedelegate.h"

#include <QModelIndex>
#include <QComboBox>
#include <QDebug>

#include "filemodel.h"
#include "filemodelitem.h"
#include "fileitem.h"

FileDelegate::FileDelegate()
 : QItemDelegate()
{
}


FileDelegate::~FileDelegate()
{
}

QWidget * FileDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & styleOption, const QModelIndex & index) const
{
    Q_UNUSED(styleOption);
	if (index.column() == FileModel::Priority)
	{
		QComboBox * prioritySelector = new QComboBox(parent);
		prioritySelector->addItems(QStringList() << "Skip" << "Normal" << "High");
		return prioritySelector;
	}
	else
	{
		return 0;
	}
}

void FileDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
	model->setData(index, static_cast<QComboBox *>(editor)->currentIndex());
}

void FileDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	FileModelItem * item = static_cast<FileModelItem *>(index.internalPointer());
	if (item->isFile())
	{
		static_cast<QComboBox *>(editor)->setCurrentIndex(item->toFile()->priority());
	}
}


