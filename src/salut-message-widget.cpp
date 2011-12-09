/*
    Extended KMessageWidget for link-local xmpp account autocreation
    Copyright (C) 2011  Martin Klapetek <martin.klapetek@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "salut-message-widget.h"

#include <KTelepathy/circular-countdown.h>

#include <KLocalizedString>
#include <KIcon>
#include <KAction>

#include <QLayout>
#include <QWidgetAction>
#include <QVBoxLayout>

SalutMessageWidget::SalutMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setMessageType(KMessageWidget::Information);
    setWordWrap(true);
    QSize sz = size();
    sz.setWidth(parent->size().width());
    resize(sz);

    setCloseButtonVisible(false);

    KTp::CircularCountdown *circCountdown = new KTp::CircularCountdown(8000, this);

    connect(circCountdown, SIGNAL(timeout()), this, SIGNAL(timeout()));

    KAction *configAction = new KAction(KIcon("configure"), i18n("Configure manually..."), this);
    connect(configAction, SIGNAL(triggered(bool)), this, SIGNAL(configPressed()));
    addAction(configAction);

    KAction *cancelAction = new KAction(KIcon("dialog-cancel"), i18n("Cancel"), this);
    connect(cancelAction, SIGNAL(triggered(bool)), this, SIGNAL(cancelPressed()));
    addAction(cancelAction);

    //whatever will user choose, stop the countdown
    connect(this, SIGNAL(cancelPressed()), circCountdown, SLOT(stop()));
    connect(this, SIGNAL(configPressed()), circCountdown, SLOT(stop()));

    //6px padding for the CircularCountdown
    int padding = 6;

    //this takes the CircularCountdown and moves it to the top right corner of this message widget
    //16 is the width of CC, but somehow circCountdown->width() doesn't work right, so it's harcoded
    circCountdown->move(this->width() - (16 + padding), padding);
    circCountdown->start();
}

SalutMessageWidget::~SalutMessageWidget()
{
}

///these params always comes from KUser with first & last name splitted manualy by the last space
void SalutMessageWidget::setParams(const QString& firstname, const QString& lastname, const QString& nick)
{
    QString displayName;

    //either one of the names is filled and nick is filled
    if (((lastname.isEmpty() && !firstname.isEmpty()) || (!lastname.isEmpty() && firstname.isEmpty()))
            && !nick.isEmpty()) {

        displayName = QString("%1 (%2)").arg(lastname.isEmpty() ? firstname : lastname, nick);

    //either one of the names is filled and nick is empty
    } else if (((lastname.isEmpty() && !firstname.isEmpty()) || (!lastname.isEmpty() && firstname.isEmpty()))
            && nick.isEmpty()) {

        displayName = QString("%1").arg(lastname.isEmpty() ? firstname : lastname);

    //both first & last names are empty but nick is not
    } else if (lastname.isEmpty() && firstname.isEmpty() && !nick.isEmpty()) {

        displayName = QString("%1").arg(nick);

    } else if (lastname.isEmpty() && firstname.isEmpty() && nick.isEmpty()) {
        //FIXME: let the user know that he reached a very strange situation

    } else {
        displayName = QString("%1 %2 (%3)").arg(firstname, lastname, nick);
    }

    setText(i18n("You will appear as \"%1\" on your local network.",
                 displayName));
}
