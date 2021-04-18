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
    void clear_save_data(int id);

    void copy_old_data(int old_data, int new_data);
    void set_old_data(int id, std::vector<std::pair<double,
                      std::string>> series);

    //Parses data values for logic to use
    std::vector<double> get_AVG_temp();
    std::vector<double> get_AVG_max();
    std::vector<double> get_AVG_min();
    std::vector<double> get_Electricity();
    std::vector<double> get_Wind();
    std::vector<double> get_Hydro();
    std::vector<double> get_Nuclear();
    std::vector<std::pair<double, std::string>> get_Save_Data(int id);

private:

    // fingrid data points => map(id) = vector{value, timestamp}
    std::unordered_map<int, std::vector<std::pair<double, std::string>>>  data_points;
    //Temperatures
    std::vector<double> AVG_temp;
    std::vector<double> AVG_max;
    std::vector<double> AVG_min;
    //Power productions
    std::vector<double> Hydro;
    std::vector<double> Nuclear;
    std::vector<double> Wind;
    std::vector<double> Electricity;
    std::vector<std::pair<double, std::string>> saveslot1;
    std::vector<std::pair<double, std::string>> saveslot2;



};

#endif // DATA_H
