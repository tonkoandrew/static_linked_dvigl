#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGridLayout>
#include <QLabel>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit_app()));

    mdiArea = new QMdiArea(this);  // инициализируем QMdiArea
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

// int numTabs = ui->tabWidget->count();
// for(int i = 0; i < numTabs; ++i){

//    QLayout* tab_layout =  ui->tabWidget->widget(i)->layout();
//     if(tab_layout != NULL){
//         tab_layout->setMenuBar(ui->menuBar);
//     }
// }

// QToolBar* sceneToolBar = new QToolBar( ui->tabWidget); 
//         sceneToolBar->addAction(ui->actionExit);
//         sceneToolBar->addAction(ui->actionOpen_project);
//         sceneToolBar->addAction(ui->actionSave_All);

// QHBoxLayout* tabWidgetLayout = new QHBoxLayout;
// tabWidgetLayout->addWidget(sceneToolBar);
// ui->tabWidget->setLayout(tabWidgetLayout);


    workerThread = new WorkerThread();
    // connect(workerThread, &WorkerThread::resultReady, this, &handleResults);
    // connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();
}

MainWindow::~MainWindow() {
    delete ui;
}

// void MainWindow::exit_app()
// {
// }

void MainWindow::on_actionExit_triggered()
{
    workerThread->quit();
    workerThread->wait();
    printf("EXITAPP\n");
    close();
}

void MainWindow::on_actionAddWindow_triggered()
{
    QWidget *widget = new QWidget(mdiArea);
    QGridLayout *gridLayout = new QGridLayout(widget);
    widget->setLayout(gridLayout);
    QLabel *label = new QLabel("Hello, I am sub window!!!", widget);
    gridLayout->addWidget(label);
    
    mdiArea->addSubWindow(widget);
    
    widget->setWindowTitle("Sub Window");
    widget->show();
}