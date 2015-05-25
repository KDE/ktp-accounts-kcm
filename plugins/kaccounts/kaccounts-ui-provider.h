/*
 * Copyright 2014  Martin Klapetek <mklapetek@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KACCOUNTSUIPROVIDER_H
#define KACCOUNTSUIPROVIDER_H

#include <KAccounts/kaccountsuiplugin.h>
#include <KMessageWidget>

namespace Tp {
class PendingOperation;
}

class KAccountsUiProvider : public KAccountsUiPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kaccounts.UiPlugin")
    Q_INTERFACES(KAccountsUiPlugin)

public:
    KAccountsUiProvider(QObject *parent = 0);
    virtual ~KAccountsUiProvider();

    virtual void init(KAccountsUiPlugin::UiType type);

    virtual void showNewAccountDialog();
    virtual void showConfigureAccountDialog(const quint32 accountId);
    virtual void setProviderName(const QString &providerName);
    virtual QStringList supportedServicesForConfig() const;

Q_SIGNALS:
    void feedbackMessage(const QString &text, const QString &comment, KMessageWidget::MessageType);

private Q_SLOTS:
    void onProfileManagerReady(Tp::PendingOperation*);
    void onConnectionManagerReady(Tp::PendingOperation*);
    void onCreateAccountDialogAccepted();
    void onCreateAccountDialogRejected();
    void onConfigureAccountDialogAccepted();
    void onConfigureAccountDialogRejected();
    void onConfigureAccountFinished();

    void storePasswordInSso(const quint32 accountId, const QString &password);

private:
    class Private;
    Private * const d;
};

#endif // KACCOUNTSUIPROVIDER_H
