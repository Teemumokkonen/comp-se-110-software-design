#include "saves.h"

Saves::Saves()
{

}

void Saves::read_save_file()
{
    QFile file_(QCoreApplication::applicationDirPath() + file_name_);
    if(!file_.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QTextStream in(&file_);
    int count = 0;
    std::vector<double> temp_vector;
    int index;
    while ( !in.atEnd() ) {
        if( count % 2 == 0) {
            QString line = in.readLine();
            QStringList list = line.split(',', Qt::SkipEmptyParts);
            temp_vector = {};
            for ( int i = 0; list.size() > i; i++ ) {
                temp_vector.push_back(list.at(i).toDouble());
            }
            index = (int) temp_vector.at(0);
            temp_vector.erase(temp_vector.begin());
            count++;

        } else {
            QString line = in.readLine();
            QStringList list = line.split(',', Qt::SkipEmptyParts);
            std::vector<std::string> temp_vector_2;
            for ( int i = 0; list.size() > i; i++ ) {
                temp_vector_2.push_back(temp_vector_2.at(i));
            }
            std::vector<std::pair<double, std::string>> temp_vector_3;
            for ( unsigned int i = 0; temp_vector_2.size() > i; i++ ) {
                temp_vector_3.push_back(std::pair<double,
                                        std::string>(temp_vector.at(i),
                                                      temp_vector_2.at(i)));
            }
            entries_.insert(std::pair<int,
                            std::vector<std::pair<double,
                            std::string>>>(index, temp_vector_3));
            count++;
        }
    }
    file_.close();
}

void Saves::write_save_file()
{
    QFile file_(QCoreApplication::applicationDirPath() + file_name_);
    if(!file_.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return;
    }
    QTextStream out(&file_);
    std::string line = "";
    std::string tempLine;
    int count = 0;
    for ( auto it = entries_.begin(); it != entries_.end(); it++ ) {
        if( count % 2 == 0) {
            tempLine = std::to_string(it->first);
            for ( unsigned int i = 0; i < it->second.size(); i++) {
                tempLine.push_back(',');
                tempLine += std::to_string(it->second.at(i).first);
            }
            count++;

        } else {
            for ( unsigned int i = 0; i < it->second.size(); i++) {
                tempLine.push_back(',');
                tempLine += it->second.at(i).second;
            }
            count++;
        }
        tempLine += "\n";
        line += tempLine;
    }

    out << QString::fromStdString(line);
    file_.close();
}

std::vector<std::pair<double, std::string>> Saves::get_entry(int slot)
{
    auto it = entries_.find(slot);
    if(it != entries_.end()){
        return it->second;
    }
    return {};
}

void Saves::remove_save_entry(int slot)
{
    auto it = entries_.find(slot);
    if(it != entries_.end()){
        entries_.erase(it);
        write_save_file();
    }
}

void Saves::new_save_entry(int slot, std::vector<std::pair<double, std::string>> series)
{
    entries_.insert(std::pair<int,
                    std::vector<std::pair<double,
                    std::string>>>(slot, series));
    write_save_file();
}
