#include "graph.h"

using namespace QtCharts;


Graph::Graph(){
    addGraph();
}

Graph::~Graph(){

}

void Graph::addGraph()
{
    graph = new QWidget;
    graph->setWindowTitle("Graafi");

    addSeries();

    //Luodaan graafi, lisätään graafiin otokset (series) ja annetaan sille otsikko
    QChart *chart = new QChart();

    QSplineSeries* series = addSeries();

    chart->addSeries(series);
    chart->setTitle("Täs ois graafi");
    chart->createDefaultAxes(); //Skaalaa automaattisesti x - y akselin
    //chart->legend()->hide(); //voidaan piilottaa piirrettyjen graafien tiedot


    //Piirretään graafi näkyväksi grafiikaksi
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);


    //Näytetään se
    chartView->resize(300, 300);
    chartView->show();
}

QSplineSeries* Graph::addSeries()
{
    //Tehdään luodaan uusi otos jonka perusteella graafi piirtää kuvaajan
     QSplineSeries *series = new QSplineSeries();
     series->setName("Otos");

     //Lisätään dataa otokseen append(x, y) esim. näin
     series->append(0, 2);
     series->append(2, 5);
     series->append(5, 6);
     series->append(7, 9);
     series->append(10, 5);
     //Tai näin
     *series << QPointF(11, 1) << QPointF(13, 5);

     return series;
}
