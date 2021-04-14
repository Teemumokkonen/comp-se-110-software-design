#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QtCharts>


// ehkä hyödylliset includet:
// Qlistwidget
// QDialog
// Qsignalmapper (voi olla eri nimi)

const int ELECTRICITY_CONSUMPTION = 193;
const int ELECTRICITY_FORECAST = 166;
const int TENTATIVE_PRODUCTION_PREDICTION = 242;
const int ELECTRICITY_PRODUCTION = 192;
const int WIND_POWER_PRODUCTION = 181;
const int NUCLEAR_POWER_PRODUCTION = 188;
const int HYDRO_POWER_PRODUCTION = 191;

const int TEMPERATURE_ID = 1;
const int OBSERVED_WIND = 2;
const int OBSERVED_CLOUDINESS = 3;
const int PREDICTED_WIND = 4;
const int PREDICTED_TEMPERATURE = 5;
const int AVERAGE_TEMP = 6;
const int MAX_TEMP = 7;
const int MIN_TEMP = 8;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTabWidget *tab;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QChart * getChart() const;

signals:
    void sendDateInformation(QDate startDate, QDate EndDate, std::vector<int> id, QString place);

private slots:

    // function to fetch the dates that user gives
    void getDate();

    // saves graph to users local files.
    void saveGraph();

    // changes the Qlistwidgets items checkbox status acording to which ones has been selected.
    void changeButtonStatus();

private:

    QChart* chart_;
    Ui::MainWindow* ui;

    //dates that user inputs to get the data.

    QDate startDate;
    QDate endDate;
    int howLong;
    std::vector<int> id_list = {ELECTRICITY_CONSUMPTION, ELECTRICITY_PRODUCTION, WIND_POWER_PRODUCTION,
                           NUCLEAR_POWER_PRODUCTION, HYDRO_POWER_PRODUCTION, ELECTRICITY_FORECAST, TENTATIVE_PRODUCTION_PREDICTION};

    std::vector<int> weather_id = {TEMPERATURE_ID, OBSERVED_WIND, OBSERVED_CLOUDINESS, PREDICTED_WIND, PREDICTED_TEMPERATURE, AVERAGE_TEMP, MAX_TEMP, MIN_TEMP};
    QList<QChartView *> m_charts;
    std::vector<int> variable_id;

};
#endif // MAINWINDOW_H
