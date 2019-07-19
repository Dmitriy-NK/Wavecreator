#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include "simulationwave.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    WaveSpeed_value=0;
    WaveWidth_value = 0;
    WaveAmplitude_value = 0;
    Threads_value = 0;
    filepath="";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_OpenFile_button_clicked()
{
    filepath = QFileDialog::getOpenFileName(this,"Open File", "C://",tr("Images (*.bmp)"));
    QMessageBox::information(this,"..",filepath);

}

void MainWindow::on_StartSimulation_button_clicked()
{
    if(WaveSpeed_value == 0 ||WaveWidth_value==0||WaveAmplitude_value==0 ||Threads_value==0 ) {
            QMessageBox::information(this,"Error","Fill all the gaps");
    }else if (WaveSpeed_value < 0 ||WaveWidth_value < 0||WaveAmplitude_value < 0 ||Threads_value < 0 ){
            QMessageBox::information(this,"Error","Fill all the gaps with correct data (more than 0 and less or equal 2000)");
    } else if(Threads_value>16) {
        QMessageBox::information(this,"Error","Thread should be less or equal 16)");
    }else if (filepath==""){
        QMessageBox::information(this,"Error","Firstly select File");
    }else{
        float averageframetime=0.f;
        float maxframetime = 0.f;
        Wave* wave=new Wave(WaveWidth_value,WaveAmplitude_value,WaveSpeed_value,Threads_value);
        SimulationWave* Simulation = new SimulationWave(wave);
        if(Simulation->StartSimulation(filepath.toLocal8Bit().constData(),averageframetime,maxframetime)){
            QMessageBox::information(this,"Error","Error: the image wasn't correctly loaded.");
        }
        ui->MaxFrame->setText(QString::number(maxframetime));
        ui->AvFrame->setText(QString::number(averageframetime));
        delete Simulation;
        delete wave;
    }

}

void MainWindow::on_Wavewidth_value_textChanged(const QString &str)
{
    if(str.isEmpty())
        WaveWidth_value=0;
    else{
        if(str.toInt()<=2000)
             WaveWidth_value = str.toInt();
        else{
            WaveWidth_value = -1;
        }
    }
}

void MainWindow::on_Waveamplitude_value_textChanged(const QString &str)
{
    if(str.isEmpty())
        WaveAmplitude_value=0;
    else{
        if(str.toInt()<=2000)
             WaveAmplitude_value = str.toInt();
        else{
            WaveAmplitude_value = -1;
        }
    }
}

void MainWindow::on_Wavespeed_textChanged(const QString &str)
{
    if(str.isEmpty())
       WaveSpeed_value=0;
    else{
        if(str.toInt()<=2000)
             WaveSpeed_value = str.toInt();
        else{
            WaveSpeed_value = -1;
        }
    }
}

void MainWindow::on_ThreadValue_textChanged(const QString &str)
{
    if(str.isEmpty())
        Threads_value=0;
    else{
        if(str.toInt()<=2000)
             Threads_value = str.toInt();
        else{
            WaveWidth_value = -1;
        }
    }
}



