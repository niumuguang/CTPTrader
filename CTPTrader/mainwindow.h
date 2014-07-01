#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./cmysql_api.h"
#include <QPainter>
#include <widget.h>
#include <help.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Help help;

protected:
    CMySQL_Api* m_mysql;


private slots:
    void on_ConnectTest_clicked();

};

#endif // MAINWINDOW_H
