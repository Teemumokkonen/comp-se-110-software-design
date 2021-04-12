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
            }
        }
    }
}

void Data::clear_data()
{
    data_points.clear();
}
