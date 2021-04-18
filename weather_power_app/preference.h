#ifndef PREFERENCE_H
#define PREFERENCE_H
#include <QFile>
#include <QString>
#include <map>
#include <vector>
#include <QTextStream>
#include <QCoreApplication>


class Preference : public QObject
{
    Q_OBJECT

public:
    Preference();
    ~Preference();

    // Reads the file
    void read_preference_file();

    // Writes to the file
    void write_preference_file();

    // Add a new etry to the data structure
    void new_preference_entry(int slot, std::vector<int> settings);

    // Removes an entry from the data structure
    void remove_preference_entry(int slot);

    // Return a specific entry based on a slot
    std::vector<int> get_entry(int slot);

private:
    QFile file_;
    QString file_name_ = "preferences.txt";
    std::map<int, std::vector<int>> entries_;
};

#endif // PREFERENCE_H
