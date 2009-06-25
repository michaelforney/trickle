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
#include "torrent.h"

#include <QRegExp>
#include <QDebug>

//#include "xmlrpc.h"
//#include "updatetimer.h"

Torrent::Torrent(const QString & hash)
{
	setHash(hash);
	setState(Stopped);
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

Torrent::Torrent()
{
}

Torrent::~Torrent()
{
}

bool Torrent::operator==(const Torrent & other)
{
	return hash() == other.hash();
}

/*void Torrent::stop()
{
	//XmlRpc::instance()->call("d.stop", QVariantList() << m_hash, this, "result");
}

void Torrent::start()
{
	//XmlRpc::instance()->call("d.start", QVariantList() << m_hash, this, "result");
}

void Torrent::update()
{
	QVariantList args;
	args << m_hash;
	args << "";
	args << "t.get_scrape_complete=";
	args << "t.get_scrape_incomplete=";
	//XmlRpc::instance()->call("t.multicall", args, this, "result");
}*/

//Data Get Functions

int Torrent::seedsConnected() const
{
	return m_seedsConnected;
}

int Torrent::seedsTotal() const
{
	return m_seedsTotal;
}

int Torrent::leechsConnected() const
{
	return m_leechsConnected;
}

int Torrent::leechsTotal() const
{
	return m_leechsTotal;
}

ByteSize Torrent::downloadRate() const
{
	return m_downloadRate;
}

ByteSize Torrent::uploadRate() const
{
	return m_uploadRate;
}

ByteSize Torrent::size() const
{
	return m_size;
}

ByteSize Torrent::uploaded() const
{
	return m_uploaded;
}

ByteSize Torrent::downloaded() const
{
	return m_downloaded;
}

QString Torrent::name() const
{
	return m_name;
}

QString Torrent::hash() const
{
	return m_hash;
}

int Torrent::completedChunks() const
{
	return m_completedChunks;
}

Torrent::Priority Torrent::priority() const
{
	return m_priority;
}

Torrent::TorrentState Torrent::state() const
{
    return m_state;
}

double Torrent::ratio() const
{
	return m_uploaded / m_downloaded;
}

ByteSize Torrent::chunkSize() const
{
	return m_chunkSize;
}

int Torrent::chunks() const
{
	return m_chunks;
}

/*void Torrent::result(const QString & method, const QVariant & result)
{
	if (method == "t.multicall")
	{
		QVariantList resultList = result.value<QVariantList>()[0].value<QVariantList>();
		setSeedsTotal(resultList[0].toInt());
		setLeechsTotal(resultList[1].toInt());
		emit dataChanged(this);
		emit updated();
	}
}*/
		
//Data Set Functions
		
void Torrent::setSeedsConnected(int seedsConnected)
{
	m_seedsConnected = seedsConnected;
}

void Torrent::setSeedsTotal(int seedsTotal)
{
	m_seedsTotal = seedsTotal;
}

void Torrent::setLeechsConnected(int leechsConnected)
{
	m_leechsConnected = leechsConnected;
}

void Torrent::setLeechsTotal(int leechsTotal)
{
	m_leechsTotal = leechsTotal;
}

void Torrent::setDownloadRate(ByteSize downloadRate)
{
	m_downloadRate = downloadRate;
}

void Torrent::setUploadRate(ByteSize uploadRate)
{
	m_uploadRate = uploadRate;
}

void Torrent::setChunks(int chunks)
{
	m_chunks = chunks;
}

void Torrent::setChunkSize(ByteSize chunkSize)
{
	m_chunkSize = chunkSize;
}

void Torrent::setUploaded(ByteSize uploaded)
{
	m_uploaded = uploaded;
}

void Torrent::setDownloaded(ByteSize downloaded)
{
	m_downloaded = downloaded;
}

void Torrent::setName(const QString & name)
{
	m_name = name;
}

void Torrent::setSize(ByteSize size)
{
	m_size = size;
}

void Torrent::setHash(const QString & hash)
{
	m_hash = hash;
}

void Torrent::setState(TorrentState state)
{
	m_state = state;
}

/*void Torrent::setRatio(double ratio)
{
	m_ratio = ratio;
}*/

void Torrent::setCompletedChunks(int completedChunks)
{
	m_completedChunks = completedChunks;
}

void Torrent::setPriority(Priority priority)
{
	m_priority = priority;
}

//#include "torrent.moc"
