#include "preference.h"

Preference::Preference()
{
    QFile file_(file_name_);
    read_preference_file();
}

Preference::~Preference()
{
}

void Preference::read_preference_file()
{
    file_.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file_);
    while ( !in.atEnd() ) {
        QString line = in.readLine();
        QStringList list = line.split(',', Qt::SkipEmptyParts);
        std::vector<int> temp_vector;
        for ( int i = 0; list.length() > i; i++ ) {
            temp_vector.push_back(list.at(i).toInt());
        }
        int index = temp_vector.at(0);
        temp_vector.erase(temp_vector.begin());
        entries_.insert(std::pair<int, std::vector<int>>(index, temp_vector));
    }
    file_.close();
}

void Preference::write_preference_file()
{
    file_.open(QIODevice::WriteOnly | QIODevice::Text);
    for ( auto it = entries_.begin(); it != entries_.end(); it++ ) {
        QString line = QString::number(it->first);
        for ( unsigned int i = 0; it->second.size() > i; i++ ) {
            line.push_back(',');
            line.push_back(QString::number(it->second.at(i)));
        }
    }
    file_.close();
}

void Preference::new_preference_entry(int slot, std::vector<int> settings)
{
    entries_.insert(std::pair<int, std::vector<int>>(slot, settings));
    write_preference_file();
}

void Preference::remove_preference_entry(int slot)
{
    auto it = entries_.find(slot);
    entries_.erase(it);
    write_preference_file();
}

std::pair<int, std::vector<int>> Preference::get_entry(int slot)
{
    auto it = entries_.find(slot);
    return std::pair<int, std::vector<int>>(it->first, it->second);
}
