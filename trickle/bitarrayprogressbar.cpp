/*
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA  02110-1301, USA.

---
Copyright (C) 2009, Michael Forney <mforney@trickleproject.org>
*/

#include "bitarrayprogressbar.h"

#include <QPainter>
#include <QPaintEvent>
#include <QStyleOptionFrameV2>
#include <KDebug>
#include <QPair>

BitArrayProgressBar::BitArrayProgressBar(QWidget * parent)
: QWidget(parent)
{
    setMinimumSize(minimumSize().width(), 25);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    updateGeometry();
}

BitArrayProgressBar::BitArrayProgressBar(const QBitArray & bitArray, QWidget * parent)
: QWidget(parent)
{
    setBitArray(bitArray);

    setMinimumSize(minimumSize().width(), 25);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    updateGeometry();
}

BitArrayProgressBar::~BitArrayProgressBar()
{
}

void BitArrayProgressBar::paintEvent(QPaintEvent * event)
{
    //qDebug() << "Painting AreaProgressBar";
    QPainter p;
    p.begin(this);

    QStyleOptionFrameV2 option;
    option.initFrom(this);
    option.lineWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, &option, this);

    style()->drawPrimitive(QStyle::PE_Frame, &option, &p, this);

    QRect rect = style()->subElementRect(QStyle::SE_FrameContents, &option, this);
    p.setClipRect(rect);
    p.fillRect(rect, palette().base());
    rect = event->rect();
    QVector<QRectF> regionRects;
    bool currentBit = false;
    int regionStart;
    for (int i = 0; i < m_bitArray.size(); i++)
    {
        if (m_bitArray.testBit(i) != currentBit)
        {
            if (m_bitArray.testBit(i))
            {
                regionStart = i;
            }
            else
            {
                if (i - regionStart >= (qreal)(m_bitArray.size()) / rect.width())
                {
                    regionRects.append(QRectF(QPointF((qreal)(regionStart) * rect.width() / m_bitArray.size(), 0), QPointF((qreal)(i * rect.width() / m_bitArray.size()), rect.height())));
                }
            }
            currentBit = !currentBit;
        }
    }
    if (currentBit)
    {
        regionRects.append(QRectF(QPointF((qreal)(regionStart) * rect.width() / m_bitArray.size(), 0), QPointF(rect.width(), rect.height())));
    }
    p.setBrush(palette().highlight());
    p.setPen(Qt::NoPen);
    p.drawRects(regionRects);

    QStyleOptionProgressBarV2 progressOption;
    progressOption.initFrom(this);
    int value = m_bitArray.count(true);
    int progress = (qreal)value * 100 / m_bitArray.size();
    progressOption.text = QString("%1/%2 (%3%)").arg(value).arg(m_bitArray.size()).arg(progress);
    progressOption.minimum = 0;
    progressOption.maximum = m_bitArray.size();
    //progressOption.progress = value;
    style()->drawControl(QStyle::CE_ProgressBarLabel, &progressOption, &p, this);

    p.end();
}

void BitArrayProgressBar::setBitArray(const QBitArray & bitArray)
{
    m_bitArray = bitArray;
    update();
}

void BitArrayProgressBar::clear()
{
    setBitArray(QBitArray());
}

#include "bitarrayprogressbar.moc"
