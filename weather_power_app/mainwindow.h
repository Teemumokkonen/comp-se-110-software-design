#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QtCharts>


// ehkä hyödylliset includet:
// Qlistwidget
// QDialog
// Qsignalmapper (voi olla eri nimi)



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTabWidget *tab;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QChart * getChart() const;

signals:
    void sendDateInformation(QDate startDate, QDate EndDate, std::vector<int> id);

private slots:

    // function to fetch the dates that user gives
    void getDate();

private:

    QChart* chart_;
    Ui::MainWindow* ui;

    //dates that user inputs to get the data.

    QDate startDate;
    QDate endDate;
    int howLong;
    QList<QChartView *> m_charts;
    std::vector<int> id_list = {193, 192, 181, 188, 191, 166, 241};
    std::vector<int> variable_id;

};
#endif // MAINWINDOW_H
