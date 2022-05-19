#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <thread>
#include "process.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_process_clicked();

    void on_le_processName_textChanged(const QString &arg1);

    void on_sb_processBurstTime_valueChanged(int arg1);

    void on_pb_pause_clicked();

    void on_pb_continue_clicked();

    void on_pb_end_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    // Colas para almacenar los procesos
    std::vector<Process> processes;
    std::vector<Process> endedProcesses;    

    // Variable para saber cual es el proceso actual
    size_t index;

    // Bandera para saber si el proceso en ejecución ha sido pausado
    bool pausedFlag;    

    // Contador de ID's para los procesos que se agregan al programa
    int idCounter;

    // Contador del tiempo global
    int counterTime;

    // Variables para almacenar el quantum
    int quantum;
    int currentQuantum;

    // ----------------------------------------

    // Hilos necesarios para ejecutar el programa    
    std::thread executeRoundRobinThread;
    std::thread executeGlobalCounterThread;

    // Funciones de los hilos    
    void executeRoundRobin();
    void executeGlobalCounter();

    // Funciones del programa
    void updateProcessTable();
    void updateEndedProcessTable();
    void updateProcess(const Process&);
    void clearProcessFrame();
    void calculateRoundRobin(Process&);


    enum Column {
        id, name, state, burstTime, arrivalTime, completionTime, turnAroundTime, waitingTime
    };

    enum State {
        created, execution, paused, ended, waiting
    };
};

#endif // MAINWINDOW_H
