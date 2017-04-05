#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Get Regovar's core instance
    regovar->init();
    // Init HMI
    buildMenu();

    // construct widget
    mHomeTabWidget = buildHomeTab();
    mLoginWidget   = new LoginWidget(this);
    mStackWidget   = new QStackedWidget(this);

    // add widget to the stack
    mStackWidget->addWidget(mLoginWidget);
    mStackWidget->addWidget(mHomeTabWidget);

    //create connection
    connect(mHomeTabWidget, SIGNAL(login(QString&, QString&)),this, SLOT(checkAuthent(QString&, QString&)));

    // set stack to the central widget
    setCentralWidget(mStackWidget);

    // set current stack widget
    if (!regovar->currentUser().isValid())
        mStackWidget->setCurrentWidget(mLoginWidget);
    else
        mStackWidget->setCurrentWidget(mHomeTabWidget);


    restoreSettings();
}

MainWindow::~MainWindow()
{

}





void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void MainWindow::restoreSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}


QWidget* MainWindow::buildHomeTab()
{
    QQuickView* view = new QQuickView();
    QWidget* container = QWidget::createWindowContainer(view, this);
    container->setMinimumSize(200, 200);
    container->setFocusPolicy(Qt::TabFocus);
    view->setSource(QUrl("qrc:/qml/home.qml"));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->rootContext()->setContextProperty("main", this);

    return container;
}

void MainWindow::buildMenu()
{

    mMenuBar = new QMenuBar(0);

    // Regovar menu
    QAction *homeAct = new QAction(tr("&Home"), this);
    homeAct->setShortcuts(QKeySequence::New);
    homeAct->setStatusTip(tr("Go to the start screen"));

    QAction *myProfileAct = new QAction(tr("&My profile"), this);
    myProfileAct->setStatusTip(tr("Edit my profile"));

    QAction *settingsAct = new QAction(tr("&Settings"), this);
    settingsAct->setStatusTip(tr("Edit Regovar application's settings"));

    QAction *fullscreenAct = new QAction(tr("&Fullscreen"), this);
    fullscreenAct->setShortcuts(QKeySequence::FullScreen);
    fullscreenAct->setStatusTip(tr("Display the application in fullscreen"));

    QAction *quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Close the application"));

    QMenu *regovarMenu = mMenuBar->addMenu(tr("&Regovar"));
    regovarMenu->addAction(homeAct);
    regovarMenu->addAction(fullscreenAct);
    regovarMenu->addSeparator();
    regovarMenu->addAction(myProfileAct);
    regovarMenu->addAction(settingsAct);
    regovarMenu->addSeparator();
    regovarMenu->addAction(quitAct);


    // TODO : loading "module menu" dynamicaly from library
    //    menuBar->addMenu(tr("&Project"));
    //    menuBar->addMenu(tr("&Subject"));
    //    menuBar->addMenu(tr("&Pipeline"));

    // TODO : these generics menus may be complete by dynamic module
    //    menuBar->addMenu(tr("&Statistics"));
    //    menuBar->addMenu(tr("&Tools"));
    //    menuBar->addMenu(tr("&Administration"));

    // Help menu
    QAction *beginnerGuideAct = new QAction(tr("&Beginner's guide"), this);
    beginnerGuideAct->setStatusTip(tr("Watch the beginner's guide (online video ~5min)"));

    QAction *tutorialsAct = new QAction(tr("&Tutorials"), this);
    tutorialsAct->setStatusTip(tr("Online's tutorials"));

    QAction *aboutAct = new QAction(tr("&About Regovar"), this);
    aboutAct->setStatusTip(tr("Informations about the application"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    QMenu *helpMenu = mMenuBar->addMenu(tr("&Help"));
    helpMenu->addAction(beginnerGuideAct);
    helpMenu->addAction(tutorialsAct);
    helpMenu->addAction(fullscreenAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);


    // Display the menu in the main window
    setMenuBar(mMenuBar);
}






void MainWindow::about()
{
    QMessageBox::about(this, tr("About Regovar"),
                       tr("<b>Regovar</b> application is the best. "
                          "As you can see with this perfect dialog box."));
}

void MainWindow::checkAuthent(QString& login, QString& password)
{
    qDebug() << tr("checkAuthent login:%1 pwd:%2").arg(login, password);
    regovar->login(login, password);
}

















void MainWindow::closeEvent(QCloseEvent *)
{
    writeSettings();
}
