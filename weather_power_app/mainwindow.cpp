#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{

    ui->setupUi(this);
    QChartView *chartView;

    //Luodaan graafi ja annetaan sille otsikko
    chart_ = new QChart();
    chart_->setTitle("Graph");
    chartView = new QChartView(chart_);
    chartView->setRenderHints(QPainter::Antialiasing);
    ui->gridLayout->addWidget(chartView, 1, 0);
    m_charts << chartView;

    // menubar items
    makeMenuBar();
    connect(aboutUsAct, &QAction::triggered, this, &MainWindow::showReadme);
    connect(aboutApp, &QAction::triggered, this, &MainWindow::showAppInfo);

    // signals for mainwindow
    connect(ui->calender, &QCalendarWidget::clicked, this, &MainWindow::getDate);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::saveGraph);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::changeButtonStatus);
    connect(ui->listWidget_2, &QListWidget::itemClicked, this, &MainWindow::changeButtonStatus);
    connect(ui->prefButton1, &QPushButton::clicked, this, &MainWindow::prefButton1clicked);
    connect(ui->prefButton2, &QPushButton::clicked, this, &MainWindow::prefButton2clicked);
    connect(ui->saveButton1, &QPushButton::clicked, this, &MainWindow::saveButton1clicked);
    connect(ui->saveButton2, &QPushButton::clicked, this, &MainWindow::saveButton2clicked);

    ui->save_label->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

QChart *MainWindow::getChart() const
{
    return chart_;
}

void MainWindow::checkBoxText(int slot, QString text)
{
    if(slot == 1) {
        ui->statusChange1->setText(text);

    } else if(slot == 2) {
        ui->statusChange2->setText(text);

    } else if(slot == 3) {
       ui->statusChange3->setText(text);

    } else {
       ui->statusChange4->setText(text);
    }
}

std::vector<int> MainWindow::selectedBoxes()
{
    std::vector<int> temp_vector;
    for(int i = 0; i < ui->listWidget->count(); i++) {
        if(ui->listWidget->item(i)->checkState() == Qt::Checked) {
            temp_vector.push_back(i);
        }
    }
    for(int j = 0; j < ui->listWidget_2->count(); j++) {
        if(ui->listWidget_2->item(j)->checkState() == Qt::Checked) {
            temp_vector.push_back(j + 10);
        }
    }
    return temp_vector;
}

void MainWindow::retrievePrefBoxes(std::vector<int> boxes)
{
    for(int i = 0; i < ui->listWidget->count(); i++) {
        ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
    }
    for(int i = 0; i < ui->listWidget_2->count(); i++) {
        ui->listWidget_2->item(i)->setCheckState(Qt::Unchecked);
    }
    for(unsigned int i = 0; i < boxes.size(); i++) {
        if(boxes.at(i) < 10) {
            ui->listWidget->item(i)->setCheckState(Qt::Checked);

        } else {
            int j = boxes.at(i) - 10;
            ui->listWidget_2->item(j)->setCheckState(Qt::Checked);
        }
    }
}

void MainWindow::makeMenuBar()
{
    aboutMenu = menuBar()->addMenu(tr("&About"));
    aboutUsAct = new QAction(tr("&readme"));
    aboutApp = new QAction(tr("&About this app"));
    aboutMenu->addAction(aboutUsAct);
    aboutMenu->addAction(aboutApp);
    aboutUsAct->setStatusTip(tr("readme that can be read from app"));
    aboutApp->setStatusTip(tr("tips how to use this app for user"));
}


void MainWindow::getDate()
{
    // clear old graph data so new can be added
    chart_->removeAllSeries();
    variable_id.clear();

    // get date data from calender
    startDate = ui->calender->selectedDate();
    howLong = ui->howManyDays->value();
    endDate = startDate.addDays(howLong);
    ui->data_label->setText(startDate.toString("dd.MM.yyyy") + " to "
                           + endDate.toString("dd.MM.yyyy"));

    QString place = ui->comboBox->currentText();
    // get selected data id from Qwidgetlists
    for(int i = 0; i < 7; i++) {
        QListWidgetItem* checkers = ui->listWidget->item(i);
        if(checkers->checkState() == Qt::Checked) {
            variable_id.push_back(id_list.at(i));
        }
    }
    for(int i = 0; i < 8; i++) {
        QListWidgetItem* checkers = ui->listWidget_2->item(i);
        if(checkers->checkState() == Qt::Checked) {
            variable_id.push_back(weather_id.at(i));
        }
    }

    // id for data that user has chosen. See logic.h for all working ids.
    emit sendDateInformation(startDate, endDate, variable_id, place);

}

void MainWindow::saveGraph()
{
    bool ok;
    QString fileName = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("File name:"), QLineEdit::Normal,
                                         "dataGraph", &ok);
    if(ok && !fileName.isEmpty()) {
        OutputFolder = QFileDialog::getExistingDirectory(this, ("Select Folder where to save the file"), QDir::homePath());
    }

    if (!OutputFolder.isEmpty()) {
        QPixmap p = m_charts.at(0)->grab();
        QOpenGLWidget *glWidget  = m_charts.at(0)->findChild<QOpenGLWidget*>();
        if(glWidget){
            QPainter painter(&p);
            QPoint d = glWidget->mapToGlobal(QPoint())-m_charts.at(0)->mapToGlobal(QPoint());
            painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
            painter.drawImage(d, glWidget->grabFramebuffer());
            painter.end();
        }

        p.save(OutputFolder + "/" + fileName + ".png", "PNG");
        ui->save_label->setText("Saved succesfully!");
        ui->save_label->setVisible(true);
    }

    if (ok && fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning", "File name can't be empty");
        ui->save_label->setText("Something went wrong!");
        ui->save_label->setVisible(true);

    }
    QTimer::singleShot(5000, ui->save_label, &QLabel::hide);
}

void MainWindow::changeButtonStatus()
{

    if(ui->listWidget->item(5)->checkState() == Qt::Checked or ui->listWidget->item(6)->checkState() == Qt::Checked or
       ui->listWidget_2->item(3)->checkState() == Qt::Checked or ui->listWidget_2->item(4)->checkState() == Qt::Checked) {
        for(int j = 0; j < ui->listWidget->count(); j++) {
            if(id_list.at(j) != TENTATIVE_PRODUCTION_PREDICTION and id_list.at(j) != ELECTRICITY_FORECAST) {
                ui->listWidget->item(j)->setCheckState(Qt::Unchecked);
                ui->listWidget->item(j)->setFlags(ui->listWidget->item(j)->flags() & ~Qt::ItemIsEnabled);
            }
        }
        for(int j = 0; j < ui->listWidget_2->count(); j++) {
            if(weather_id.at(j) != PREDICTED_TEMPERATURE and weather_id.at(j) != PREDICTED_WIND) {
                ui->listWidget_2->item(j)->setCheckState(Qt::Unchecked);
                ui->listWidget_2->item(j)->setFlags(ui->listWidget_2->item(j)->flags() & ~Qt::ItemIsEnabled);
            }
        }

    }


    else if(ui->listWidget->item(5)->checkState() != Qt::Checked and ui->listWidget->item(6)->checkState() != Qt::Checked
            and ui->listWidget_2->item(3)->checkState() != Qt::Checked and ui->listWidget_2->item(4)->checkState() != Qt::Checked) {
        for(int j = 0; j < ui->listWidget->count(); j++) {
            if(id_list.at(j) != TENTATIVE_PRODUCTION_PREDICTION and id_list.at(j) != ELECTRICITY_FORECAST) {
                ui->listWidget->item(j)->setFlags(ui->listWidget->item(j)->flags() | Qt::ItemIsEnabled);
            }
        }

        for(int j = 0; j < ui->listWidget_2->count(); j++) {
            if(weather_id.at(j) != PREDICTED_TEMPERATURE and weather_id.at(j) != PREDICTED_WIND) {
                ui->listWidget_2->item(j)->setFlags(ui->listWidget_2->item(j)->flags() | Qt::ItemIsEnabled);
            }
        }
    }
}

void MainWindow::showReadme()
{
    showAboutInfo(README);
}

void MainWindow::showAppInfo()
{
    showAboutInfo(APPINFO);
}

void MainWindow::showAboutInfo(QString file)
{
    QString title = "About...";

    QString text = "";
    QFile inputFile(file);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          if(title == "Scores"){
                QStringList splittedline =  line.split(";");
                for (int i = 0; i < splittedline.size(); i++){
                    text.append(splittedline.at(i) + " ");
                }
                text.append("\n");
          }
          else {
                text.append(line + "\n");
          }
       }
       inputFile.close();
    }

    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setWindowTitle(title);
    msgBox.exec();

}

void MainWindow::prefButton1clicked()
{
    std::vector<int> temp_vector = selectedBoxes();
    if(ui->statusChange1->checkState() != Qt::Checked ) {
        emit sendPrefInfo(1, false, temp_vector);

    } else {
        emit sendPrefInfo(1, true, temp_vector);
    }
}
void MainWindow::prefButton2clicked()
{
    std::vector<int> temp_vector = selectedBoxes();
    if(ui->statusChange2->checkState() != Qt::Checked ) {
        emit sendPrefInfo(2, false, temp_vector);

    } else {
        emit sendPrefInfo(2, true, temp_vector);
    }
}

void MainWindow::saveButton1clicked()
{
    std::vector<int> temp_vector = selectedBoxes();
    if(ui->statusChange3->checkState() != Qt::Checked ) {
        emit sendSaveInfo(1, false, temp_vector);

    } else {
        emit sendSaveInfo(1, true, temp_vector);
    }
}

void MainWindow::saveButton2clicked()
{
    std::vector<int> temp_vector = selectedBoxes();
    if(ui->statusChange4->checkState() != Qt::Checked ) {
        emit sendSaveInfo(2, false, temp_vector);

    } else {
        emit sendSaveInfo(2, true, temp_vector);
    }
}
void MainWindow::LcdAverage(double average)
{
    ui->lcdNumberAVG->display(average);
}

void MainWindow::LcdMin(double average)
{
    ui->lcdNumbermin->display(average);
}

void MainWindow::LcdMax(double average)
{
    ui->lcdNumbermax->display(average);
}
