/* Copyright (C) 2014 Boren Zhang <bobo1993324@gmail.com>

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

#ifndef GUI_PARTWALKER_H
#define GUI_PARTWALKER_H

#include "UiUtils/PartWalker.h"

namespace Gui {

class MessageView;
typedef UiUtils::PartWalker<QWidget *, MessageView *> PartWidgetFactory;

}

namespace UiUtils {

extern template
QWidget *Gui::PartWidgetFactory::walk(const QModelIndex &partIndex, int recursionDepth, const PartLoadingOptions loadingMode);
extern template
Gui::MessageView *Gui::PartWidgetFactory::context() const;
extern template
void Gui::PartWidgetFactory::setNetworkWatcher(Imap::Mailbox::NetworkWatcher *netWatcher);

}

#endif // GUI_PARTWALKER_H
