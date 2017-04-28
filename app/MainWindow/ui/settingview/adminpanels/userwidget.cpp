#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "myprofilewidget.h"
#include "regovar.h"

MyProfileWidget::MyProfileWidget(QWidget *parent) : AbstractSettingsWidget(parent)
{
    mLogin = new QLabel(regovar->currentUser()->login());
    mFirstname = new QLineEdit();
    mLastname = new QLineEdit();
    mFunction = new QLineEdit();
    mEmail = new QLineEdit();
    mLocation = new QLineEdit();
    mAvatar = new QLabel("avatar");

    connect(mFirstname, &QLineEdit::textChanged, this, &MyProfileWidget::onChanged);
    connect(mLastname, &QLineEdit::textChanged, this, &MyProfileWidget::onChanged);
    connect(mFunction, &QLineEdit::textChanged, this, &MyProfileWidget::onChanged);
    connect(mEmail, &QLineEdit::textChanged, this, &MyProfileWidget::onChanged);
    connect(mLocation, &QLineEdit::textChanged, this, &MyProfileWidget::onChanged);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow(tr("Firstname"), mFirstname);
    formLayout->addRow(tr("Lastname"), mLastname);
    formLayout->addRow(tr("Email"), mEmail);
    formLayout->addRow(tr("Location"), mLocation);
    formLayout->addRow(tr("Function"), mFunction);


    QWidget* stretcher = new QWidget(this);
    stretcher->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    QVBoxLayout* leftPanel = new QVBoxLayout();
    leftPanel->addWidget(mAvatar);
    leftPanel->addWidget(new QLabel("My titles"));
    leftPanel->addWidget(stretcher);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftPanel);
    mainLayout->addLayout(formLayout);

    setLayout(mainLayout);
}


bool MyProfileWidget::save()
{
    regovar->currentUser()->setFirstname(mFirstname->text());
    regovar->currentUser()->setLastname(mLastname->text());
    regovar->currentUser()->setFunction(mFunction->text());
    regovar->currentUser()->setEmail(mEmail->text());
    regovar->currentUser()->setLocation(mLocation->text());

    regovar->currentUser()->save();
    return true;
}

bool MyProfileWidget::load()
{
    mFirstname->setText(regovar->currentUser()->firstname());
    mLastname->setText(regovar->currentUser()->lastname());
    mFunction->setText(regovar->currentUser()->function());
    mEmail->setText(regovar->currentUser()->email());
    mLocation->setText(regovar->currentUser()->location());

    return true;
}

void MyProfileWidget::onChanged()
{
    mHaveChanged = true;
}



const bool MyProfileWidget::haveChanged() const
{
    return mHaveChanged;
}