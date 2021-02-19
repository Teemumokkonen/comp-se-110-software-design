#include "logic.h"

#include <QApplication>
void startApp();
Logic* logic;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    startApp();
    return a.exec();
}

void startApp(){
    logic->deleteLater();
    logic = new Logic;
    logic->init();
}
