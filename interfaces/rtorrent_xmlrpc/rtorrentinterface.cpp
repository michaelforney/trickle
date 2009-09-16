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
#include "rtorrentinterface.h"

#include "rtorrentconfig.h"
#include "rtorrentconfigwidget.h"
#include "xmlrpc.h"
#include "bytesize.h"
#include "logdata.h"

#include <KGenericFactory>

K_PLUGIN_FACTORY(rTorrentInterfaceFactory,
                  registerPlugin<rTorrentInterface>();
                  registerPlugin<rTorrentConfig>();
                  registerPlugin<rTorrentConfigWidget>();)
K_EXPORT_PLUGIN(rTorrentInterfaceFactory("trickle_rtorrent"))

rTorrentInterface::rTorrentInterface(QObject * parent, const QVariantList & /*args*/)
 : Interface(parent)
{
     setConfig(new rTorrentConfig(this, QVariantList()));
}

rTorrentInterface::~rTorrentInterface()
{
}

void rTorrentInterface::setDownloadLimit(const ByteSize & size)
{
    Q_UNUSED(size); //TODO: Remove when implemented
    //xmlRpc->call("set_download_rate", QVariantList() << size.bytes());
}

void rTorrentInterface::setUploadLimit(const ByteSize & size)
{
    Q_UNUSED(size); //TODO: Remove when implemented
    //xmlRpc->call("set_upload_rate", QVariantList() << size.bytes());
}

void rTorrentInterface::startTorrent(const QString & hash)
{
    Q_UNUSED(hash); //TODO: Remove when implemented
}

void rTorrentInterface::stopTorrent(const QString & hash)
{
    Q_UNUSED(hash); //TODO: Remove when implemented
}

bool rTorrentInterface::connectToServer()
{
    //xmlRpc->setServer(server());
    return true;
}

void rTorrentInterface::clear()
{
}

KIO::StoredTransferJob * rTorrentInterface::call(const QString & method, const QVariantList & args)
{
    QDomDocument document("methodCall");
    QDomElement documentElement = document.createElement("methodCall");
    document.appendChild(documentElement);
    QDomElement methodElement = document.createElement("methodName");
    methodElement.appendChild(document.createTextNode(method));
    documentElement.appendChild(methodElement);
    QDomElement paramsElement = document.createElement("params");
    foreach(QVariant arg, args)
    {
        QDomElement paramElement = document.createElement("param");
        QDomElement valueElement = toElement(arg, document);
        paramElement.appendChild(valueElement);
        paramsElement.appendChild(paramElement);
    }
    documentElement.appendChild(paramsElement);

    KUrl url;
    url.setScheme("http");
    url.setHost(server().host());
    url.setPort(server().port());
    url.setPath(config()->path());

    //kDebug() << url;

    QByteArray requestData = document.toByteArray();
    KIO::StoredTransferJob * job = KIO::storedHttpPost(requestData, url, KIO::HideProgressInfo);
    job->addMetaData("content-type", "Content-Type: text/xml");
    job->addMetaData("user-agent", "User-Agent: Trickle/0.01");
    return job;
}

void rTorrentInterface::updateTorrentList()
{
    kDebug() << "updateTorrentList()";

    LogData::self()->logMessage("Updating torrent list");
    KIO::StoredTransferJob * job = call("d.multicall", QVariantList() <<
        "" <<
        "d.get_hash=" <<
        "d.get_name=" <<
        "d.get_state=" <<
        "d.get_complete=" <<
        "d.get_size_bytes=" <<
        "d.get_peers_accounted=" <<
        "d.get_peers_complete=" <<
        "d.get_down_rate=" <<
        "d.get_completed_bytes=" <<
        "d.get_up_rate=" <<
        "d.get_up_total=" <<
        "d.get_priority=" <<
        //"d.get_completed_chunks=" <<
        "d.get_size_chunks=" <<
        "d.get_chunk_size=" <<
        "d.get_bitfield=");
    jobs.insert(job, qMakePair(TorrentList, QVariantList()));
    connect(job, SIGNAL(finished(KJob *)), this, SLOT(jobFinished(KJob *)));
}

void rTorrentInterface::updateFileInfo(const QString & hash)
{
    KIO::StoredTransferJob * job = call("f.multicall", QVariantList() <<
        hash <<
        "" <<
        "f.get_path=" <<
        //"f.get_path_components=" <<
        "f.get_size_bytes=" <<
        "f.get_completed_chunks=" <<
        "f.get_priority=");
    jobs.insert(job, qMakePair(FileList, QVariantList() << hash));
    connect(job, SIGNAL(finished(KJob *)), this, SLOT(jobFinished(KJob *)));
}

void rTorrentInterface::updatePeerInfo(const QString & hash)
{
    Q_UNUSED(hash); //TODO: Remove when implemented
}

void rTorrentInterface::updateTrackerInfo(const QString & hash)
{
    kDebug() << "Tracker job starting";
    KIO::StoredTransferJob * job = call("t.multicall", QVariantList() <<
        hash <<
        "" <<
        "t.get_id=" <<
        "t.get_url=" <<
        "t.is_open=" <<
        "t.is_enabled=" <<
        "t.get_type=" <<
        "t.get_scrape_incomplete=" <<
        "t.get_scrape_complete=" <<
        "t.get_scrape_time_last=" <<
        "t.get_normal_interval=");
    jobs.insert(job, qMakePair(TrackerList, QVariantList() << hash));
    connect(job, SIGNAL(finished(KJob *)), this, SLOT(jobFinished(KJob *)));
}

void rTorrentInterface::setPriority(const QString & hash, Torrent::Priority priority)
{
    LogData::self()->logMessage(QString("Changing priority of torrent with hash: %1 to %2").arg(hash).arg(priority));
    KIO::StoredTransferJob * job = call("d.set_priority", QVariantList() <<
        hash <<
        priority);
    jobs.insert(job, qMakePair(SetTorrentPriority, QVariantList() << hash));
    connect(job, SIGNAL(finished(KJob *)), this, SLOT(jobFinished(KJob *)));
}

void rTorrentInterface::jobFinished(KJob * job)
{
    KIO::StoredTransferJob * transferJob = qobject_cast<KIO::StoredTransferJob *>(job);
    if (transferJob)
    {
        if (transferJob->error())
        {
            kDebug() << "ERROR!!! Run for your lives!!!";
            kDebug() << transferJob->errorString();
            return;
        }

        if (jobs.contains(transferJob))
        {
            rTorrentRequest requestType = jobs.value(transferJob).first;
            switch(requestType)
            {
                case TorrentList:
                {
                    kDebug() << "Torrent job finished";
                    LogData::self()->logMessage("Torrent list update finished");
                    //kDebug() << transferJob->data();
                    //kDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!FINDME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
                    QDomDocument document;
                    document.setContent(transferJob->data());
                    //kDebug() << document.toString();
                    QVariant result = toVariant(document.documentElement().firstChildElement("params").firstChildElement("param").firstChildElement("value"));
                    QVariantList torrents = result.toList();

                    QMap<QString, Torrent> torrentMap;
                    foreach(QVariant torrentVariant, torrents)
                    {
                        QVariantList torrentAttributes = torrentVariant.toList();
                        Torrent torrent(torrentAttributes.takeFirst().toString());
                        torrent.setName(torrentAttributes.takeFirst().toString());
                        bool started = torrentAttributes.takeFirst().toBool();
                        bool complete = torrentAttributes.takeFirst().toBool();
                        //kDebug() << torrent.name() << ", " << started << ", " << complete;
                        Torrent::TorrentState state;
                        if (started)
                        {
                            if (complete)
                            {
                                state = Torrent::Seeding;
                            }
                            else
                            {
                                state = Torrent::Downloading;
                            }
                        }
                        else
                        {
                            if (complete)
                            {
                                state = Torrent::Completed;
                            }
                            else
                            {
                                state = Torrent::Stopped;
                            }
                        }
                        torrent.setState(state);
                        //kDebug() << ByteSize(torrentAttributes.at(3).toLongLong()).toString();
                        torrent.setSize(ByteSize(torrentAttributes.takeFirst().toLongLong()));
                        torrent.setLeechsConnected(torrentAttributes.takeFirst().toLongLong());
                        torrent.setSeedsConnected(torrentAttributes.takeFirst().toLongLong());
                        torrent.setDownloadRate(ByteSize(torrentAttributes.takeFirst().toLongLong()));
                        torrent.setDownloaded(ByteSize(torrentAttributes.takeFirst().toLongLong()));
                        torrent.setUploadRate(ByteSize(torrentAttributes.takeFirst().toLongLong()));
                        torrent.setUploaded(ByteSize(torrentAttributes.takeFirst().toLongLong()));
                        int priorityInt = torrentAttributes.takeFirst().toInt();
                        Torrent::Priority priority;
                        switch(priorityInt)
                        {
                            case 0:
                            {
                                priority = Torrent::Off;
                                break;
                            }
                            case 1:
                            {
                                priority = Torrent::Low;
                                break;
                            }
                            case 2:
                            {
                                priority = Torrent::Medium;
                                break;
                            }
                            case 3:
                            {
                                priority = Torrent::High;
                                break;
                            }
                            default:
                            {
                                priority = Torrent::Off;
                            }
                        }
                        torrent.setPriority(priority);
                        //torrent.setCompletedChunks(torrentAttributes.takeFirst().toLongLong());
                        torrent.setChunks(torrentAttributes.takeFirst().toLongLong());
                        torrent.setChunkSize(torrentAttributes.takeFirst().toLongLong());
                        QByteArray byteField = QByteArray::fromHex(torrentAttributes.takeFirst().toByteArray());
                        //kDebug() << byteField.toHex();
                        QBitArray bitField(torrent.chunks());
                        for (int i = 0; i < byteField.size(); i++)
                        {
                            for (int bit = 0; bit < 8; bit++)
                            {
                                if (byteField.at(i) & (1 << (7 - bit)))
                                {
                                    int bitIndex = (i * 8) + bit;
                                    if (bitIndex >= 0 && bitIndex < bitField.size())
                                    {
                                        bitField.setBit(bitIndex);
                                    }
                                }
                            }
                        }
                        torrent.setBitField(bitField);
                        //kDebug() << torrent.hash() << ", " << torrent.name();
                        torrentMap.insert(torrent.hash(), torrent);
                        if (watchedTorrents().contains(torrent.hash()))
                        {
                            emit watchedTorrentUpdated(torrent);
                        }
                    }
                    kDebug() << "emitting";
                    emit torrentsUpdated(torrentMap);
                    jobs.remove(transferJob);
                    break;
                }
                case FileList:
                {
                    kDebug() << "File job finished";
                    QDomDocument document;
                    document.setContent(transferJob->data());
                    //kDebug() << document.toString();
                    QVariant result = toVariant(document.documentElement().firstChildElement("params").firstChildElement("param").firstChildElement("value"));
                    QVariantList fileVariants = result.toList();
                    //kDebug() << fileVariants;
                    QMap<QString, File> files;
                    foreach(QVariant fileVariant, fileVariants)
                    {
                        QVariantList fileAttributes = fileVariant.toList();
                        File file;
                        file.setPath(fileAttributes.takeFirst().toString());
                        file.setSize(ByteSize(fileAttributes.takeFirst().toLongLong()));
                        file.setCompletedChunks(fileAttributes.takeFirst().toLongLong());
                        files.insert(file.path(), file);
                    }
                    kDebug() << "emitting files updated";
                    emit filesUpdated(jobs.value(transferJob).second.at(0).toString(), files);
                    break;
                }
                case TrackerList:
                {
                    kDebug() << "Tracker job finished";
                    QDomDocument document;
                    document.setContent(transferJob->data());
                    //kDebug() << document.toString();
                    QVariant result = toVariant(document.documentElement().firstChildElement("params").firstChildElement("param").firstChildElement("value"));
                    QVariantList trackerVariants = result.toList();
                    QMap<int, Tracker> trackers;
                    foreach(QVariant trackerVariant, trackerVariants)
                    {
                        QVariantList trackerAttributes = trackerVariant.toList();
                        Tracker tracker;
                        tracker.setId(trackerAttributes.takeFirst().toInt());
                        tracker.setUrl(KUrl(trackerAttributes.takeFirst().toString()));
                        tracker.setOpen(trackerAttributes.takeFirst().toBool());
                        tracker.setEnabled(trackerAttributes.takeFirst().toBool());
                        int typeInt = trackerAttributes.takeFirst().toInt();
                        Tracker::Type type;
                        switch (typeInt)
                        {
                            case 0:
                            {
                                type = Tracker::Http;
                                break;
                            }
                            case 1:
                            {
                                type = Tracker::Udp;
                                break;
                            }
                            case 2:
                            {
                                type = Tracker::Dht;
                                break;
                            }
                        }
                        tracker.setType(type);
                        tracker.setPeersIncomplete(trackerAttributes.takeFirst().toInt());
                        tracker.setPeersComplete(trackerAttributes.takeFirst().toInt());
                        tracker.setLastAnnounce(QDateTime::fromTime_t(trackerAttributes.takeFirst().toLongLong()));
                        tracker.setAnnounceInterval(trackerAttributes.takeFirst().toInt());
                        trackers.insert(tracker.id(), tracker);
                    }
                    emit trackersUpdated(jobs.value(transferJob).second.at(0).toString(), trackers);
                    break;
                }
                case SetTorrentPriority:
                {
                    QDomDocument document;
                    document.setContent(transferJob->data());
                    kDebug() << document.toString();
                    //QVariant resultVariant = toVariant(document.documentElement().firstChildElement("params").firstChildElement("param").firstChildElement("value"));
                }
                default:
                {
                    kDebug() << "Unknown request type: " << requestType;
                }
            }
        }
    }
}

QVariant rTorrentInterface::toVariant(const QDomElement & value)
{
    if (value.isNull())
    {
        return QVariant();
    }

    QDomElement element = value.firstChildElement();
    QString type = element.tagName();
    //kDebug() << type;
    if (type == "array")
    {
        QVariantList array;
        QDomElement dataElement = element.firstChildElement("data");
        QDomElement child = dataElement.firstChildElement();
        while (!child.isNull())
        {
            array.append(toVariant(child));
            child = child.nextSiblingElement();
        }
        return array;
    }
    else if (type == "string")
    {
        return element.text();
    }
    else if (type == "i4")
    {
        return element.text().toInt();
    }
    else if (type == "i8")
    {
        return element.text().toLongLong();
    }
    else
    {
        qDebug() << "Unknown type: " << type;
        return QVariant();
    }
}

QDomElement rTorrentInterface::toElement(const QVariant & value, QDomDocument document)
{
    QDomElement valueElement = document.createElement("value");
    if (value.type() == QVariant::String)
    {
        QDomElement stringElement = document.createElement("string");
        stringElement.appendChild(document.createTextNode(value.toString()));
        valueElement.appendChild(stringElement);
    }
    else if (value.type() == QVariant::Int)
    {
        QDomElement intElement = document.createElement("i4");
        intElement.appendChild(document.createTextNode(QString::number(value.toInt())));
        valueElement.appendChild(intElement);
    }
    else if (value.type() == QVariant::LongLong)
    {
        QDomElement intElement = document.createElement("i8");
        intElement.appendChild(document.createTextNode(QString::number(value.toLongLong())));
        valueElement.appendChild(intElement);
    }
    else if (value.type() == QVariant::List)
    {
        QDomElement listElement = document.createElement("array");
        QDomElement dataElement = document.createElement("data");
        foreach (QVariant arg, value.toList())
        {
            dataElement.appendChild(toElement(arg, document));
        }
        listElement.appendChild(dataElement);
        valueElement.appendChild(listElement);
    }
    return valueElement;
}

rTorrentConfig * rTorrentInterface::config() const
{
    return static_cast<rTorrentConfig *>(genericConfig());
}

void rTorrentInterface::setConfig(InterfaceConfig * rconfig)
{
    if (!rconfig || config() == rconfig)
    {
        return;
    }

    if (config())
    {
        config()->deleteLater();
    }

    Interface::setConfig(rconfig);
    reset();
}

#include "rtorrentinterface.moc"
