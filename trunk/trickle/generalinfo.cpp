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
#include "generalinfo.h"

#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QGroupBox>

#include "xmlrpc.h"
#include "updatetimer.h"

GeneralInfo::GeneralInfo()
 : QWidget()
{
	QVBoxLayout * mainLayout = new QVBoxLayout(this);
	{
		QGroupBox * speedBox = new QGroupBox("Speeds", this);
		QHBoxLayout * speedBoxLayout = new QHBoxLayout(speedBox);
		{
			QVBoxLayout * labelLayout = new QVBoxLayout();
			{
				labelLayout->addWidget(new QLabel("Upload Limit: "));
				labelLayout->addWidget(new QLabel("Download Limit: "));
			}
			QVBoxLayout * editLayout = new QVBoxLayout();
			{
				uploadLimitEdit = new QSpinBox(speedBox);
				{
					uploadLimitEdit->setMinimum(0);
					uploadLimitEdit->setMaximum(1024);
				}
				downloadLimitEdit = new QSpinBox(speedBox);
				{
					downloadLimitEdit->setMinimum(0);
					downloadLimitEdit->setMaximum(1024);
				}
				
				connect(uploadLimitEdit, SIGNAL(editingFinished()), this, SLOT(changeUploadLimit()));
				connect(downloadLimitEdit, SIGNAL(editingFinished()), this, SLOT(changeDownloadLimit()));
				
				editLayout->addWidget(uploadLimitEdit);
				editLayout->addWidget(downloadLimitEdit);
			}
			speedBoxLayout->addLayout(labelLayout);
			speedBoxLayout->addLayout(editLayout);
			speedBoxLayout->addStretch();			
		}
		mainLayout->addWidget(speedBox);
		mainLayout->addStretch();
		
		connect(UpdateTimer::instance(), SIGNAL(timeout()), this, SLOT(update()));
	}
}

GeneralInfo::~GeneralInfo()
{
}

void GeneralInfo::changeUploadLimit()
{
	//XmlRpc::instance()->call("set_upload_rate", QVariantList() << uploadLimitEdit->value() * 1024, this, "result");
}

void GeneralInfo::changeDownloadLimit()
{
	//XmlRpc::instance()->call("set_download_rate", QVariantList() << downloadLimitEdit->value() * 1024, this, "result");
}
		
void GeneralInfo::update()
{
	//XmlRpc::instance()->call("get_download_rate", QVariantList(), this, "result");
	//XmlRpc::instance()->call("get_upload_rate", QVariantList(), this, "result");
}

void GeneralInfo::result(const QString & method, const QVariant & result)
{
	if (method == "get_upload_rate")
	{
		if (!uploadLimitEdit->hasFocus())
		{
			uploadLimitEdit->setValue(result.toInt() / 1024);
		}
	}
	else if (method == "get_download_rate")
	{
		if (!downloadLimitEdit->hasFocus())
		{
			downloadLimitEdit->setValue(result.toInt() / 1024);
		}
	}
}

#include "generalinfo.moc"
