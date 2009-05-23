/***************************************************************************
 *   Copyright (C) 2007 by Michael Forney                                  *
 *   michael@obberon.com                                                   *
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

void rTorrentInterface::update()
{
}

bool rTorrentInterface::connectToServer()
{
	//xmlRpc->setServer(server());
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

    kDebug() << url;
    
    QByteArray requestData = document.toByteArray();
    kDebug() << requestData;
    KIO::StoredTransferJob * job = KIO::storedHttpPost(requestData, url, KIO::HideProgressInfo);
    job->addMetaData("content-type", "Content-Type: text/xml");
    job->addMetaData("user-agent", "User-Agent: Trickle/0.01");
    return job;
}

void rTorrentInterface::updateTorrentList()
{
    kDebug() << "updateTorrentList()";
    KIO::StoredTransferJob * job = call("d.multicall", QVariantList() <<
        "" <<
        "d.get_hash=" <<
        "d.get_name=");
    jobs.insert(job, TorrentList);
    connect(job, SIGNAL(finished(KJob *)), this, SLOT(jobFinished(KJob *)));
}

void rTorrentInterface::updateFileList(const QString & hash)
{
    Q_UNUSED(hash); //TODO: Remove when implemented
}

void rTorrentInterface::jobFinished(KJob * job)
{
    KIO::StoredTransferJob * transferJob = qobject_cast<KIO::StoredTransferJob *>(job);
    kdDebug() << "Job finished";
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
            rTorrentRequest requestType = jobs.value(transferJob);
            switch(requestType)
            {
                case TorrentList:
                {
                    kDebug() << transferJob->data();
                    kDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!FINDME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
                    /*QVariantMap response = CSVCodec::decode(transferJob->data()).toMap();
                    QVariantList torrents = response.value("torrents").toList();
                    QMap<QString, Torrent> torrentMap;
                    foreach(QVariant torrentVariant, torrents)
                    {
                        QVariantList webuiTorrent = torrentVariant.toList();
                        //qDebug() << webuiTorrent;
                        Torrent torrent(webuiTorrent.at(0).toString());
                        Torrent::TorrentState state;
                        WebUiStates webUiState(webuiTorrent.at(1).toInt());
                        qDebug() << webUiState;
                        qDebug() << webuiTorrent.at(4).toInt();
                        if (webUiState & Started && webuiTorrent.at(4).toInt() == 1000)
                        {
                            state = Torrent::Seeding;
                        }
                        else if (webUiState & Started && webuiTorrent.at(4).toInt() < 1000)
                        {
                            state = Torrent::Downloading;
                        }
                        else if (!webUiState & Started && webuiTorrent.at(4).toInt() == 1000)
                        {
                            state = Torrent::Completed;
                        }
                        else if (!webUiState & Started && webuiTorrent.at(4).toInt() < 1000)
                        {
                            state = Torrent::Stopped;
                        }
                        qDebug() << state;
                        torrent.setState(state);
                        torrent.setName(webuiTorrent.at(2).toString());
                        torrent.setSize(webuiTorrent.at(3).toLongLong());
                        torrent.setDownloaded(webuiTorrent.at(5).toLongLong());
                        torrent.setUploaded(webuiTorrent.at(6).toLongLong());
                        torrentMap.insert(webuiTorrent.at(0).toString(), torrent);
                    }
                    qDebug() << "emitting";
                    emit torrentMapUpdated(torrentMap);*/
                    jobs.remove(transferJob);
                    break;
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
