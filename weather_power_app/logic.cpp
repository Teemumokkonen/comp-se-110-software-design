#include "logic.h"

QString FINGRID_API_KEY = "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6";

Logic::Logic()
{
}

void Logic::init(){

    preference_ = new Preference();
    preference_->read_preference_file();
    data_ = new Data();
    w_.show();
    connect(&w_, &MainWindow::sendDateInformation, this, &Logic::getDataTimes);
    connect(&manager_, &QNetworkAccessManager::finished, this, &Logic::fileIsReady);
    connect(&w_, &MainWindow::sendPrefInfo, this, &Logic::prefButtonclicked);
    connect(&w_, &MainWindow::sendSaveInfo, this, &Logic::saveButtonclicked);
    setCheckBoxText();
}

void Logic::chanceSeries()
{

}

Logic::~Logic()
{
}


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
    // data is read from the temp file element by element
    while(!xmlReader->atEnd() && !xmlReader->hasError()) {
        // Read next element
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if(token == QXmlStreamReader::StartElement) {
            // here is checker if data is from fmi-api
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
            // adding to datastructure here
            data_->push_data(temp_id.at(variable_id), value, time);

            }
            // here is checker if data is from fingrid-api
            else if(xmlReader->name() == "event") {
                while(true) {
                    xmlReader->readNext();
                    if(xmlReader->name() == "value") {
                        value = xmlReader->readElementText().toDouble();

                    }
                    if(xmlReader->name() == "start_time") {
                        time = xmlReader->readElementText().toStdString();
                    }
                    if (xmlReader->name() == "end_time") {
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

    // fetch data from data object and add it to chart.
    data_->show_data(temp_id.at(variable_id), series);
    w_.getChart()->addSeries(series);

    // add y-axis for the graph, 100 is here because all id_from fingrid are > 100 and from fmi < 100
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

    // attach y-axis to certain series so that multiple lineseries can be dispayel at the same time
    series->attachAxis(axisY);
    axisY_list.push_back(axisY);

    // set one x-axis for the graph
    if(date_checker == false) {
        axisX = new QDateTimeAxis;
        axisX->setTickCount(10);
        axisX->setFormat("dd MM yyyy h:mm");
        axisX->setTitleText("Date");
        w_.getChart()->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);
        date_checker = true;
    }
}

void Logic::prefButtonclicked(int slot, bool status, std::vector<int> boxes)
{
    std::vector<int> ids = preference_->get_entry(slot);
    if(status == false) {
        w_.retrievePrefBoxes(ids);

    } else {
        if (!ids.empty()) {
            preference_->remove_preference_entry(slot);
            w_.checkBoxText(slot, "Empty");

        } else {
            preference_->new_preference_entry(slot, boxes);
            w_.checkBoxText(slot, "Has Data");
        }
    }
}

void Logic::saveButtonclicked(int slot, bool status, std::vector<int> boxes)
{
    // Tarvii sen toisen vittusaatanan!!!
    // tähän tulee kun klikataan save buttoneita
}

void Logic::setCheckBoxText()
{
    for(int i = 1; i < 5; i++) {
        if(i == 1 || i == 2) {
            if(preference_->get_entry(i).empty()) {
                w_.checkBoxText(i, "Empty");

            } else {
                w_.checkBoxText(i, "Has Data");
            }
        } else {
            int j = i - 2;
            /*if(??(j).empty()) {
                w_.checkBoxText(i, "Empty");

            } else {
                w_.checkBoxText(i, "Has Data");
            }*/
            // TULEE SAVET toisesta vittusaatanasta
            // settaa savejen tekstit slotit 3 ja 4.
        }
    }
}

void Logic::getDataTimes(QDate start, QDate end, std::vector<int> id_list, QString place)
{
    // check if graph has old axis and remove them if there are any.
    if(axisX != nullptr) {
        w_.getChart()->removeAxis(axisX);
    }

    for(auto element : axisY_list) {
        if(element != nullptr) {
            w_.getChart()->removeAxis(element);
        }
    }
    axisY_list.clear();

    date_checker = false;

    // clear the data object so that there can be added new data.
    data_->clear_data();
    temp_id = id_list;
    variable_id = 0;


    // loop over selected ids.
    for(unsigned long int i = 0; i < id_list.size(); i++) {
        // find if id belongs to weather or electricity data.
        auto it = std::find(electricity_id.begin(), electricity_id.end(), id_list.at(i));
        if (it != electricity_id.end()){
            // if selected data is forecast data.
            if(id_list.at(i) == ELECTRICITY_FORECAST or id_list.at(i) == TENTATIVE_PRODUCTION_PREDICTION) {
                // declaring argument of time()
                QDateTime current =  current.currentDateTime();;
                QDateTime next = current.addDays(1);
                qDebug() << next.toString("yyyy-MM-ddTHH:mm:ssZ");
                QUrl url = QUrl("https://api.fingrid.fi/v1/variable/" + QVariant(id_list.at(i)).toString() +
                                "/events/xml?start_time=" + current.toString("yyyy-MM-ddTHH:mm:ssZ")  + "&end_time=" + next.toString("yyyy-MM-ddTHH:mm:ssZ"));
                QNetworkRequest request(url);
                request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
                manager_.get(request);
            }
            // if selected data is normal electricity data.
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
            // if selected data is average, mininum or max temp for a month
            if ((id_list.at(i) <= 8 and id_list.at(i) >= 6)) {
                auto it = std::find(weather_id.begin(), weather_id.end(), id_list.at(i));
                int index = std::distance(weather_id.begin(), it);
                int days = start.daysInMonth();
                QUrl url = QUrl("https://opendata.fmi.fi/wfs?request=getFeature&version=2.0.0&storedquery_id=fmi::observations::weather::hourly::simple&place=" + place + "&starttime="
                                 + start.toString("yyyy-MM-01T00:00:00Z")  + "&endtime=" + end.toString("yyyy-MM") + "-"
                "" + QString::number(days) + end.toString("T00:00:00Z") + "&parameters=" + callouts.at(index));
                QNetworkRequest request(url);
                request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
                manager_.get(request);
            }

            // if selected data is measured temp, windspeed or cloud coverage
            else if(id_list.at(i) <= 3 and id_list.at(i) >= 1) {
                auto it = std::find(weather_id.begin(), weather_id.end(), id_list.at(i));
                int index = std::distance(weather_id.begin(), it);
                QUrl url = QUrl("https://opendata.fmi.fi/wfs?request=getFeature&version=2.0.0&storedquery_id=fmi::observations::weather::simple&place=" + place + "&starttime="
                                 + start.toString("yyyy-MM-ddT00:00:00Z")  + "&endtime=" + end.toString("yyyy-MM-ddT00:00:00Z") + "&parameters=" + callouts.at(index));
                QNetworkRequest request(url);
                request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
                manager_.get(request);
            }
            // if selected data is forecast.
            else {

                QDateTime current =  current.currentDateTime();
                QDateTime next  = current.addDays(1);
                auto it = std::find(weather_id.begin(), weather_id.end(), id_list.at(i));
                int index = std::distance(weather_id.begin(), it);


                QUrl url = QUrl("https://opendata.fmi.fi/wfs?request=getFeature&version=2.0.0&storedquery_id=fmi::forecast::hirlam::surface::point::simple&&"
                                "place=" + place + "&timestep=30&starttime="
                                 + current.toString("yyyy-MM-ddTHH:mm:ssZ")  + "&endtime="
                                 + next.toString("yyyy-MM-ddTHH:mm:ssZ")  + "&parameters=" + callouts.at(index));

                QNetworkRequest request(url);
                request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
                manager_.get(request);
            }
        }
    }
}
