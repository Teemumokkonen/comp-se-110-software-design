#ifndef LOGIC_H
#define LOGIC_H
#include "mainwindow.h"
#include <QTemporaryFile>
#include <QNetworkReply>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QXmlStreamReader>
#include <QFile>
#include "preference.h"


class Logic : public QObject
{
    Q_OBJECT

public:
    Logic();
    ~Logic();
    void init();

private slots:

    void fileIsReady(QNetworkReply* reply);

    void parseData(QString file);

private:
    MainWindow w_;
    QNetworkAccessManager manager_;


};
#endif // LOGIC_H
