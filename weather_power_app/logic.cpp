#include "logic.h"

Logic::Logic()
{
}

void Logic::init(){

    w_.show();

    connect(&manager_, &QNetworkAccessManager::finished, this, &Logic::fileIsReady);
    manager_.get(QNetworkRequest(QUrl("http://opendata.fmi.fi/wfs?request=getFeature&version=2.0.0&storedquery_id=fmi::observations::weather::simple&place=Pirkkala&timestep=30&parameters=t2m,ws_10min,n_man")));
}

Logic::~Logic()
{
}

//after fetching data from api it will be written to temp file
void Logic::fileIsReady(QNetworkReply* reply)
{
    QString name = "12hforecast";
    QTemporaryFile temp_file(name);
    temp_file.open();
    QString path = QDir::tempPath();
    temp_file.write(reply->readAll());
    qDebug() << "joo";
}
