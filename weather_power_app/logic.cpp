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
    QString name = "12hforecast.xml";
    QFile data_file(name);
    if (!data_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    data_file.write(reply->readAll());
    data_file.close();
    parseData(name);
}

void Logic::parseData(QString file)
{
    QFile data(file);
    if (!data.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QXmlStreamReader* xmlReader = new QXmlStreamReader(&data);

    while(!xmlReader->atEnd() && !xmlReader->hasError()) {
            // Read next element
            QXmlStreamReader::TokenType token = xmlReader->readNext();

            //If token is StartElement - read it
            if(token == QXmlStreamReader::StartElement) {
                if(xmlReader->name() == "Time") {
                    qDebug() << xmlReader->readElementText();
                }

                if(xmlReader->name() == "ParameterName") {
                    qDebug() << xmlReader->readElementText();
                }

                if(xmlReader->name() == "ParameterValue") {
                    qDebug() << xmlReader->readElementText();
                }
            }
    }
}

