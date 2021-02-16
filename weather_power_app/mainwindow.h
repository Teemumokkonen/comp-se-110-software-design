#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QMenu>

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

private:

    Ui::MainWindow *ui;

    // Window settings
    int window_width = 1000;
    int window_height = 600;

    QMenu *gameMenu;
    QMenu *aboutMenu;


};
#endif // MAINWINDOW_H
