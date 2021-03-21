#include "logic.h"

std::string FINGRID_API_KEY = "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6";

Logic::Logic()
{
}

void Logic::init(){

    preference_ = new Preference();
    preference_->create_save_file();
    data_ = new Data();
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
        //qDebug() << xmlReader->name();
        //qDebug() <<token;
        //qDebug() <<xmlReader->text();
        //If token is StartElement - read it
        if(token == QXmlStreamReader::StartElement) {
            if(xmlReader->name() == "BsWfsElement") {
                    while(true) {
                        xmlReader->readNext();
                        if(xmlReader->name() == "Time") {
                            qDebug() << xmlReader->readElementText();
                        }
                        if(xmlReader->name() == "ParameterName") {
                            qDebug() << xmlReader->readElementText();
                        }
                        if (xmlReader->name() == "ParameterValue") {
                            qDebug() << xmlReader->readElementText() + "\n";
                            break;
                        }
                }
                    // \n is here now for debugging purposes.
                    // adding to datastructure here
            }
            else if(xmlReader->name() == "event") {
                while(true) {
                    xmlReader->readNext();
                    if(xmlReader->name() == "value") {
                        //qDebug() << xmlReader->readElementText();
                        value = xmlReader->readElementText().toDouble();

                    }
                    if(xmlReader->name() == "start_time") {
                        //qDebug() << xmlReader->readElementText();
                        time = xmlReader->readElementText().toStdString();
                    }
                    if (xmlReader->name() == "end_time") {
                        // \n is here now for debugging purposes.
                        //qDebug() << xmlReader->readElementText() + "\n";
                        break;
                    }

                }
                // adding to datastructure here
                data_->push_data(variable_id, value, time);
            }
        }
    }
    data_->show_data(193);
}

void Logic::getDataTimes(QDate start, QDate end, int i)
{

    qDebug() << start.toString("yyyy-MM-ddT00:00:00Z");
    qDebug() << end.toString("yyyy-MM-ddT00:00:00Z");
    // Qurl for the Fingrid api. needs to be modified so that that the correct data can be fetched.

    QUrl url = QUrl("https://api.fingrid.fi/v1/variable/" + QVariant(i).toString() +
                    "/events/xml?start_time=" + start.toString("yyyy-MM-ddT00:00:00Z")  + "&end_time=" + end.toString("yyyy-MM-ddT00:00:00Z"));
    QNetworkRequest request(url);
    request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
    variable_id = i;
    manager_.get(request);
}
