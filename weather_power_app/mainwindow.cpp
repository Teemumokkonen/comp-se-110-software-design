#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    this->setFixedSize(window_width, window_height);
    connect(ui->calender, &QCalendarWidget::clicked, this, &MainWindow::getDate);

    calender2 = new QCalendarWidget;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getDate()
{
   startDate = ui->calender->selectedDate();
   howLong = ui->howManyDays->value(); //.toInt();
   QString s = QString::number(howLong);
   ui->data_label->setText(startDate.toString("dd.MM.yyyy") + " + " + s + " days");

}

