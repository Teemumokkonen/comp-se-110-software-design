#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::fileIsReady);
    manager->get(QNetworkRequest(QUrl("http://opendata.fmi.fi/wfs?request=getFeature&version=2.0.0&storedquery_id=fmi::observations::weather::simple&place=Pirkkala&timestep=30&parameters=t2m,ws_10min,n_man")));

    connect(ui->calender, &QCalendarWidget::clicked, this, &MainWindow::getDate);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getDate()
{
   startDate = ui->calender->selectedDate();
   howLong = ui->howManyDays->value();
   endDate = startDate.addDays(howLong);
   ui->data_label->setText(startDate.toString("dd.MM.yyyy") + " to "
                           + endDate.toString("dd.MM.yyyy"));

}

//after fetching data from api it will be written to temp file
void MainWindow::fileIsReady( QNetworkReply * reply)
{
    QString name = "12hforecast";
    QTemporaryFile temp_file(name);
    temp_file.open();
    QString path = QDir::tempPath();
    temp_file.write(reply->readAll());
}
