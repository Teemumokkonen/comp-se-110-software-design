#include "preference.h"

Preference::Preference()
{
}

Preference::~Preference()
{
}

void Preference::read_preference_file()
{
    QFile file_(QCoreApplication::applicationDirPath() + file_name_);
    if(!file_.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QTextStream in(&file_);
    while ( !in.atEnd() ) {
        QString line = in.readLine();
        QStringList list = line.split(',', Qt::SkipEmptyParts);
        std::vector<int> temp_vector;
        for ( int i = 0; list.size() > i; i++ ) {
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
    QFile file_(QCoreApplication::applicationDirPath() + file_name_);
    if(!file_.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return;
    }
    QTextStream out(&file_);
    QString line = "";
    QString tempLine;
    for ( auto it = entries_.begin(); it != entries_.end(); it++ ) {
        tempLine = QString::number(it->first);
        for ( auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            tempLine.push_back(',');
            tempLine.push_back(QString::number(*it2));
        }
        tempLine.push_back("\n");
        line.push_back(tempLine);
    }
    out << line;
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
    if(it != entries_.end()){
        entries_.erase(it);
        write_preference_file();
    }
}

std::vector<int> Preference::get_entry(int slot)
{
    auto it = entries_.find(slot);
    if(it != entries_.end()){
        return it->second;
    }
    return {};
}
