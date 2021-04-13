#include "logic.h"

QString FINGRID_API_KEY = "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6";

Logic::Logic()
{
}

void Logic::init(){

    preference_ = new Preference();
    //preference_->create_save_file();
    data_ = new Data();
    w_.show();
    connect(&w_, &MainWindow::sendDateInformation, this, &Logic::getDataTimes);
    connect(&manager_, &QNetworkAccessManager::finished, this, &Logic::fileIsReady);


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
    qDebug() << temp_id.at(variable_id);
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
                            time = xmlReader->readElementText().toStdString();
                        }
                        if(xmlReader->name() == "ParameterName") {
                            //qDebug() << xmlReader->readElementText();
                        }
                        if (xmlReader->name() == "ParameterValue") {
                            value = xmlReader->readElementText().toDouble();
                            break;
                        }
                }
                    // \n is here now for debugging purposes.
                    // adding to datastructure here
            data_->push_data(temp_id.at(variable_id), value, time);
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
                data_->push_data(temp_id.at(variable_id), value, time);
            }
        }
    }
    draw_graph();
    variable_id++;
}

void Logic::draw_graph()
{
    //lineseries
    QSplineSeries *series  = new QSplineSeries;
    QValueAxis * axisY = new QValueAxis;

    data_->show_data(temp_id.at(variable_id), series);
    w_.getChart()->addSeries(series);

    if(temp_id.at(variable_id) < 100) {
        auto it = std::find(weather_id.begin(), weather_id.end(), temp_id.at(variable_id));
        int index = std::distance(weather_id.begin(), it);
        series->setName(labels.at(index));
        axisY->setLabelFormat("%i");
        axisY->setTitleText(labels.at(index) + " (" + units.at(index) + ")");
        w_.getChart()->addAxis(axisY, Qt::AlignLeft);

    }

    else  {
        auto it = std::find(electricity_id.begin(), electricity_id.end(), temp_id.at(variable_id));
        int index = std::distance(electricity_id.begin(), it);
        series->setName(electricity_labels.at(index));
        axisY->setLabelFormat("%i");
        axisY->setTitleText(electricity_labels.at(index) + " (MWh/h)");
        w_.getChart()->addAxis(axisY, Qt::AlignLeft);

    }

    series->attachAxis(axisY);
    axisY_list.push_back(axisY);

    if(date_checker == false) {
        axisX = new QDateTimeAxis;
        axisX->setTickCount(10);
        axisX->setFormat("dd MM yyyy");
        axisX->setTitleText("Date");
        w_.getChart()->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);
        date_checker = true;
    }




    //w_.getChart()->createDefaultAxes();
}

void Logic::getDataTimes(QDate start, QDate end, std::vector<int> id_list, QString place)
{
    if(axisX != nullptr) {
        w_.getChart()->removeAxis(axisX);
    }

    for(auto element : axisY_list) {
        if(element != nullptr) {
            w_.getChart()->removeAxis(element);
        }
    }

    date_checker = false;

    data_->clear_data();
    temp_id = id_list;
    variable_id = 0;
    // loop over selected ids.
    for(unsigned long int i = 0; i < id_list.size(); i++) {
        auto it = std::find(electricity_id.begin(), electricity_id.end(), id_list.at(i));

        if (it != electricity_id.end()){

            if(id_list.at(i) == ELECTRICITY_FORECAST or id_list.at(i) == TENTATIVE_PRODUCTION_PREDICTION) {
                // declaring argument of time()
                QDateTime current =  current.currentDateTime();;
                QDateTime next = current.addDays(1);
                QUrl url = QUrl("https://api.fingrid.fi/v1/variable/" + QVariant(id_list.at(i)).toString() +
                                "/events/xml?start_time=" + current.toString("yyyy-MM-ddTHH:mm:ssZ")  + "&end_time=" + next.toString("yyyy-MM-ddTHH:mm:ssZ"));
                    QNetworkRequest request(url);
                    request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
                    manager_.get(request);
            }
            else {
                QUrl url = QUrl("https://api.fingrid.fi/v1/variable/" + QVariant(id_list.at(i)).toString() +
                                "/events/xml?start_time=" + start.toString("yyyy-MM-ddT00%3A00%3A00Z")  + "&end_time=" + end.toString("yyyy-MM-ddT00%3A00%3A00Z"));
                    //https://api.fingrid.fi/v1/variable/241/events/xml?start_time=2021-01-01T22%3A00%3A00Z&end_time=2021-03-17T22%3A00%3A00Z
                    QNetworkRequest request(url);
                    request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
                    manager_.get(request);
            }
        }


        else {
            //model for getting temp data averages, min and max from certain period of time.
            if (id_list.at(i) <= 8 and id_list.at(i) >= 6) {
                //"http://opendata.fmi.fi/wfs?request=getFeature&version=2.0.0&storedquery_id=fmi::observations::weather::simple&place=Pirkkala&timestep=30&parameters=t2m,ws_10min,n_man"
                auto it = std::find(weather_id.begin(), weather_id.end(), id_list.at(i));
                int index = std::distance(weather_id.begin(), it);
                QUrl url = QUrl("https://opendata.fmi.fi/wfs?request=getFeature&version=2.0.0&storedquery_id=fmi::observations::weather::hourly::simple&place=" + place + "&starttime="
                                 + start.toString("yyyy-MM-ddT00:00:00Z")  + "&endtime=" + end.toString("yyyy-MM-ddT00:00:00Z") + "&parameters=" + callouts.at(index));
                QNetworkRequest request(url);
                request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
                manager_.get(request);
            }
            else if(id_list.at(i) <= 3 and id_list.at(i) >= 1) {
                auto it = std::find(weather_id.begin(), weather_id.end(), id_list.at(i));
                int index = std::distance(weather_id.begin(), it);
                QUrl url = QUrl("https://opendata.fmi.fi/wfs?request=getFeature&version=2.0.0&storedquery_id=fmi::observations::weather::simple&place=" + place + "&starttime="
                                 + start.toString("yyyy-MM-ddT00:00:00Z")  + "&endtime=" + end.toString("yyyy-MM-ddT00:00:00Z") + "&parameters=" + callouts.at(index));
                QNetworkRequest request(url);
                request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
                manager_.get(request);
            }
            else {

                QDateTime current =  current.currentDateTime();
                QDateTime next = current.addDays(1);
                auto it = std::find(weather_id.begin(), weather_id.end(), id_list.at(i));
                int index = std::distance(weather_id.begin(), it);
                QUrl url = QUrl("https://opendata.fmi.fi/wfs?request=getFeature&version=2.0.0&storedquery_id=fmi::observations::weather::simple&place=" + place + "&starttime="
                                 + current.toString("yyyy-MM-ddTHH:mm:ssZ")  + "&endtime=" + next.toString("yyyy-MM-ddTHH:mm:ssZ") + "&parameters=" + callouts.at(index));
                QNetworkRequest request(url);
                request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
                manager_.get(request);

            }
        }
    }
}
