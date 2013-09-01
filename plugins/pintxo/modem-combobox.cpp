/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2013 Anant Kamath <kamathanant@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <QDebug>

#include <ModemManagerQt/manager.h>
#include <ModemManagerQt/modemgsmcardinterface.h>

#include "modem-combobox.h"

ModemComboBox::ModemComboBox(QWidget* parent) : QComboBox(parent)
{
    modems = ModemManager::modemInterfaces();
    if(!modems.isEmpty()) {
        Q_FOREACH(ModemManager::ModemInterface::Ptr modem, modems) {
            ModemManager::ModemGsmCardInterface::Ptr simCard = ModemManager::findModemInterface(modem->udi(), ModemManager::ModemInterface::GsmCard).objectCast<ModemManager::ModemGsmCardInterface>();
            if(!simCard.isNull()) {
                QString simIdent = simCard->getSimIdentifier();
                QString spn = simCard->getSpn();
                addItem(spn.isEmpty() ? QLatin1String("Unknown modem") : spn);
            }
        }
    }
}

ModemComboBox::~ModemComboBox()
{
}

QString ModemComboBox::selectedSimIdentifier()
{
    ModemManager::ModemGsmCardInterface::Ptr simCard = ModemManager::findModemInterface(modems.at(currentIndex())->udi(), ModemManager::ModemInterface::GsmCard).objectCast<ModemManager::ModemGsmCardInterface>();
    if(!simCard.isNull()) {
        return simCard->getSimIdentifier();
    }
    return QString();
}

#include "modem-combobox.moc"
