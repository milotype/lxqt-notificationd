/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Razor - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2012 Razor team
 * Authors:
 *   Alec Moskvin <alecm@gmx.com>
 *   Petr Vanek <petr@scribus.info>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "notifyd.h"

#include <QtCore/QDebug>

#include <QtGui/QMessageBox>


/*
 * Implementation of class Notifyd
 */

Notifyd::Notifyd(QObject* parent)
    : QObject(parent),
      mId(0)
{
    qDebug() << "Notifyd created";
    m_area = new NotificationArea();

    connect(this, SIGNAL(notificationAdded(uint,QString,QString,QString,QString,int)),
            m_area->layout(), SLOT(addNotification(uint,QString,QString,QString,QString,int)));
    connect(this, SIGNAL(notificationClosed(uint, uint)),
            m_area->layout(), SLOT(removeNotification(uint, uint)));
    // feedback for original caller
    connect(m_area->layout(), SIGNAL(notificationClosed(uint,uint)),
            this, SIGNAL(NotificationClosed(uint,uint)));

}

Notifyd::~Notifyd()
{
    qDebug() << "Notifyd deleted";
    m_area->deleteLater();
}

void Notifyd::CloseNotification(uint id)
{
    qDebug() << QString("CloseNotification(%1);").arg(id);
    emit notificationClosed(id, 3);
}

QStringList Notifyd::GetCapabilities()
{
    QStringList caps;
    caps << "actions"
         << "body"
      // << "body-hyperlinks"
      // << "body-images"
      // << "body-markup"
      // << "icon-multi"
      // << "icon-static"
      // << "sound"
      ;
    qDebug() << QString("GetCapabilities(); =>") << caps;
    return caps;
}

QString Notifyd::GetServerInformation(QString& vendor, QString& version, QString& spec_version)
{
    //qDebug() << QString("GetServerInformation(%1, %2, %3);").arg(vendor, version, spec_version);
    qDebug() << QString("GetServerInformation(...);");
    spec_version = QString("0.9");
    version = QString("0.1");
    vendor = QString("Alec");
    return QString("notifyd");
}

uint Notifyd::Notify(const QString& app_name,
                               uint replaces_id,
                     const QString& app_icon,
                     const QString& summary,
                     const QString& body,
                 const QStringList& actions,
                 const QVariantMap& hints,
                                int expire_timeout
                     )
{
//    qDebug() << "IDs" << mId << replaces_id;
    uint ret;
    if (replaces_id == 0)
    {
        mId++;
        ret = mId;
    }
    else
        ret = replaces_id;
#if 0
    qDebug() << QString("Notify(\n"
                        "  app_name = %1\n"
                        "  replaces_id = %2\n"
                        "  app_icon = %3\n"
                        "  summary = %4\n"
                        "  body = %5\n"
                        ).arg(app_name, QString::number(replaces_id), app_icon, summary, body)
                     << "  actions =" << actions << endl
                     << "  hints =" << hints << endl
             << QString("  expire_timeout = %1\n) => %2").arg(QString::number(expire_timeout), QString::number(mId));
#endif
    emit notificationAdded(ret, app_name, summary, body, app_icon, expire_timeout);

    return ret;
}