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
    void getDataTimes(QDate startDate, QDate endDate, std::vector<int> i);

private slots:

    void fileIsReady(QNetworkReply* reply);

    void parseData(QString file);

    void draw_graph();


private:
    MainWindow w_;
    QNetworkAccessManager manager_;
    // Ids for getting needed data from fingrid
    Data* data_;
    Preference* preference_;
    int variable_id;
    std::vector<int> temp_id;
    std::vector<int> electricity_id = {ELECTRICITY_CONSUMPTION, ELECTRICITY_PRODUCTION, WIND_POWER_PRODUCTION,
                           NUCLEAR_POWER_PRODUCTION, HYDRO_POWER_PRODUCTION, ELECTRICITY_FORECAST, TENTATIVE_PRODUCTION_PREDICTION};


    std::vector<int> weather_id = {TEMPERATURE_ID, OBSERVED_WIND, OBSERVED_CLOUDINESS, PREDICTED_WIND, PREDICTED_TEMPERATURE, AVERAGE_TEMP, MAX_TEMP, MIN_TEMP};
    std::vector<QString> callouts = {"temperature","windspeedms","TotalCloudCover","temperature","windspeedms", "TA_PT1H_AVG", "TA_PT1H_MAX", "TA_PT1H_MIN"};
    // used to send data to Data
    double value;
    int id;
    std::string time;

};
#endif // LOGIC_H
