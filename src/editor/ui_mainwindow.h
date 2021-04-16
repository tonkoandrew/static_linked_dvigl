/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_project;
    QAction *actionSave_All;
    QAction *actionExit;
    QAction *actionAaa;
    QAction *actionNew_File_or_Project;
    QAction *actionRedo;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionSelect_All;
    QAction *actionSave;
    QAction *actionRun;
    QAction *actionStop;
    QAction *actionPause;
    QAction *actionRebuild;
    QAction *actionClean;
    QAction *actionImport_Model;
    QAction *actionAbout;
    QAction *actionAddWindow;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuBuild;
    QMenu *menuWindow;
    QMenu *menuHelp;
    QMenu *menuAssets;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(559, 499);
        actionOpen_project = new QAction(MainWindow);
        actionOpen_project->setObjectName(QString::fromUtf8("actionOpen_project"));
        actionSave_All = new QAction(MainWindow);
        actionSave_All->setObjectName(QString::fromUtf8("actionSave_All"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAaa = new QAction(MainWindow);
        actionAaa->setObjectName(QString::fromUtf8("actionAaa"));
        actionNew_File_or_Project = new QAction(MainWindow);
        actionNew_File_or_Project->setObjectName(QString::fromUtf8("actionNew_File_or_Project"));
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionSelect_All = new QAction(MainWindow);
        actionSelect_All->setObjectName(QString::fromUtf8("actionSelect_All"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionRun = new QAction(MainWindow);
        actionRun->setObjectName(QString::fromUtf8("actionRun"));
        actionStop = new QAction(MainWindow);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        actionPause = new QAction(MainWindow);
        actionPause->setObjectName(QString::fromUtf8("actionPause"));
        actionRebuild = new QAction(MainWindow);
        actionRebuild->setObjectName(QString::fromUtf8("actionRebuild"));
        actionClean = new QAction(MainWindow);
        actionClean->setObjectName(QString::fromUtf8("actionClean"));
        actionImport_Model = new QAction(MainWindow);
        actionImport_Model->setObjectName(QString::fromUtf8("actionImport_Model"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAddWindow = new QAction(MainWindow);
        actionAddWindow->setObjectName(QString::fromUtf8("actionAddWindow"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 559, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuBuild = new QMenu(menuBar);
        menuBuild->setObjectName(QString::fromUtf8("menuBuild"));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuAssets = new QMenu(menuBar);
        menuAssets->setObjectName(QString::fromUtf8("menuAssets"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuAssets->menuAction());
        menuBar->addAction(menuBuild->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew_File_or_Project);
        menuFile->addAction(actionOpen_project);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_All);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionAaa);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionSelect_All);
        menuBuild->addAction(actionRun);
        menuBuild->addAction(actionStop);
        menuBuild->addAction(actionPause);
        menuBuild->addSeparator();
        menuBuild->addAction(actionRebuild);
        menuBuild->addAction(actionClean);
        menuWindow->addAction(actionAddWindow);
        menuHelp->addAction(actionAbout);
        menuAssets->addAction(actionImport_Model);
        mainToolBar->addAction(actionExit);
        mainToolBar->addAction(actionOpen_project);
        mainToolBar->addAction(actionSave_All);
        mainToolBar->addAction(actionAddWindow);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Dvigl editor", nullptr));
        actionOpen_project->setText(QCoreApplication::translate("MainWindow", "Open Project", nullptr));
        actionSave_All->setText(QCoreApplication::translate("MainWindow", "Save All", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAaa->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
        actionNew_File_or_Project->setText(QCoreApplication::translate("MainWindow", "New Project", nullptr));
        actionRedo->setText(QCoreApplication::translate("MainWindow", "Redo", nullptr));
        actionCut->setText(QCoreApplication::translate("MainWindow", "Cut", nullptr));
        actionCopy->setText(QCoreApplication::translate("MainWindow", "Copy", nullptr));
        actionPaste->setText(QCoreApplication::translate("MainWindow", "Paste", nullptr));
        actionSelect_All->setText(QCoreApplication::translate("MainWindow", "Select All", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        actionRun->setText(QCoreApplication::translate("MainWindow", "Run", nullptr));
        actionStop->setText(QCoreApplication::translate("MainWindow", "Stop", nullptr));
        actionPause->setText(QCoreApplication::translate("MainWindow", "Pause", nullptr));
        actionRebuild->setText(QCoreApplication::translate("MainWindow", "Rebuild", nullptr));
        actionClean->setText(QCoreApplication::translate("MainWindow", "Clean", nullptr));
        actionImport_Model->setText(QCoreApplication::translate("MainWindow", "Import Model", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        actionAddWindow->setText(QCoreApplication::translate("MainWindow", "Add Window", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuBuild->setTitle(QCoreApplication::translate("MainWindow", "Build", nullptr));
        menuWindow->setTitle(QCoreApplication::translate("MainWindow", "Window", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        menuAssets->setTitle(QCoreApplication::translate("MainWindow", "Assets", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
