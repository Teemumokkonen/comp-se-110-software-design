#include "logic.h"

QString FINGRID_API_KEY = "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6";

Logic::Logic()
{
}

void Logic::init(){

    preference_ = new Preference();
    saves_ = new Saves();
    saves_->read_save_file();
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

    QUrl req = reply->url();
    auto it = requestList.find(req);
    if(it != requestList.end()) {
        QString name = "12hforecast.xml";
        QFile data_file(name);
        if (!data_file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        data_file.write(reply->readAll());
        data_file.close();
        parseData(name, it->second);
    }
}

void Logic::parseData(QString file, int i)
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

                        }
                        if (xmlReader->name() == "ParameterValue") {
                            value = xmlReader->readElementText().toDouble();
                            break;

                        }
                }
            // adding to datastructure here
            data_->push_data(temp_id.at(i), value, time);

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
            data_->push_data(temp_id.at(i), value, time);
            }
        }
    }
    draw_graph(i);
}



void Logic::draw_graph(int i)
{
    //lineseries
    QSplineSeries *series  = new QSplineSeries;
    QValueAxis * axisY = new QValueAxis;
    if(i == 301 || i == 302) {
        data_->show_data(i, series);
    } else {
        // fetch data from data object and add it to chart.
        data_->show_data(temp_id.at(i), series);
    }
    w_.getChart()->addSeries(series);

    //Formats the data
    temp_AVG = data_->get_AVG_temp();
    temp_AVGmin = data_->get_AVG_min();
    temp_AVGmax = data_->get_AVG_max();

    Electricity = data_->get_Electricity();
    Nuclear = data_->get_Nuclear();
    Hydro = data_->get_Hydro();
    Wind = data_->get_Wind();

    // add y-axis for the graph, 100 is here because all id_from fingrid are > 100 and from fmi < 100
    if(i == 301 || i == 302) {
        series->setName("Saved data");
        axisY->setLabelFormat("%i");
        axisY->setTitleText("Saved data");
        w_.getChart()->addAxis(axisY, Qt::AlignLeft);

    } else if(temp_id.at(i) < 100) {
        auto it = std::find(weather_id.begin(), weather_id.end(), temp_id.at(i));
        int index = std::distance(weather_id.begin(), it);
        series->setName(labels.at(index));
        axisY->setLabelFormat("%i");
        axisY->setTitleText(labels.at(index) + " (" + units.at(index) + ")");
        w_.getChart()->addAxis(axisY, Qt::AlignLeft);

    } else if (temp_id.at(i) < 300) {
        auto it = std::find(electricity_id.begin(), electricity_id.end(), temp_id.at(i));
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
        if(!ids.empty()) {
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

    std::vector<std::pair<double, std::string>> ids = saves_->get_entry(slot);
    if(status == false) {
        if(!ids.empty()) {
            // clear vanha?
            data_->set_old_data(slot + 300, ids);
            draw_graph(slot + 300);
        }

    } else {
            if(!ids.empty()) {
                saves_->remove_save_entry(slot);
                data_->clear_save_data(slot + 300);
                w_.checkBoxText(slot + 2, "Empty");

            } else {
                if(!boxes.empty()) {
                    int chosen_id;
                    for ( unsigned int i = 0; i < boxes.size(); i++ ) {
                        if(boxes.at(i) == 13 || boxes.at(i) == 14) {
                            chosen_id = boxes.at(i) - 9;

                        } else if(boxes.at(i) == 5) {
                            chosen_id = 166;

                        } else if(boxes.at(i) == 6) {
                            chosen_id = 242;
                        }
                    }
                    data_->copy_old_data(chosen_id, slot + 300);
                    std::vector<std::pair<double, std::string>> datas =
                            data_->get_Save_Data(slot + 300);
                    saves_->new_save_entry(slot, datas);
                    w_.checkBoxText(slot + 2, "Has Data");
            }
        }
    }
}

void Logic::setCheckBoxText()
{
    for(int i = 1; i < 4; i++) {
        if(i == 1 || i == 2) {
            if(preference_->get_entry(i).empty()) {
                w_.checkBoxText(i, "Empty");

            } else {
                w_.checkBoxText(i, "Has Data");
            }
        } else {
            int j = i - 2;
            if(saves_->get_entry(j).empty()) {
                w_.checkBoxText(i, "Empty");

            } else {
                w_.checkBoxText(i, "Has Data");
            }
        }
    }
}

void Logic::calculate_average_temp(double days, int id)
{
    if(id == 6){
        for(std::vector<double>::iterator it_temp = temp_AVG.begin(); it_temp !=temp_AVG.end(); ++it_temp){
            all_temp += *it_temp;
        }
        double average = all_temp/days;
        temps.push_back(average);
        w_.LcdAverage(average);
    }
    if(id == 7) {
        for(std::vector<double>::iterator it_max = temp_AVGmax.begin(); it_max != temp_AVGmax.end(); ++it_max){
            all_max += *it_max;
        }
        double average_max = all_max/days;
        temps.push_back(average_max);
        w_.LcdMax(average_max);
    }
    if(id == 8){
        for(std::vector<double>::iterator it_min = temp_AVGmin.begin(); it_min != temp_AVGmin.end(); ++it_min){
            all_min += *it_min;
        }
        double average_min = all_min/days;
        temps.push_back(average_min);
        w_.LcdMin(average_min);
    }
}


void Logic::calculate_powerforms(int id)
{
    //Summs datapoints all electricity production
    for(std::vector<double>::iterator it_temp = Electricity.begin(); it_temp !=Electricity.end(); ++it_temp){
        all_elec += *it_temp;
    }
    std::vector<double> power;
    if(id == WIND_POWER_PRODUCTION){
        for(std::vector<double>::iterator it_temp = Wind.begin(); it_temp !=Wind.end(); ++it_temp){
            all_wind += *it_temp;
        }
        double calculation = all_wind/all_elec *100;
        w_.LcdWind(calculation);

    } else if(id == NUCLEAR_POWER_PRODUCTION){
        for(std::vector<double>::iterator it_temp = Nuclear.begin(); it_temp !=Nuclear.end(); ++it_temp){
            all_nuke += *it_temp;
        }
        double calculation = all_nuke/all_elec *100;
        w_.LcdNuclear(calculation);

    } else if(id == HYDRO_POWER_PRODUCTION) {
        for(std::vector<double>::iterator it_temp = Hydro.begin(); it_temp !=Hydro.end(); ++it_temp){
            all_hydro += *it_temp;
        }
        double calculation = all_hydro/all_elec *100;
        w_.LcdHydro(calculation);
    }
}

void Logic::clear_values()
{
    data_->clear_data();
    requestList.clear();
    all_temp = 0;
    all_min = 0;
    all_max = 0;
    all_elec = 0;
    all_nuke = 0;
    all_wind = 0;
    all_hydro = 0;
    temps.clear();
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
    temp_id = id_list;
    clear_values();

    // loop over selected ids.
    for(unsigned long int i = 0; i < id_list.size(); i++) {
        // find if id belongs to weather or electricity data.
        auto it = std::find(electricity_id.begin(), electricity_id.end(), id_list.at(i));
        if (it != electricity_id.end()){
            calculate_powerforms(id_list.at(i));
            // if selected data is forecast data.
            if(id_list.at(i) == ELECTRICITY_FORECAST or id_list.at(i) == TENTATIVE_PRODUCTION_PREDICTION) {
                // declaring argument of time()
                QDateTime current =  current.currentDateTime();;
                QDateTime next = current.addDays(1);
                url = QUrl("https://api.fingrid.fi/v1/variable/" + QVariant(id_list.at(i)).toString() +
                                "/events/xml?start_time=" + current.toString("yyyy-MM-ddTHH:mm:ssZ")  + "&end_time=" + next.toString("yyyy-MM-ddTHH:mm:ssZ"));
                QNetworkRequest request(url);
                request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
                pair = std::make_pair(url, i);
                requestList.insert(pair);
                manager_.get(request);

            }
            // if selected data is normal electricity data.
            else {
                url = QUrl("https://api.fingrid.fi/v1/variable/" + QVariant(id_list.at(i)).toString() +
                                "/events/xml?start_time=" + start.toString("yyyy-MM-ddT00%3A00%3A00Z")  + "&end_time=" + end.toString("yyyy-MM-ddT00%3A00%3A00Z"));
                //https://api.fingrid.fi/v1/variable/241/events/xml?start_time=2021-01-01T22%3A00%3A00Z&end_time=2021-03-17T22%3A00%3A00Z
                QNetworkRequest request(url);
                request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
                pair = std::make_pair(url, i);
                requestList.insert(pair);
                manager_.get(request);

            }
        }

        else {
            // if selected data is average, mininum or max temp for a month
            if ((id_list.at(i) <= 8 and id_list.at(i) >= 6)) {
                auto it = std::find(weather_id.begin(), weather_id.end(), id_list.at(i));
                int index = std::distance(weather_id.begin(), it);
                int days = start.daysInMonth();
                url = QUrl("https://opendata.fmi.fi/wfs?request=getFeature&version=2.0.0&storedquery_id=fmi::observations::weather::hourly::simple&place=" + place + "&starttime="
                                 + start.toString("yyyy-MM-01T00:00:00Z")  + "&endtime=" + start.toString("yyyy-MM") + "-"
                "" + QString::number(days) + start.toString("T23:59:00Z") + "&parameters=" + callouts.at(index));
                QNetworkRequest request(url);
                request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
                pair = std::make_pair(url, i);
                calculate_average_temp(days, id_list.at(i));
                requestList.insert(pair);
                manager_.get(request);
            }

            // if selected data is measured temp, windspeed or cloud coverage
            else if(id_list.at(i) <= 3 and id_list.at(i) >= 1) {
                auto it = std::find(weather_id.begin(), weather_id.end(), id_list.at(i));
                int index = std::distance(weather_id.begin(), it);
                url = QUrl("https://opendata.fmi.fi/wfs?request=getFeature&version=2.0.0&storedquery_id=fmi::observations::weather::simple&place=" + place + "&starttime="
                                 + start.toString("yyyy-MM-ddT00:00:00Z")  + "&endtime=" + end.toString("yyyy-MM-ddT00:00:00Z") + "&parameters=" + callouts.at(index));
                QNetworkRequest request(url);
                request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
                std::pair<QUrl, int> pair = std::make_pair(url, i);
                requestList.insert(pair);
                manager_.get(request);

            }
            // if selected data is forecast.
            else {
                QDateTime current =  current.currentDateTime();
                QDateTime next  = current.addDays(1);
                auto it = std::find(weather_id.begin(), weather_id.end(), id_list.at(i));
                int index = std::distance(weather_id.begin(), it);


                url = QUrl("https://opendata.fmi.fi/wfs?request=getFeature&version=2.0.0&storedquery_id=fmi::forecast::hirlam::surface::point::simple&&"
                                "place=" + place + "&timestep=30&starttime="
                                 + current.toString("yyyy-MM-ddTHH:mm:ssZ")  + "&endtime="
                                 + next.toString("yyyy-MM-ddTHH:mm:ssZ")  + "&parameters=" + callouts.at(index));

                QNetworkRequest request(url);
                request.setRawHeader("x-api-key", "f7yYNeOR2W38fAXGGWWzG9T8avve3Yvl1cGv4op6");
                pair = std::make_pair(url, i);
                requestList.insert(pair);
                manager_.get(request);
            }
        }
    }
}
