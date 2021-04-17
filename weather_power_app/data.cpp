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

                if(id == 6 && datetime.time().hour() == 12){
                    AVG_temp.push_back(iter->first);
                }
                else if (id == 7 && datetime.time().hour() == 12){
                    AVG_max.push_back(iter->first);
                }
                else if (id == 8 && datetime.time().hour() == 12) {
                    AVG_min.push_back(iter->first);
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
