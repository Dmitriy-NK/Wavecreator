#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    int WaveWidth_value;
    int WaveAmplitude_value;
    int WaveSpeed_value;
    int Threads_value;
    QString filepath;
private slots:
    void on_OpenFile_button_clicked();

    void on_StartSimulation_button_clicked();

    void on_Wavewidth_value_textChanged(const QString &arg1);

    void on_Waveamplitude_value_textChanged(const QString &arg1);

    void on_Wavespeed_textChanged(const QString &arg1);

    void on_ThreadValue_textChanged(const QString &arg1);





private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
