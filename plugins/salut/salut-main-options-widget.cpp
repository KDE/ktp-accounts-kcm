/*
 * This file is part of telepathy-firstnames-kcm-plugins
 *
 * Copyright (C) 2011 Florian Reinhard <florian.reinhard@googlemail.com>
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

#include "salut-main-options-widget.h"

#include <KCMTelepathyAccounts/ParameterEditModel>

#include <KUser>

SalutMainOptionsWidget::SalutMainOptionsWidget(ParameterEditModel *model, QWidget *parent)
    : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::SalutMainOptionsWidget;
    m_ui->setupUi(this);

    handleParameter(QLatin1String("first-name"), QVariant::String, m_ui->firstnameLineEdit, m_ui->firstnameLabel, true);
    handleParameter(QLatin1String("last-name"),  QVariant::String, m_ui->lastnameLineEdit,  m_ui->lastnameLabel, true);
    handleParameter(QLatin1String("nickname"),   QVariant::String, m_ui->nicknameLineEdit,  m_ui->nicknameLabel, true);

    // if the first- and last-name are empty on startup we add them based on
    // the current users full name
    if (m_ui->firstnameLineEdit->text().isEmpty() &&
        m_ui->lastnameLineEdit->text().isEmpty() &&
        m_ui->nicknameLineEdit->text().isEmpty())
    {
        KUser user = KUser();
        QString name = user.property(KUser::FullName).toString();
        int lastSpacePosition = name.lastIndexOf(QLatin1Char(' '));
        m_ui->firstnameLineEdit->setText(name.left(lastSpacePosition));
        m_ui->lastnameLineEdit->setText(name.mid(lastSpacePosition + 1));
        m_ui->nicknameLineEdit->setText(user.loginName());
    }

    QTimer::singleShot(0, m_ui->firstnameLineEdit, SLOT(setFocus()));
}

SalutMainOptionsWidget::~SalutMainOptionsWidget()
{
    delete m_ui;
}

void SalutMainOptionsWidget::updateDefaultDisplayName()
{
    QString displayName;
    QString firstname = m_ui->firstnameLineEdit->text();
    QString lastname = m_ui->lastnameLineEdit->text();
    QString nickname = m_ui->nicknameLineEdit->text();

    if (!firstname.isEmpty()) {
        displayName = firstname;
    }

    if (!lastname.isEmpty()) {
        if (!displayName.isEmpty()) {
            displayName.append(QString::fromLatin1(" %1").arg(lastname));
        } else {
            displayName = lastname;
        }
    }

    if (!nickname.isEmpty()) {
        if (!displayName.isEmpty()) {
            displayName.append(QString::fromLatin1(" (%1)").arg(nickname));
        } else {
            displayName = nickname;
        }
    }

    setDefaultDisplayName(displayName);
}

#include "salut-main-options-widget.moc"
