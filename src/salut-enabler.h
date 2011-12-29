/*
    Class that auto configures link-local xmpp account
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


#ifndef SALUT_ENABLER_H
#define SALUT_ENABLER_H

#include <QtCore/QObject>

#include <TelepathyQt/Types>
#include <KMessageWidget>

class QFrame;
class QWidget;

namespace Tp {
    class PendingOperation;
}

class SalutEnabler : public QObject
{
    Q_OBJECT
public:
    explicit SalutEnabler(const Tp::AccountManagerPtr accountManager, QObject *parent = 0);
    virtual ~SalutEnabler();

    QFrame *frameWidget(QWidget *parent);

Q_SIGNALS:
    void userInfoReady();
    void done();
    void cancelled();
    void feedbackMessage(const QString &text, const QString &comment, KMessageWidget::MessageType);

public Q_SLOTS:
    void onUserAccepted();
    void onUserWantingChanges();
    void onUserCancelled();
    void onDialogAccepted(const QVariantMap &values);

private Q_SLOTS:
    void onAccountCreated(Tp::PendingOperation *op);
    void onConnectionManagerReady(Tp::PendingOperation *op);
    void onProfileManagerReady(Tp::PendingOperation *op);

private:
    class Private;
    Private * const d;
};

#endif // SALUT_ENABLER_H
