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

private:
    Ui::MainWindow *ui;

    // Colas para almacenar los procesos
    std::vector<Process> processes;
    std::vector<Process> endedProcesses;

    // Atributo para tener el proceso actual en ejecución
    Process actualProcess;

    // Banderas para saber si hay un proceso en ejecución
    // o si el proceso en ejecución ha sido pausado
    bool processFlag;
    bool pausedFlag;

    // Contador de ID's para los procesos que se agregan al programa
    int idCounter;

    // Contador del tiempo global
    int counterTime;

    // ----------------------------------------

    // Hilos necesarios para ejecutar el programa
    std::thread actualProcessThread;
    std::thread executeProcessThread;
    std::thread executeGlobalCounterThread;

    // Funciones de los hilos    
    void executeActualProcess();
    void updateActualProcess();
    void executeGlobalCounter();

    // Funciones del programa
    void updateProcessTable();
    void updateEndedProcessTable();
    void updateProcess();
    void calculateFCFS();


    enum Column {
        id, name, state, burstTime, arrivalTime, completionTime, turnAroundTime, waitingTime
    };

    enum State {
        created, execution, paused, ended
    };
};

#endif // MAINWINDOW_H
