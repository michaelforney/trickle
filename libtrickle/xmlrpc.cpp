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

#include "xmlrpcrequest.h"
#include "server.h"

XmlRpc::XmlRpc()
 : QObject()
{
	http = new QHttp();
	connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(requestFinished(int)));
	connect(http, SIGNAL(authenticationRequired(QString, quint16, QAuthenticator *)), this, SLOT(authentication(QString, quint16, QAuthenticator *)));
	authenticating = false;
}

XmlRpc::~XmlRpc()
{
	http->close();
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
	
	QByteArray requestData = document.toByteArray();
	QHttpRequestHeader header("POST", m_path);
	header.setContentType("text/xml");
	header.setContentLength(requestData.size());
	header.setValue("Host", m_server->host());
	header.setValue("User-Agent", "Trickle/0.01");
	
	XmlRpcRequest * request = new XmlRpcRequest(method);
	
	int id = http->request(header, requestData, request->buffer());
	httpRequests.insert(id, request);
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

void XmlRpc::requestFinished(int id)
{
	if (httpRequests.contains(id))
	{
		XmlRpcRequest * request = httpRequests[id];
		request->buffer()->seek(0);
		QDomDocument document;
		document.setContent(request->buffer());
		request->buffer()->close();
		
		QDomElement docElement = document.documentElement();
		QDomElement paramElement = docElement.firstChildElement("params").firstChildElement("param");
		QDomElement valueElement = paramElement.firstChildElement("value");

		if (valueElement.isNull())
		{
			qDebug() << request->method();
			qDebug() << document.toString();
		}
		QVariant resultVariant = toVariant(valueElement);
		httpRequests.remove(id);
		emit result(request->method(), resultVariant);
	}
}

int XmlRpc::requests() const
{
	return httpRequests.size();
}

void XmlRpc::setServer(Server * server, const QString & path, QHttp::ConnectionMode mode)
{
	m_server = server;
	m_path = path;
	http->setHost(host(), mode, port());
}

bool XmlRpc::isBusy() const
{
	return httpRequests.size() > 0;
}

bool XmlRpc::isAuthenticating() const
{
	return authenticating;
}

void XmlRpc::doneAuthenticating()
{
	authenticating = false;
}

void XmlRpc::authentication(QString, quint16, QAuthenticator * authenticator)
{
	qDebug("authentication");
	authenticating = true;
	emit authenticate(authenticator);
}

#include "xmlrpc.moc"
