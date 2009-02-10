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
#include "xmlrpcrequest.h"

#include <QBuffer>

XmlRpcRequest::XmlRpcRequest(const QString & method)
{
	setMethod(method);
	m_buffer = new QBuffer();
	m_buffer->open(QIODevice::ReadWrite);
}

XmlRpcRequest::~XmlRpcRequest()
{
	//delete m_buffer;
}

void XmlRpcRequest::setMethod(const QString & method)
{
	m_method = method;
}

QString XmlRpcRequest::method() const
{
	return m_method;
}
QBuffer * XmlRpcRequest::buffer() const
{
	return m_buffer;
}



