#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCalendarWidget>

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

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:

    // function to fetch the dates that user gives
    void getDate();

private:

    Ui::MainWindow* ui;

    //dates that user inputs to get the data.

    QDate startDate;
    QDate endDate;
    int howLong;



};
#endif // MAINWINDOW_H
