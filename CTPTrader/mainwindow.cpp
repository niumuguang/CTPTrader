#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    widget *native = new widget(&help, this);
    QLabel *nativeLabel = new QLabel(tr("Native"));
    nativeLabel->setAlignment(Qt::AlignHCenter);


    //this->layout()->addWidget(native, 200, 20);
    QLayout *layout = this->layout();
    native->pos().setX(80);
    native->pos().rx() = 90;
    //&(native->pos().x()) = 100;

    int sss = native->pos().x();
    int fff = native->x();
    layout->addWidget(native);
    //layout->addWidget(native, 200, 20);
    //layout->addWidget(openGL, 0, 1);
    //layout->addWidget(nativeLabel, 100, 0);
    //layout->addWidget(openGLLabel, 1, 1);
    this->setLayout(layout);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), native, SLOT(animate()));
    //connect(timer, SIGNAL(timeout()), openGL, SLOT(animate()));
    timer->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ConnectTest_clicked()
{
//     m_mysql = new CMySQL_Api("root","niumuguang","FutureDB","ag1412");
//     bool ss = m_mysql->IsConnect();

//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    //int side = qMin(Width(),)
//    //painter.setViewport();
//    painter.setPen(QPen(Qt::black,12,Qt::DashDotDotLine,Qt::RoundCap));
//    painter.drawEllipse(0,0,10,20);
//    painter.drawLine(0,0,200,200);
//    this->update();
}
