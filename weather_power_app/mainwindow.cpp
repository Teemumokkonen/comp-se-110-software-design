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
    variable_id.clear();
    startDate = ui->calender->selectedDate();
    howLong = ui->howManyDays->value();
    endDate = startDate.addDays(howLong);
    ui->data_label->setText(startDate.toString("dd.MM.yyyy") + " to "
                           + endDate.toString("dd.MM.yyyy"));

    for(int i = 0; i < 7; i++) {
        QListWidgetItem* checkers = ui->listWidget->item(i);
        if(checkers->checkState() == Qt::Checked) {
            variable_id.push_back(id_list.at(i));
        }
    }

    // id for data that user has chosen. See logic.h for all working ids.
    emit sendDateInformation(startDate, endDate, variable_id);

}
