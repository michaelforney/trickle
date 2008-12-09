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
#include "torrentitem.h"

#include <QRegExp>
#include <QDebug>

#include "xmlrpc.h"
#include "updatetimer.h"

TorrentItem::TorrentItem(const QString & hash)
 : QObject()
{
	setHash(hash);
	setState(Closed);
	setSeedsConnected(0);
	setSeedsTotal(0);
	setLeechsConnected(0);
	setLeechsTotal(0);
	setDownloadRate(0);
	setUploadRate(0);
	setChunkSize(0);
	setCompletedChunks(0);
	setChunks(0);
	setUploaded(0);
	setDownloaded(0);
}


TorrentItem::~TorrentItem()
{
}

bool TorrentItem::operator==(const TorrentItem & other)
{
	return hash() == other.hash();
}

void TorrentItem::stop()
{
	//XmlRpc::instance()->call("d.stop", QVariantList() << m_hash, this, "result");
}

void TorrentItem::start()
{
	//XmlRpc::instance()->call("d.start", QVariantList() << m_hash, this, "result");
}

void TorrentItem::update()
{
	QVariantList args;
	args << m_hash;
	args << "";
	args << "t.get_scrape_complete=";
	args << "t.get_scrape_incomplete=";
	//XmlRpc::instance()->call("t.multicall", args, this, "result");
}

//Data Get Functions

int TorrentItem::seedsConnected() const
{
	return m_seedsConnected;
}

int TorrentItem::seedsTotal() const
{
	return m_seedsTotal;
}

int TorrentItem::leechsConnected() const
{
	return m_leechsConnected;
}

int TorrentItem::leechsTotal() const
{
	return m_leechsTotal;
}

ByteSize TorrentItem::downloadRate() const
{
	return m_downloadRate;
}

ByteSize TorrentItem::uploadRate() const
{
	return m_uploadRate;
}

ByteSize TorrentItem::size() const
{
	return m_size;
}

ByteSize TorrentItem::uploaded() const
{
	return m_uploaded;
}

ByteSize TorrentItem::downloaded() const
{
	return m_downloaded;
}

QString TorrentItem::name() const
{
	return m_name;
}

QString TorrentItem::hash() const
{
	return m_hash;
}

int TorrentItem::completedChunks() const
{
	return m_completedChunks;
}

int TorrentItem::priority() const
{
	return m_priority;
}

TorrentItem::TorrentState TorrentItem::state() const
{
	if (m_state == Open)
	{
		if (completedChunks() == chunks())
		{
			return Seeding;
		}
		else
		{
			return Downloading;
		}
	}
	else
	{
		if (completedChunks() == chunks())
		{
			return Completed;
		}
		else
		{
			return Stopped;
		}
	}
}

double TorrentItem::ratio() const
{
	return m_ratio;
}

ByteSize TorrentItem::chunkSize() const
{
	return m_chunkSize;
}

int TorrentItem::chunks() const
{
	return m_chunks;
}

void TorrentItem::result(const QString & method, const QVariant & result)
{
	if (method == "t.multicall")
	{
		QVariantList resultList = result.value<QVariantList>()[0].value<QVariantList>();
		setSeedsTotal(resultList[0].toInt());
		setLeechsTotal(resultList[1].toInt());
		emit dataChanged(this);
		emit updated();
	}
}
		
//Data Set Functions
		
void TorrentItem::setSeedsConnected(int seedsConnected)
{
	m_seedsConnected = seedsConnected;
}

void TorrentItem::setSeedsTotal(int seedsTotal)
{
	m_seedsTotal = seedsTotal;
}

void TorrentItem::setLeechsConnected(int leechsConnected)
{
	m_leechsConnected = leechsConnected;
}

void TorrentItem::setLeechsTotal(int leechsTotal)
{
	m_leechsTotal = leechsTotal;
}

void TorrentItem::setDownloadRate(ByteSize downloadRate)
{
	m_downloadRate = downloadRate;
}

void TorrentItem::setUploadRate(ByteSize uploadRate)
{
	m_uploadRate = uploadRate;
}

void TorrentItem::setChunks(int chunks)
{
	m_chunks = chunks;
}

void TorrentItem::setChunkSize(ByteSize chunkSize)
{
	m_chunkSize = chunkSize;
}

void TorrentItem::setUploaded(ByteSize uploaded)
{
	m_uploaded = uploaded;
}

void TorrentItem::setDownloaded(ByteSize downloaded)
{
	m_downloaded = downloaded;
}

void TorrentItem::setName(const QString & name)
{
	m_name = name;
}

void TorrentItem::setSize(ByteSize size)
{
	m_size = size;
}

void TorrentItem::setHash(const QString & hash)
{
	m_hash = hash;
}

void TorrentItem::setState(int state)
{
	m_state = state;
}

void TorrentItem::setRatio(double ratio)
{
	m_ratio = ratio;
}

void TorrentItem::setCompletedChunks(int completedChunks)
{
	m_completedChunks = completedChunks;
}

void TorrentItem::setPriority(int priority)
{
	m_priority = priority;
}

#include "torrentitem.moc"
