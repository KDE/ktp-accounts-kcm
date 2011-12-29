#include "account-identity-dialog.h"
#include "ui_account-identity-dialog.h"

#include <TelepathyQt/Account>
#include <TelepathyQt/AvatarData>

//FIXME possibly need to monitor account connection status and disable if appropriate?

AccountIdentityDialog::AccountIdentityDialog(const Tp::AccountPtr &account, QWidget *parent) :
    KDialog(parent),
    m_account(account),
    ui(new Ui::AccountIdentityDialog)
{
    QWidget *widget = new QWidget(this);
    ui->setupUi(widget);

    Q_ASSERT(! m_account.isNull());

    setMainWidget(widget);

    setWindowTitle(i18n("Edit Account Identity"));
    setButtons(KDialog::Cancel | KDialog::Apply);

    connect(m_account.data(), SIGNAL(nicknameChanged(QString)), SLOT(onNicknameChanged(QString)));
    connect(m_account.data(), SIGNAL(avatarChanged(Tp::Avatar)), SLOT(onAvatarChanged(Tp::Avatar)));
    onNicknameChanged(account->nickname());
    onAvatarChanged(account->avatar());

    ui->accountId->setText(m_account->displayName());
    connect(ui->accountNickname, SIGNAL(textChanged(QString)), SLOT(onEdited()));
    connect(ui->accountAvatar, SIGNAL(avatarChanged()), SLOT(onEdited()));

    onEdited();

    connect(this, SIGNAL(applyClicked()), SLOT(apply()));
}

AccountIdentityDialog::~AccountIdentityDialog()
{
    delete ui;
}

bool AccountIdentityDialog::hasChanged() const
{
    if (m_account.isNull()) {
        return false;
    }
    if (ui->accountNickname->text() != m_account->nickname()) {
        return true;
    }
    if (ui->accountAvatar->avatar().avatarData != m_account->avatar().avatarData) {
        return true;
    }
    return false;
}

void AccountIdentityDialog::onNicknameChanged(const QString &nickname)
{
    ui->accountNickname->setText(nickname);
    onEdited();
}

void AccountIdentityDialog::onAvatarChanged(const Tp::Avatar &avatar)
{
    ui->accountAvatar->setAvatar(avatar);
    onEdited();
}

void AccountIdentityDialog::apply()
{
    if (m_account.isNull()) {
        return;
    }

    m_account->setAvatar(ui->accountAvatar->avatar());
    m_account->setNickname(ui->accountNickname->text());

    //not much point watching these, they just return that everything was OK even when it isn't.
}

void AccountIdentityDialog::onEdited()
{
    enableButtonApply(hasChanged());
}

#include "account-identity-dialog.moc"
