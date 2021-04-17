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

    void clear_data();

    std::vector<double> get_AVG_temp();
    std::vector<double> get_AVG_max();
    std::vector<double> get_AVG_min();

private:

    // fingrid data points => map(id) = vector{value, timestamp}
    std::unordered_map<int, std::vector<std::pair<double, std::string>>>  data_points;
    std::vector<double> AVG_temp;
    std::vector<double> AVG_max;
    std::vector<double> AVG_min;
};

#endif // DATA_H
