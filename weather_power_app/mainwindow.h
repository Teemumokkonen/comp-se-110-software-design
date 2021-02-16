#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QDate>
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
    QCalendarWidget* calender2;

    // Window settings
    int window_width = 800;
    int window_height = 600;

    //dates that user inputs to get the data.

    QDate startDate;
    int howLong;



};
#endif // MAINWINDOW_H
