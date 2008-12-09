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
#ifndef TORRENTITEM_H
#define TORRENTITEM_H

#include <QObject>

/**
	@author Michael Forney <michael@obberon.com>
*/

#include <QVariant>

#include "bytesize.h"

class TorrentItem : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int seedsConnected READ seedsConnected WRITE setSeedsConnected)
	Q_PROPERTY(int seedsTotal READ seedsTotal WRITE setSeedsTotal)
	Q_PROPERTY(int leechsConnected READ leechsConnected WRITE setLeechsConnected)
	Q_PROPERTY(int leechsTotal READ leechsTotal WRITE setLeechsTotal)
	Q_PROPERTY(ByteSize downloadRate READ downloadRate WRITE setDownloadRate)
	Q_PROPERTY(ByteSize uploadRate READ uploadRate WRITE setUploadRate)
	Q_PROPERTY(int chunks READ chunks WRITE setChunks)
	Q_PROPERTY(int completedChunks READ completedChunks WRITE setCompletedChunks)
	Q_PROPERTY(ByteSize chunkSize READ chunkSize WRITE setChunkSize)
	Q_PROPERTY(ByteSize size READ size WRITE setSize)
	Q_PROPERTY(ByteSize uploaded READ uploaded WRITE setUploaded)
	Q_PROPERTY(ByteSize downloaded READ downloaded WRITE setDownloaded)
	//Q_PROPERTY(TorrentState state READ state WRITE setState);
	Q_PROPERTY(double ratio READ ratio WRITE setRatio)
	Q_PROPERTY(QString name READ name WRITE setName)
	Q_PROPERTY(QString hash READ hash WRITE setHash)
	Q_PROPERTY(int priority READ priority WRITE setPriority)
	Q_ENUMS(TorrentState)
	
	public:
		TorrentItem(const QString & hash);
		~TorrentItem();
		
		enum Attribute { Hash, Name, Status, ChunkSize, Chunks, Size, Uploaded, UploadRate, Downloaded, DownloadRate, State, Ratio, Seeders, SeedersConnected, Leechers, LeechersConnected };
		enum TorrentState { Downloading, Seeding, Stopped, Completed };
		enum RTorrentState { Closed, Open };
		enum TorrentPriority { Off, Low, Medium, High };
		
		int seedsConnected() const;
		int seedsTotal() const;
		int leechsConnected() const;
		int leechsTotal() const;
		ByteSize downloadRate() const;
		ByteSize uploadRate() const;
		int chunks() const;
		int completedChunks() const;
		ByteSize chunkSize() const;
		ByteSize size() const;
		ByteSize uploaded() const;
		ByteSize downloaded() const;
		TorrentItem::TorrentState state() const;
		double ratio() const;
		QString name() const;
		QString hash() const;
		int priority() const;
		
		void setSeedsConnected(int seedsConnected);
		void setSeedsTotal(int seedsTotal);
		void setLeechsConnected(int leechsConnected);
		void setLeechsTotal(int leechsTotal);
		void setDownloadRate(ByteSize downloadRate);
		void setUploadRate(ByteSize uploadRate);
		void setChunks(int chunks);
		void setCompletedChunks(int completedChunks);
		void setChunkSize(ByteSize chunkSize);
		void setUploaded(ByteSize uploaded);
		void setDownloaded(ByteSize downloaded);
		void setName(const QString & name);
		void setHash(const QString & hash);
		void setSize(ByteSize size);
		void setState(int state);
		void setPriority(int priority);
		void setRatio(double ratio);
		bool operator==(const TorrentItem & other);
		bool operator<(const TorrentItem & other);
	public slots:
		void update();
		void result(const QString & method, const QVariant & result);
		void stop();
		void start();
	private:
		int m_seedsConnected;
		int m_seedsTotal;
		int m_leechsConnected;
		int m_leechsTotal;
		ByteSize m_downloadRate;
		ByteSize m_uploadRate;
		ByteSize m_chunkSize;
		int m_chunks;
		int m_completedChunks;
		ByteSize m_uploaded;
		ByteSize m_downloaded;
		int m_state;
		int m_priority;
		double m_ratio;
		QString m_name;
		QString m_hash;
		ByteSize m_size;
	signals:
		void call(const QString & method, const QVariantList & args, QObject * sender, const QString & member);
		void dataChanged(TorrentItem * item);
		void updated();
};

#endif
