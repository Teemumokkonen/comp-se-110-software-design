#include "preference.h"

Preference::Preference()
{

}

void Preference::create_save_file()
{
    // suoraan rakentajaan?
    file_.open(file_name_, std::fstream::out);
    file_.close();
}

void Preference::new_preference_entry(int slot, std::string name, QDate start_date, QDate end_date)
{
    // selvitellään meneekö ilma stringejä.
    file_.open(file_name_, std::ios_base::app);
    std::string number = std::to_string(slot);
    // file_ << number + ", " + name + ", "
}

void Preference::remove_preference_entry(int slot)
{
    // Kattellaa syssymmäl
}
