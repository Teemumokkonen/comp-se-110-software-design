#include "preference.h"

Preference::Preference()
{

}

void Preference::create_save_file()
{
    std::ifstream file("preferences.txt");
    if (!file) {
        file.open("preferences.txt");
    }
}
