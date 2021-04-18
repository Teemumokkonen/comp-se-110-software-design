#ifndef SAVES_H
#define SAVES_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QCoreApplication>
#include <map>

class Saves : public QObject
{
    Q_OBJECT

public:
    Saves();

    // Reads the file
    void read_save_file();

    // Writes to the file
    void write_save_file();

    // Returns the series
    std::vector<std::pair<double, std::string>> get_entry(int slot);

    // Removes an entry from the data structure
    void remove_save_entry(int slot);

    // Add a new etry to the data structure
    void new_save_entry(int slot, std::vector<std::pair<double, std::string>> series);

private:
    QFile file_;
    QString file_name_ = "saves.txt";
    std::map<int, std::vector<std::pair<double, std::string>>> entries_;
};

#endif // SAVES_H
