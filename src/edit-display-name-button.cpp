/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2012 Daniele E. Domenichelli <daniele.domenichelli@gmail.com>
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

#include "edit-display-name-button.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>

#include <KDE/KDialog>
#include <KDE/KLineEdit>
#include <KDE/KLocalizedString>
#include <KDE/KDebug>

#include <TelepathyQt/Account>
#include <TelepathyQt/PendingOperation>


class EditDisplayNameDialog : public KDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(EditDisplayNameDialog)

public:
    EditDisplayNameDialog(const Tp::AccountPtr &account,
                          QWidget* parent = 0,
                          Qt::WFlags flags = 0);
    virtual ~EditDisplayNameDialog();

    QString displayName() const;

private:
    Tp::AccountPtr m_account;
    KLineEdit *m_displayNameLineEdit;
};


EditDisplayNameDialog::EditDisplayNameDialog(const Tp::AccountPtr &account,
                                             QWidget* parent,
                                             Qt::WFlags flags)
    : KDialog(parent, flags),
      m_account(account)
{
    setCaption(i18n("Edit Display Name"));
    setButtons( KDialog::Ok | KDialog::Cancel );
    setWindowIcon(KIcon(QLatin1String("telepathy-kde")));
    setFixedSize(400, 150);

    QWidget * mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *topLayout = new QHBoxLayout();

    QLabel *topLabel = new QLabel(i18n("Choose a new display name for your account"), this);
    topLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    topLabel->setWordWrap(true);
    QFont font = topLabel->font();
    font.setBold(true);
    topLabel->setFont(font);

    QLabel *bottomLabel = new QLabel(i18n("A display name is your local alias for the account, only you will see it."), this);
    bottomLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    bottomLabel->setWordWrap(true);

    QLabel *icon = new QLabel;
    icon->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    icon->setPixmap(KIcon(account->iconName()).pixmap(KIconLoader::SizeMedium));

    m_displayNameLineEdit = new KLineEdit(account->displayName(), this);
    m_displayNameLineEdit->setToolTip(i18n("New display name"));
    m_displayNameLineEdit->setWhatsThis(i18n("A display name is your local alias for the account, only you will see it."));


    topLayout->addWidget(topLabel);
    topLayout->addWidget(icon);

    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();

    mainLayout->addWidget(m_displayNameLineEdit);
    mainLayout->addWidget(bottomLabel);
    mainLayout->addStretch();

    mainWidget->setLayout(mainLayout);
    setMainWidget(mainWidget);
}

EditDisplayNameDialog::~EditDisplayNameDialog()
{
}


QString EditDisplayNameDialog::displayName() const
{
    return m_displayNameLineEdit->text();
}



EditDisplayNameButton::EditDisplayNameButton(QWidget* parent) :
    KPushButton(parent)
{
    connect(this,
            SIGNAL(clicked(bool)),
            SLOT(onClicked()));
}

EditDisplayNameButton::~EditDisplayNameButton()
{
}

void EditDisplayNameButton::setAccount(const Tp::AccountPtr &account)
{
    m_account = account;
}

Tp::AccountPtr EditDisplayNameButton::account() const
{
    return m_account;
}

void EditDisplayNameButton::onClicked()
{
    if (!m_account.isNull() && m_account->isValid()) {
        QWeakPointer<EditDisplayNameDialog> dialog = new EditDisplayNameDialog(m_account, this);
        dialog.data()->exec();
        if (!dialog.isNull()) {
            if (dialog.data()->result() == KDialog::Accepted && dialog.data()->displayName() != m_account->displayName()) {
                kDebug() << "Setting display name" << dialog.data()->displayName() << "for account" << account()->uniqueIdentifier();
                Tp::PendingOperation *op = m_account->setDisplayName(dialog.data()->displayName());
                connect(op, SIGNAL(finished(Tp::PendingOperation*)), SLOT(onFinished(Tp::PendingOperation*)));
            }
            dialog.data()->deleteLater();
        }
    }
}

void EditDisplayNameButton::onFinished(Tp::PendingOperation* op)
{
    if (op->isError()) {
        kWarning() << "Cannot set display name" << op->errorName() << op->errorMessage();
    }
}


#include "edit-display-name-button.moc"
#include "moc_edit-display-name-button.cpp"
