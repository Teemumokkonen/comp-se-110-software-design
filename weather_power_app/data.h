#ifndef DATA_H
#define DATA_H

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <QDebug>
#include <QtCharts>

class Data
{

public:
    Data();
    ~Data();

    void push_data(int id, double value, std::string timestamp);

    // for debugging push_data method
    void show_data(int id, QLineSeries *series);
private:

    // fingrid data points => map(id) = vector{value, timestamp}
    std::unordered_map<int, std::vector<std::pair<double, std::string>>>  data_points;
};

#endif // DATA_H
