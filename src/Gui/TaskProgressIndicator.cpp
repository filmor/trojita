/* Copyright (C) 2006 - 2014 Jan Kundrát <jkt@flaska.net>
   Copyright (C) 2014 Thomas Lübking <thomas.luebking@gmail.com>

   This file is part of the Trojita Qt IMAP e-mail client,
   http://trojita.flaska.net/

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3 or any later version
   accepted by the membership of KDE e.V. (or its successor approved
   by the membership of KDE e.V.), which shall act as a proxy
   defined in Section 14 of version 3 of the license.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "TaskProgressIndicator.h"
#include <QApplication>
#include <QMouseEvent>
#include <QTimer>
#include "Imap/Model/Model.h"
#include "Imap/Model/VisibleTasksModel.h"

namespace Gui
{

TaskProgressIndicator::TaskProgressIndicator(QWidget *parent) :
    QProgressBar(parent), m_busyCursorTrigger(new QTimer(this)), m_busy(false)
{
    setMinimum(0);
    setMaximum(0);
    connect(m_busyCursorTrigger, SIGNAL(timeout()), this, SLOT(setCursorBusy()));
    m_busyCursorTrigger->setSingleShot(true);
    m_busyCursorTrigger->setInterval(666);
}

/** @short Connect to the specified IMAP model as the source of the activity information */
void TaskProgressIndicator::setImapModel(Imap::Mailbox::Model *model)
{
    if (model) {
        m_visibleTasksModel = new Imap::Mailbox::VisibleTasksModel(model, model->taskModel());
        connect(m_visibleTasksModel, SIGNAL(layoutChanged()), this, SLOT(updateActivityIndication()));
        connect(m_visibleTasksModel, SIGNAL(modelReset()), this, SLOT(updateActivityIndication()));
        connect(m_visibleTasksModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(updateActivityIndication()));
        connect(m_visibleTasksModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(updateActivityIndication()));
    }
}

/** @short Reflect a possible change in the activity in the GUI */
void TaskProgressIndicator::updateActivityIndication()
{
    if (!m_visibleTasksModel)
        return;

    bool busy = m_visibleTasksModel->hasChildren();
    setVisible(busy);
    if (!m_busy && busy) {
        if (!m_busyCursorTrigger->isActive()) {
            // do NOT restart, we don't want to prolong this delay
            m_busyCursorTrigger->start();
        }
    } else if (m_busy && !busy) {
        if (!m_busyCursorTrigger->isActive()) {
            // don't restore unless we've alredy set it
            QApplication::restoreOverrideCursor();
        }
        // don't cause future timeout
        m_busyCursorTrigger->stop();
    }

    if (busy) {
        setToolTip(tr("%n ongoing actions", 0, m_visibleTasksModel->rowCount()));
    } else {
        setToolTip(tr("IMAP connection idle"));
    }

    m_busy = busy;
}

void TaskProgressIndicator::setCursorBusy()
{
    QApplication::setOverrideCursor(Qt::BusyCursor);
}

/** @short Reimplemented from QProgressBar for launching the pop-up widgets with detailed activity */
void TaskProgressIndicator::mousePressEvent(QMouseEvent *event)
{
    if (!m_visibleTasksModel)
        return;

    if (event->buttons() == Qt::LeftButton) {
        event->accept();
    } else {
        QProgressBar::mousePressEvent(event);
    }
}

}
