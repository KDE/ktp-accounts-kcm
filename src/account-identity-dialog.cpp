#include "account-identity-dialog.h"
#include "ui_account-identity-dialog.h"

#include <TelepathyQt/Account>
#include <TelepathyQt/AvatarData>
#include <TelepathyQt/PendingOperation>

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
    setButtons(KDialog::Ok | KDialog::Cancel);

    connect(m_account.data(), SIGNAL(nicknameChanged(QString)), SLOT(onNicknameChanged(QString)));
    connect(m_account.data(), SIGNAL(avatarChanged(Tp::Avatar)), SLOT(onAvatarChanged(Tp::Avatar)));
    onNicknameChanged(account->nickname());
    onAvatarChanged(account->avatar());

    ui->accountId->setText(m_account->displayName());

    connect(this, SIGNAL(okClicked()), SLOT(apply()));
}

AccountIdentityDialog::~AccountIdentityDialog()
{
    delete ui;
}

void AccountIdentityDialog::onNicknameChanged(const QString &nickname)
{
    ui->accountNickname->setText(nickname);
}

void AccountIdentityDialog::onAvatarChanged(const Tp::Avatar &avatar)
{
    ui->accountAvatar->setAvatar(avatar);
}

void AccountIdentityDialog::apply()
{
    if (!m_account.isNull()) {
        //not much point watching these, they just return that everything was OK even when it isn't.
        m_account->setAvatar(ui->accountAvatar->avatar());
        m_account->setNickname(ui->accountNickname->text());
    }

    close();
}

#include "account-identity-dialog.moc"
