#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include "workerthread.h"


namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

    WorkerThread *workerThread;

private:
    Ui::MainWindow* ui;
	QMdiArea* mdiArea;

private slots:
    void on_actionAddWindow_triggered();
    void on_actionExit_triggered();
};

#endif // MAINWINDOW_H
