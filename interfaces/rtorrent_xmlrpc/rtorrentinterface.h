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
#ifndef RTORRENTINTERFACE_H
#define RTORRENTINTERFACE_H

#include "interface.h"

/**
    @author Michael Forney <mforney@trickleproject.org>
*/

#include "torrent.h"

#include <KIO/Job>
#include <QVariantList>
#include <QDomDocument>

class ByteSize;
class XmlRpc;
class rTorrentConfig;

class rTorrentInterface : public Interface
{
    Q_OBJECT
    public:
        enum rTorrentRequest { TorrentList, FileList, TrackerList, SetTorrentPriority };
        rTorrentInterface(QObject * parent, const QVariantList & args);
        ~rTorrentInterface();
        QString title() const { return "rTorrent XmlRpc Interface"; }
        QString description() const { return "Interface to connect to rTorrent through XmlRpc"; }
    public slots:
        void setDownloadLimit(const ByteSize & size);
        void setUploadLimit(const ByteSize & size);

        void startTorrent(const QString & hash);
        void stopTorrent(const QString & hash);

        void setPriority(const QString & hash, Torrent::Priority priority);

        bool connectToServer();
        void clear();
    protected slots:
        void updateTorrentList();
        void updateFileInfo(const QString & hash);
        void updatePeerInfo(const QString & hash);
        void updateTrackerInfo(const QString & hash);

        void jobFinished(KJob * job);
        void setConfig(InterfaceConfig * config);
    protected:
        KIO::StoredTransferJob * call(const QString & method, const QVariantList & args);
        QVariant toVariant(const QDomElement & value);
        QDomElement toElement(const QVariant & value, QDomDocument document);
        rTorrentConfig * config() const;
    private:
        QMap<KIO::StoredTransferJob *, QPair<rTorrentRequest, QVariantList> > jobs;
};

#endif
