#ifndef PREFERENCE_H
#define PREFERENCE_H
#include <fstream>
#include <iostream>
#include <QDate>
#include <string>


class Preference
{
    // Pitäs tulla txt file buildiin, toivottavasti toimii, muuten olllaan kuses
public:
    Preference();
    void create_save_file();
    void new_preference_entry(int slot, std::string name, QDate start_date, QDate end_date);
    void remove_preference_entry(int slot);

private:
    std::string file_name_ = "preferences.txt";
    std::fstream file_;
};

#endif // PREFERENCE_H
