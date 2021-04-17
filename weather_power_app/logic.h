#ifndef LOGIC_H
#define LOGIC_H

#include "mainwindow.h"
#include "data.h"
#include "preference.h"
#include <QTemporaryFile>
#include <QNetworkReply>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QXmlStreamReader>
#include <QFile>
#include <string>
#include <algorithm>
#include <ctime>
#include <map>
#include <QSignalMapper>

class Logic : public QObject
{
    Q_OBJECT

public:
    Logic();
    ~Logic();
    void init();


public slots:

    // takes signal from mainwindow when the date has been
    // set from the calendar
    void getDataTimes(QDate startDate, QDate endDate, std::vector<int> i, QString place);

private slots:


    // Takes https response from api and saves data to temporary file.
    void fileIsReady(QNetworkReply* reply);

    // parses data from temporary file and sends it to data object to saved locally.
    void parseData(QString file, int i);

    // draws graph based on data that user has chosen.
    void draw_graph(int i);

    void calculate_average_temp(double day, int id);


private:
    MainWindow w_;
    QNetworkAccessManager manager_;
    Data* data_;
    Preference* preference_;
    QDateTimeAxis *axisX;
    std::vector<QValueAxis *> axisY_list;

    std::vector<int> temp_id;
    std::vector<double> temp_AVG;
    std::vector<double> temp_AVGmin;
    std::vector<double> temp_AVGmax;
    std::vector<double> temps;
    double all_min;
    double all_max;
    double all_temp;

    // Ids for getting needed data from fingrid
    std::vector<int> electricity_id = {ELECTRICITY_CONSUMPTION, ELECTRICITY_PRODUCTION, WIND_POWER_PRODUCTION,
                           NUCLEAR_POWER_PRODUCTION, HYDRO_POWER_PRODUCTION, ELECTRICITY_FORECAST, TENTATIVE_PRODUCTION_PREDICTION};
    // Ids for saving fmi data
    std::vector<int> weather_id = {TEMPERATURE_ID, OBSERVED_WIND, OBSERVED_CLOUDINESS, PREDICTED_WIND, PREDICTED_TEMPERATURE, AVERAGE_TEMP, MAX_TEMP, MIN_TEMP};

    // name labels for the x and y axis and for lineseries name.
    std::vector<QString> electricity_labels = {"ELECTRICITY CONSUMPTION", "ELECTRICITY PRODUCTION", "WIND POWER PRODUCTION",
                           "NUCLEAR POWER PRODUCTION", "HYDRO POWER PRODUCTION", "ELECTRICITY FORECAST", "TENTATIVE PRODUCTION PREDICTION"};

    std::vector<QString> labels = {"TEMPERATURE", "OBSERVED WIND", "OBSERVED CLOUDINESS", "PREDICTED WIND", "PREDICTED TEMPERATURE", "AVERAGE TEMP", "MAX TEMP", "MIN TEMP"};

    std::vector<QString> units = {"°C","m/s","%","m/s", "°C", "°C", "°C", "°C"};

    // list for all variable callouts for fmi api.
    std::vector<QString> callouts = {"temperature","windspeedms","TotalCloudCover","windspeedms", "temperature", "TA_PT1H_AVG", "TA_PT1H_MAX", "TA_PT1H_MIN"};

    QSignalMapper *signalMapper;

    double value;
    int id;
    std::string time;
    bool date_checker = false;
    std::map<QUrl, int> requestList;

};
#endif // LOGIC_H
