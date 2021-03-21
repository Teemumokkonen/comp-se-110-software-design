#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    QChartView *chartView;

    //Luodaan graafi ja annetaan sille otsikko
    chart_ = new QChart();
    chart_->setTitle("Täs ois graafi");
    chart_->createDefaultAxes(); //Skaalaa automaattisesti x - y akselin
    //chart->legend()->hide(); //voidaan piilottaa piirrettyjen graafien tiedot

    chartView = new QChartView(chart_);
    ui->gridLayout->addWidget(chartView, 1, 0);
    m_charts << chartView;

    connect(ui->calender, &QCalendarWidget::clicked, this, &MainWindow::getDate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QChart *MainWindow::getChart() const
{
    return chart_;
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
