#include "admindialog.h"
#include <QDebug>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "adminpanels/userslistwidget.h"
#include "app.h"

AdminDialog::AdminDialog(QWidget *parent) :
    QDialog(parent)
{
    mListWidget = new QListWidget;
    mStackedWidget = new QStackedWidget;
    mButtonBox = new QDialogButtonBox(QDialogButtonBox::Reset|QDialogButtonBox::Cancel|QDialogButtonBox::Save);

    mListWidget->setMaximumWidth(200);
    mListWidget->setIconSize(QSize(48,48));
    mListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    mListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    mListWidget->setDragDropMode(QAbstractItemView::NoDragDrop);

    //mStackedWidget->setContentsMargins(0,0,0,0);

    QHBoxLayout * cLayout = new QHBoxLayout;
    cLayout->addWidget(mListWidget);
    cLayout->addWidget(mStackedWidget);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addLayout(cLayout);
    mainLayout->addWidget(mButtonBox);

    setLayout(mainLayout);


    addWidget(new UsersListWidget(), tr("Users"), app->awesome()->icon(fa::users));

    resize(800,400);

    connect(mListWidget,SIGNAL(currentRowChanged(int)),this,SLOT(updateTab(int)));
    connect(mButtonBox,SIGNAL(rejected()),this,SLOT(reject()));
    connect(mButtonBox,SIGNAL(accepted()),this,SLOT(save()));

    setWindowTitle(tr("Administration"));

    load();
}


void AdminDialog::addWidget(AbstractSettingsWidget *widget,
                               const QString& categorie,
                               const QIcon& icon)
{
    // if categories doesn't exists, create it ! In Hash and In ListView
    if (!mWidgets.keys().contains(categorie)){
        mWidgets[categorie] = QList<AbstractSettingsWidget*>();
        mListWidget->addItem(new QListWidgetItem(icon,categorie));


    }

    mWidgets[categorie].append(widget);

}

void AdminDialog::save()
{

    foreach (QString key, mWidgets.keys())
    {
        foreach (AbstractSettingsWidget * w, mWidgets[key])
        {
            if (!w->save())
            {
                qDebug() << Q_FUNC_INFO << "Cannot save settings " << w->windowTitle();
            }
        }

    }

    // App::i()->loadSettings();
    emit accept();

}

void AdminDialog::load()
{

    foreach (QString key, mWidgets.keys())
    {
        foreach (AbstractSettingsWidget* widget, mWidgets[key])
        {
            if (!widget->load())
            {
                qDebug() << Q_FUNC_INFO << "Cannot load settings " << widget->windowTitle();
            }
        }
    }
    mListWidget->setCurrentRow(0);
}

void AdminDialog::updateTab(int row)
{
    Q_UNUSED(row);

    // clear stackedwidget
    for(int i = mStackedWidget->count(); i >= 0; i--)
    {
        QWidget* widget = mStackedWidget->widget(i);
        mStackedWidget->removeWidget(widget);
        widget->deleteLater();
    }

    // populate stackedwidget
    foreach (AbstractSettingsWidget* widget , mWidgets.value(mListWidget->currentItem()->text()))
    {
        mStackedWidget->addWidget(widget);
    }

}