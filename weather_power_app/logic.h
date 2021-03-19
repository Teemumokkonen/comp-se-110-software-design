#ifndef LOGIC_H
#define LOGIC_H

#include "mainwindow.h"
#include "data.h"

#include <QTemporaryFile>
#include <QNetworkReply>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QXmlStreamReader>
#include <QFile>
#include <string>
#include <ctime>



const int ELECTRICITY_CONSUMPTION = 193;
const int ELECTRICITY_FORECAST = 166;
const int TENTATIVE_PRODUCTION_PREDICTION = 241;
const int ELECTRICITY_PRODUCTION = 192;
const int WIND_POWER_PRODUCTION = 181;
const int NUCLEAR_POWER_PRODUCTION = 188;
const int HYDRO_POWER_PRODUCTION = 191;



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
    void getDataTimes(QDate startDate, QDate endDate, int i);

private slots:

    void fileIsReady(QNetworkReply* reply);

    void parseData(QString file);


private:
    MainWindow w_;
    QNetworkAccessManager manager_;
    // Ids for getting needed data from fingrid
    std::vector<int> ids = {ELECTRICITY_CONSUMPTION, ELECTRICITY_FORECAST, TENTATIVE_PRODUCTION_PREDICTION,
                           ELECTRICITY_PRODUCTION, WIND_POWER_PRODUCTION, NUCLEAR_POWER_PRODUCTION, HYDRO_POWER_PRODUCTION};
    Data* data_;
    int variable_id;

    // used to send data to Data
    double value;
    int id;
    std::string time;

};
#endif // LOGIC_H
