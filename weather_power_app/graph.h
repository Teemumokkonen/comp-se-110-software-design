#ifndef GRAPH_H
#define GRAPH_H

#include <QtWidgets>
#include <QtCharts>

class Graph : public QWidget
{
    Q_OBJECT
    QWidget *graph;

public:
    Graph();
    ~Graph();

    void addGraph();
    QSplineSeries* addSeries(); //Voidaan lisätä muuttujaksi data josta erotellaan piirto pisteet graafille
};

#endif // GRAPH_H
