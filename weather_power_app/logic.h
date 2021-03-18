#ifndef LOGIC_H
#define LOGIC_H

#include "mainwindow.h"
#include <QTemporaryFile>
#include <QNetworkReply>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QXmlStreamReader>
#include <QFile>
#include <string>
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
    void getDataTimes(QDate startDate, QDate endDate);

private slots:

    void fileIsReady(QNetworkReply* reply);

    void parseData(QString file);


private:
    MainWindow w_;
    QNetworkAccessManager manager_;
    std::vector<int> ids = {193, 166, 241, 192, 181, 188, 191};
};
#endif // LOGIC_H
