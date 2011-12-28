#ifndef ACCOUNT_IDENTITY_DIALOG_H
#define ACCOUNT_IDENTITY_DIALOG_H

#include <KDialog>

#include <TelepathyQt/Types>

namespace Ui {
    class AccountIdentityDialog;
}

/** Widget that configures user's identity (nickname/avatar etc) for an account*/

class AccountIdentityDialog : public KDialog
{
    Q_OBJECT

public:
    explicit AccountIdentityDialog(const Tp::AccountPtr &account, QWidget *parent = 0);
    ~AccountIdentityDialog();

    /** Returns if the user has changed settings*/
    bool hasChanged() const;

public Q_SLOTS:
    /** Updates the account to user selection*/
    void apply();

private Q_SLOTS:
    void onNicknameChanged(const QString &nickname);
    void onAvatarChanged(const Tp::Avatar &avatar);
    void onEdited();

private:
    Tp::AccountPtr m_account;
    Ui::AccountIdentityDialog *ui;
};

#endif // ACCOUNT_IDENTITY_DIALOG_H
