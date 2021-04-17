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
    void read_preference_file();
    void write_preference_file();
    void new_preference_entry(int slot, std::vector<int> settings);
    void remove_preference_entry(int slot);
    std::vector<int> get_entry(int slot);

private:
    QFile file_;
    QString file_name_ = "preferences.txt";
    std::map<int, std::vector<int>> entries_;
};

#endif // PREFERENCE_H
