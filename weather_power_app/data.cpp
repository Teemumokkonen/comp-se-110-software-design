#include "data.h"

Data::Data()
{

}

Data::~Data()
{

}

void Data::push_data(int id, double value, std::string timestamp)
{
    std::vector<std::pair<double, std::string>> data;
    auto data_pair = std::make_pair(value, timestamp);
    data.push_back(data_pair);
    std::unordered_map<int, std::vector<std::pair<double, std::string>>>::const_iterator it = data_points.find(id);
    if(it == data_points.end()) {
        data_points.insert(make_pair(id, data));
    }
    else  {
        data_points[id].push_back(data_pair);
    }
}

void Data::show_data(int id, QLineSeries *series)
{

    if (data_points.size() != 0) {
        std::unordered_map<int, std::vector<std::pair<double, std::string>>>::const_iterator it = data_points.find(id);
        if(it != data_points.end()) {
            for(auto iter = it->second.begin(); iter != it->second.end(); iter++)
            {
                auto str = QString::fromStdString(iter->second).insert(22,':');
                QDateTime datetime = QDateTime::fromString(str,Qt::ISODate);
                series->append(datetime.toMSecsSinceEpoch(),iter->first);
                //Average Temperature
                if(id == 6 && datetime.time().hour() == 12){
                    AVG_temp.push_back(iter->first);
                }
                //Maximum Temperature
                else if (id == 7 && datetime.time().hour() == 12){
                    AVG_max.push_back(iter->first);
                }
                //Minimum Temperature
                else if (id == 7 && datetime.time().hour() == 12){
                    AVG_max.push_back(iter->first);
                }
                else if (id == 8 && datetime.time().hour() == 12) {
                    AVG_min.push_back(iter->first);
                }
                //Electricity production
                else if(id == 192){
                    Electricity.push_back(iter->first);
                }
                //Wind-power production
                else if(id == 181){
                    Wind.push_back(iter->first);
                }
                //Nuclear-power production
                else if(id == 188){
                    Nuclear.push_back(iter->first);
                }
                //Hydro-power production
                else if(id == 191){
                    Hydro.push_back(iter->first);
                }
                // Save slot 1
                else if(id == 301){
                    saveslot1.push_back(std::pair<double,
                                        std::string>(iter->first, iter->second));
                }
            }
        }
    }
}

void Data::clear_data()
{
    data_points.clear();
    AVG_temp.clear();
    AVG_max.clear();
    AVG_min.clear();
    Electricity.clear();
    Wind.clear();
    Nuclear.clear();
    Hydro.clear();
}

void Data::clear_save_data(int id)
{
    auto it = data_points.find(id);
    if(it != data_points.end()){
        data_points.erase(it);
    }
}

void Data::copy_old_data(int old_data, int new_data)
{
    if (data_points.size() != 0) {
        std::unordered_map<int, std::vector<std::pair<double,
                std::string>>>::const_iterator it = data_points.find(old_data);
        if(it != data_points.end()) {
            std::vector<std::pair<double,
            std::string>> temp_vector = it->second;
            data_points.insert(std::pair<int,
                               std::vector<std::pair<double,
                               std::string>>>(new_data, temp_vector));
        }
    }
}

void Data::set_old_data(int id, std::vector<std::pair<double,
                        std::string>> series)
{
    data_points.insert(std::pair<int,
                       std::vector<std::pair<double,
                       std::string>>>(id, series));
}

std::vector<double> Data::get_AVG_temp()
{
    return AVG_temp;
}

std::vector<double> Data::get_AVG_max()
{
    return AVG_max;
}

std::vector<double> Data::get_AVG_min()
{
    return AVG_min;
}

std::vector<double> Data::get_Electricity()
{
    return Electricity;
}

std::vector<double> Data::get_Wind()
{
    return Wind;
}

std::vector<double> Data::get_Hydro()
{
    return Hydro;
}

std::vector<double> Data::get_Nuclear()
{
    return Nuclear;
}

std::vector<std::pair<double, std::string> > Data::get_Save_Data(int id)
{
    auto it = data_points.find(id);
    if(it != data_points.end()){
        return it->second;
    }
    return {};
}
