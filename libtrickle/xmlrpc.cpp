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
#include "xmlrpc.h"

#include <QBuffer>
#include <QDebug>
#include <QFile>
#include <QDomDocument>

#include "server.h"

XmlRpc::XmlRpc()
: QObject()
{
}

XmlRpc::~XmlRpc()
{
}


QVariant toVariant(const QDomElement & value)
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

QDomElement toElement(const QVariant & value, QDomDocument document)
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

void XmlRpc::call(QString method, QList<QVariant> args)
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
    url.setHost(m_server->host());
    url.setPort(m_server->port());
    url.setPath(m_path);
    
    QByteArray requestData = document.toByteArray();
    KIO::StoredTransferJob * job = KIO::storedHttpPost(requestData, url, KIO::HideProgressInfo);
    job->addMetaData("content-type", "text/xml");
    job->addMetaData("User-Agent", "Trickle/0.01");
    methods.insert(job, method);
    connect(job, SIGNAL(finished(KJob *)), this, SLOT(jobFinished(KJob *)));
}

QString XmlRpc::host() const
{
    return m_server->host();
}

QString XmlRpc::path() const
{
    return m_path;
}

int XmlRpc::port() const
{
    return m_server->port();
}

void XmlRpc::jobFinished(KJob * job)
{
    KIO::StoredTransferJob * transferJob = qobject_cast<KIO::StoredTransferJob *>(job);
    if (transferJob)
    {
        QByteArray data = transferJob->data();
        QDomDocument document;
        document.setContent(data);
        
        QDomElement docElement = document.documentElement();
        QDomElement paramElement = docElement.firstChildElement("params").firstChildElement("param");
        QDomElement valueElement = paramElement.firstChildElement("value");
        
        if (valueElement.isNull())
        {
            qDebug() << methods.value(transferJob);
            qDebug() << document.toString();
        }
        QVariant resultVariant = toVariant(valueElement);
        emit result(methods.value(transferJob), resultVariant);
        methods.remove(transferJob);
    }
}

int XmlRpc::requestCount() const
{
    return methods.size();
}

void XmlRpc::setServer(Server * server, const QString & path)
{
    m_server = server;
    m_path = path;
}

bool XmlRpc::isBusy() const
{
    return methods.size() > 0;
}

#include "xmlrpc.moc"
