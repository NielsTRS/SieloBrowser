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

#include "Utils/Updater.hpp"

#include <QTimer>

#include <QMessageBox>

#include <QFile>
#include <QStandardPaths>
#include <QDesktopServices>

#include <QNetworkRequest>
#include <QNetworkReply>

#include "Network/NetworkManager.hpp"

#include "BrowserWindow.hpp"

#include "Application.hpp"

namespace Sn {

Updater::Updater(BrowserWindow* window, QObject* parent) :
	QObject(parent),
	m_window(window)
{
	QTimer::singleShot(10 * 1000, this, &Updater::start);
}

void Updater::downloadUpdateInfoCompleted()
{
	if (!m_versionReply)
		return;

	QString newVersion = m_versionReply->readAll();
	newVersion.remove("\n");

	if (newVersion != Application::currentVersion) {
#if defined(Q_OS_WIN)
		QMessageBox::information(m_window, tr("Update"), tr("A new version of Sielo will be download in background!"));
		QUrl updaterUrl{QUrl("http://feldrise.com/Sielo/SN_update_setup.exe")};
		startDownloadNewVersion(updaterUrl);
#elif defined(Q_OS_LINUX)
		QMessageBox::information(m_window,
								 tr("Update"),
								 tr("A new version of Sielo is available! We advise you to download it."));
#endif
	}

	m_versionReply->deleteLater();

}

void Updater::downloadCompleted()
{
	QString updaterLocation{QStandardPaths::writableLocation(QStandardPaths::TempLocation)};
	QFile updater{updaterLocation + QLatin1String("/SieloUpdater.exe")};

	if (!updater.open(QIODevice::WriteOnly)) {
		QMessageBox::critical(m_window, tr("Update fail"), tr("Impossible to update Sielo... Please retry later"));
		return;
	}

	updater.write(m_updaterReply->readAll());
	updater.close();

	QDesktopServices::openUrl(QUrl(updaterLocation + QLatin1String("/SieloUpdater.exe")));

	m_updaterReply->deleteLater();
	m_window->close();
}

void Updater::start()
{
	QUrl newVersionUrl{QUrl("http://feldrise.com/Sielo/version.txt")};

	startDownloadingUpdateInfo(newVersionUrl);
}

void Updater::startDownloadNewVersion(const QUrl& url)
{
	m_updaterReply = Application::instance()->networkManager()->get(QNetworkRequest(url));

	connect(m_updaterReply, &QNetworkReply::finished, this, &Updater::downloadCompleted);
}

void Updater::startDownloadingUpdateInfo(const QUrl& url)
{
	m_versionReply = Application::instance()->networkManager()->get(QNetworkRequest(url));

	connect(m_versionReply, &QNetworkReply::finished, this, &Updater::downloadUpdateInfoCompleted);
}

}
