/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Florian Reinhard <florian.reinhard@googlemail.com>
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

#include "skype-main-options-widget.h"

#include <KCMTelepathyAccounts/ParameterEditModel>

#include <QtCore/QDir>

SkypeMainOptionsWidget::SkypeMainOptionsWidget(ParameterEditModel *model, QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    // Set up the UI.
    m_ui = new Ui::SkypeMainOptionsWidget;
    m_ui->setupUi(this);

    handleParameter(QLatin1String("account"), QVariant::String, m_ui->accountLineEdit, m_ui->accountLabel, true);

#ifdef Q_WS_X11
    // get autocomplete choices for the accountname
    // Skype stores data for each account that has been used in $HOME/.Skype/<accountname>/
    QDir skypeConfigDir(QDir::home().filePath(QLatin1String(".Skype")));

    skypeConfigDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList folderList = skypeConfigDir.entryInfoList();

    KCompletion *completion = new KCompletion;
    Q_FOREACH (const QFileInfo &info, folderList) {
        completion->addItem(info.fileName());
    }
    m_ui->accountLineEdit->setCompletionObject(completion);
    m_ui->accountLineEdit->setAutoDeleteCompletionObject(true);
#endif
}

SkypeMainOptionsWidget::~SkypeMainOptionsWidget()
{
    delete m_ui;
}

void SkypeMainOptionsWidget::updateDefaultDisplayName()
{
    setDefaultDisplayName(m_ui->accountLineEdit->text());
}

#include "skype-main-options-widget.moc"
