#include "logic.h"

std::string FINGRID_API_KEY = "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6";

Logic::Logic()
{
}

void Logic::init(){

    w_.show();
    connect(&w_, &MainWindow::sendDateInformation, this, &Logic::getDataTimes);
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
                if(xmlReader->name() == "value") {
                    qDebug() << xmlReader->readElementText();
                }
                if(xmlReader->name() == "start_time") {
                    qDebug() << xmlReader->readElementText();
                }
                if(xmlReader->name() == "end_time") {
                    qDebug() << xmlReader->readElementText();
                }

                }
    }
}




void Logic::getDataTimes(QDate start, QDate end)
{

    qDebug() << start.toString("yyyy-MM-dd");
    qDebug() << end.toString("yyyy-MM-dd");
    // Qurl for the Fingrid api. needs to be modified so that that the correct data can be fetched.
    QUrl url = QUrl("https://api.fingrid.fi/v1/variable/193/events/xml?start_time=2021-01-01T22%3A00%3A00Z&end_time=2021-01-12T22%3A00%3A00Z");
    QNetworkRequest request(url);
    request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
    manager_.get(request);


}
