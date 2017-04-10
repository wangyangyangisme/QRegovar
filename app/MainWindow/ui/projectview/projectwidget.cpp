#include "app.h"
#include "projectwidget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QListWidgetItem>
#include <QIcon>

namespace projectview
{

ProjectWidget::ProjectWidget(QWidget *parent) : QWidget(parent)
{

    // create widget and Layout of the view
    mTitleLabel = new QLabel(tr("Project Name"), this);
    mStatusLabel = new QLabel(tr("[status]"), this);
    mStackWidget = new QStackedWidget(this);
    mResumeTab = new QWidget(this);

    QIcon* ico = new QIcon();
    ico->addPixmap(app->awesome()->icon(fa::folderopeno).pixmap(64, 64), QIcon::Normal, QIcon::On);
    ico->addPixmap(app->awesome()->icon(fa::foldero).pixmap(64, 64), QIcon::Normal, QIcon::Off);
    mToggleBrowserButton = new QPushButton(*ico, tr("Projects"), this);
    mToggleBrowserButton->setCheckable(true);

    QWidget* stretcher = new QWidget(this);
    stretcher->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    mToolBar = new QToolBar(this);
    mToolBar->addWidget(mTitleLabel);
    mToolBar->addWidget(mStatusLabel);
    mToolBar->addWidget(stretcher);
    mToolBar->addAction(app->awesome()->icon(fa::pencil),tr("Edit project information"), this, SLOT(showSettings()));
    mToolBar->addAction(app->awesome()->icon(fa::userplus),tr("Add subject/sample to the project"), this, SLOT(showSettings()));
    mToolBar->addAction(app->awesome()->icon(fa::cog),tr("Create a new analysis"), this, SLOT(showSettings()));
    mToolBar->addAction(app->awesome()->icon(fa::calendar),tr("Add a custom event to the history of the project"), this, SLOT(showSettings()));
    mToolBar->addAction(app->awesome()->icon(fa::paperclip),tr("Add attachment to the project"), this, SLOT(showSettings()));

    mSectionBar = new QListWidget(this);
    mSectionBar->addItem(new QListWidgetItem(app->awesome()->icon(fa::barchart), tr("Resume")));
    mSectionBar->addItem(new QListWidgetItem(app->awesome()->icon(fa::user), tr("Subjects")));
    mSectionBar->addItem(new QListWidgetItem(app->awesome()->icon(fa::cog), tr("Tasks")));
    mSectionBar->addItem(new QListWidgetItem(app->awesome()->icon(fa::file), tr("Files")));
    mSectionBar->setIconSize(QSize(64,64));
    mSectionBar->setViewMode(QListView::IconMode);
    mSectionBar->setSelectionMode(QAbstractItemView::SingleSelection);
    mSectionBar->setSelectionBehavior(QAbstractItemView::SelectRows);
    mSectionBar->setDragDropMode(QAbstractItemView::NoDragDrop);


    // Some Theme customization
    mToggleBrowserButton->setFlat(true);
    mTitleLabel->setFont(QFont( "Arial", 18, QFont::Bold));


    // Create pages
    mResumePage = new ResumeWidget(this);
    mSubjectPage = new QTableWidget(this);
    mTaskPage = new QTableWidget(this);
    mFilePage = new QTreeView(this);

    mStackWidget->addWidget(mResumePage);
    mStackWidget->addWidget(mSubjectPage);
    mStackWidget->addWidget(mTaskPage);
    mStackWidget->addWidget(mFilePage);
    mStackWidget->setCurrentWidget(mResumePage);
    // TODO : currentWidget not set properly the selection in the widget


    // Set main layout
    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->addWidget(mToggleBrowserButton, 0, 0);
    mainLayout->addWidget(mSectionBar, 1, 0);
    mainLayout->addWidget(mToolBar, 0, 1);
    mainLayout->addWidget(mStackWidget, 1, 1);
    mSectionBar->setMaximumWidth(75);
    setLayout(mainLayout);


    // Create Signals/Slots connections
    connect(mSectionBar, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(onSectionChanged(QListWidgetItem *, QListWidgetItem *)));
}


void ProjectWidget::onSectionChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    // TODO : switch stacked widget according to selected index T_T
    // idx = mSectionBar->indexOf(current);
    // mStackWidget->setCurrentIndex(idx);

}


} // END namespace projectview
