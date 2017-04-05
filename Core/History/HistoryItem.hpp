/***********************************************************************************
** MIT License                                                                    **
**                                                                                **
** Copyright (c) 2017 Victor DENIS (victordenis01@gmail.com)                      **
**                                                                                **
** Permission is hereby granted, free of charge, to any person obtaining a copy   **
** of this software and associated documentation files (the "Software"), to deal  **
** in the Software without restriction, including without limitation the rights   **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      **
** copies of the Software, and to permit persons to whom the Software is          **
** furnished to do so, subject to the following conditions:                       **
**                                                                                **
** The above copyright notice and this permission notice shall be included in all **
** copies or substantial portions of the Software.                                **
**                                                                                **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  **
** SOFTWARE.                                                                      **
***********************************************************************************/

#pragma once
#ifndef SIELOBROWSER_HISTORYITEM_HPP
#define SIELOBROWSER_HISTORYITEM_HPP

#include <QDateTime>
#include <QString>

namespace Sn {

struct Q_DECL_EXPORT HistoryItem {
	HistoryItem() {};
	HistoryItem(const QString& c_url, const QDateTime& c_date = QDateTime(), const QString& c_title = QString()) :
		url(c_url),
		dateTime(c_date),
		title(c_title) {}

	inline bool operator==(const HistoryItem& other) const
	{
		return other.url == url && other.dateTime == dateTime && other.title == title;
	}

	inline bool operator<(const HistoryItem& other) const
	{
		return dateTime > other.dateTime;
	}

	QString url{};
	QDateTime dateTime{};
	QString title{};

};
}
#endif //SIELOBROWSER_HISTORYITEM_HPP