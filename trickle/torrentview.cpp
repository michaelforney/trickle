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
#include "torrentview.h"

#include <QHeaderView>
#include <KDebug>
#include <KGlobal>
#include <KConfig>
#include <KConfigGroup>

#include "torrentmodel.h"
#include "torrentsortmodel.h"

TorrentView::TorrentView()
 : QTreeView()
{
     setUniformRowHeights(true);
     setItemsExpandable(false);
     setAlternatingRowColors(true);
     setSortingEnabled(true);
     setIndentation(0);

     restore();

    connect(this, SIGNAL(activated(const QModelIndex &)), this, SLOT(setTorrentIndex(const QModelIndex &)));
}

TorrentView::~TorrentView()
{
}

void TorrentView::setTorrentIndex(const QModelIndex & proxyIndex)
{
    const QAbstractProxyModel * proxyModel = static_cast<const QAbstractProxyModel *>(proxyIndex.model());
    Q_ASSERT(proxyModel);
    TorrentModel * model = static_cast<const TorrentModel *>(proxyModel->sourceModel());
    Q_ASSERT(model);
    QModelIndex index = proxyModel->mapToSource(proxyIndex);
    emit torrentHashChanged(model->hash(index));
}

void TorrentView::save()
{
    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup group = config->group("TorrentView");
    group.writeEntry("State", header()->saveState());
}

void TorrentView::restore()
{
    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup group = config->group("TorrentView");
    header()->restoreState(group.readEntry("State", QByteArray()));
}

#include "torrentview.moc"
