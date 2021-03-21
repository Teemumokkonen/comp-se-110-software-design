#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    connect(ui->calender, &QCalendarWidget::clicked, this, &MainWindow::getDate);
    connect(ui->checkBox, &QCheckBox::clicked, this, &MainWindow::drawChart);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawChart()
{
    Graph* graphWindow = new Graph();
    graphWindow->addGraph();

    //Yritetään lisätä uuteen tabiin
    tab = new QTabWidget(this);
    tab->addTab(graphWindow, "Graafi 1");
    this->layout()->addWidget(tab);
    this->setLayout(layout());
}

void MainWindow::getDate()
{
   startDate = ui->calender->selectedDate();
   howLong = ui->howManyDays->value();
   endDate = startDate.addDays(howLong);
   ui->data_label->setText(startDate.toString("dd.MM.yyyy") + " to "
                           + endDate.toString("dd.MM.yyyy"));

   // id for data that user has chosen. See logic.h for all working ids.
   int variable_id = 193;
   emit sendDateInformation(startDate, endDate, variable_id);

}
