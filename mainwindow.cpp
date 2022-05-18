#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , processFlag(false)
    , pausedFlag(false)
    , idCounter(1)
    , counterTime(0)
    , actualProcessThread(&MainWindow::updateActualProcess, this)
    , executeProcessThread(&MainWindow::executeActualProcess, this)
    , executeGlobalCounterThread(&MainWindow::executeGlobalCounter, this)
{
    ui->setupUi(this);
    setWindowTitle("FCFS");

    // Configuración inicial del frame Proceso Actual
    ui->pb_continue->setEnabled(false);
    ui->pb_pause->setEnabled(false);
    ui->pb_end->setEnabled(false);

    ui->pb_process->setEnabled((false));
    ui->sb_processBurstTime->setMinimum(1);
    ui->sb_processBurstTime->clear();

    // Editar el encabezado de la tabla de la cola de procesos
    QStringList processesTitles;
    processesTitles << "ID" << "Nombre" << "Estado" << "Duración" << "Llegada";
    ui->processTable->setColumnCount(5);
    ui->processTable->setHorizontalHeaderLabels(processesTitles);

    // Editar el encabezado de la tabla de proceso finalizados
    QStringList endedProcessesTitles;
    endedProcessesTitles << "ID" << "Nombre" << "Estado" << "Burst Time" << "Arrival Time" << "Completion Time" << "Turn Around Time" << "Waiting Time";
    ui->endedProcessTable->setColumnCount(8);
    ui->endedProcessTable->setHorizontalHeaderLabels(endedProcessesTitles);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_process_clicked() {
    Process newProcess(
                this->idCounter++, // ID
                ui->le_processName->text().toStdString(), // Nombre del proceso
                ui->sb_processBurstTime->text().toInt(), // Duración del proceso, burstTime
                this->counterTime, // Llegada del proceso, arrivalTime
                created // Estado del proceso (No iniciado)
    );

    // Agregar el proceso a la lista
    this->processes.push_back(newProcess);
    updateProcessTable();
}

void MainWindow::updateProcessTable() {
    ui->processTable->setRowCount(0);
    for(Process& newProcess : this->processes) {
        // Obtenemos los valores del proceso en cuestión
        // Para agregarlo a la tabla
        QString newId = QString::number(newProcess.getId());
        QString newName = QString::fromStdString(newProcess.getName());
        QString newState = QString::fromStdString(newProcess.getStateStr());
        QString newBurstTime = QString::number(newProcess.getBurstTime());
        QString newArrivalTime = QString::number(newProcess.getArrivalTime());

        // Creamos una fila para insertar los datos del proceso
        ui->processTable->insertRow(ui->processTable->rowCount());

        // Insertamos en cada columna el respectivo valor de cada dato
        ui->processTable->setItem(ui->processTable->rowCount()-1, id, new QTableWidgetItem(newId));
        ui->processTable->setItem(ui->processTable->rowCount()-1, name, new QTableWidgetItem(newName));
        ui->processTable->setItem(ui->processTable->rowCount()-1, state, new QTableWidgetItem(newState));
        ui->processTable->setItem(ui->processTable->rowCount()-1, burstTime, new QTableWidgetItem(newBurstTime));
        ui->processTable->setItem(ui->processTable->rowCount()-1, arrivalTime, new QTableWidgetItem(newArrivalTime));

        // Limpiamos los campos del nuevo proceso
        ui->le_processName->clear();
        ui->sb_processBurstTime->clear();
    }
}

void MainWindow::executeActualProcess() {
    while(1) {
        if(this->processFlag == true) {
            if(this->actualProcess.getBurstTime() > 0) {
                if(!pausedFlag) {
                    this->actualProcess.setBurstTime(this->actualProcess.getBurstTime()-1);
                    ui->lb_actualDurationData->setText(QString::number(this->actualProcess.getBurstTime()));
                    Sleep(1000);
                }
            }
            else {
                // Entra a este condicional cuando el tiempo del proceso
                // en ejecución ya es 0, lo que significa que ya se
                // ha terminado su ejecución
                ui->lb_actualNameData->clear();
                ui->lb_actualDurationData->clear();
                ui->lb_actualStateData->clear();
                this->processes.erase(this->processes.begin());
                    // Parte del algoritmo FCFS
                this->calculateFCFS();
                    // ------------------------
                this->endedProcesses.push_back(this->actualProcess);
                updateEndedProcessTable();
                updateProcessTable();
                this->processFlag = false;
            }
        }
    }
}

void MainWindow::updateActualProcess() {
    while(1) {
        if(this->processFlag == false && this->processes.size() > 0) {
            this->actualProcess = this->processes.front();
            this->actualProcess.setState(execution);
            ui->lb_actualNameData->setText(QString::fromStdString(this->actualProcess.getName()));
            ui->lb_actualStateData->setText(QString::fromStdString(this->actualProcess.getStateStr()));
            ui->lb_actualDurationData->setText(QString::number(this->actualProcess.getBurstTime()));
            ui->pb_pause->setEnabled(true);
            ui->pb_end->setEnabled(true);

                // Parte del algoritmo FCFS
            // Waiting Time = StartTime - ArrivalTime
            // Tiempo que esperó el proceso para ser ejecutado por el programa
            int waitingTime = this->counterTime - this->actualProcess.getArrivalTime();
            // Actualizamos el waitingTime del proceso
            this->actualProcess.setWaitingTime(waitingTime);
                // ------------------------

            updateProcess();
            updateProcessTable();
            this->processFlag = true;
        }
    }
}

/**
 * @brief MainWindow::executeGlobalCounter
 *
 * Función que se ejecuta en un hilo
 * para tener un contador global que inicia en 0
 * y aumenta en 1 cada segundo.
 */
void MainWindow::executeGlobalCounter() {
    while(1) {
        Sleep(1000);
        ui->lcd_counterTime->display(QString::number(this->counterTime++));
    }
}

void MainWindow::updateEndedProcessTable() {
    Process lastProcess = this->endedProcesses.at(this->endedProcesses.size()-1);

    // Agregamos el proceso a la tabla
    QString newId = QString::number(lastProcess.getId());
    QString newName = QString::fromStdString(lastProcess.getName());
    QString newBurstTime = QString::number(lastProcess.getTotalDuration());
    QString newState = QString::fromStdString(lastProcess.getStateStr());
    QString newArrivalTime = QString::number(lastProcess.getArrivalTime());
    QString newCompletionTime = QString::number(lastProcess.getCompletionTime());
    QString newTurnAroundTime = QString::number(lastProcess.getTurnAroundTime());
    QString newWaitingTime = QString::number(lastProcess.getWaitingTime());

    ui->endedProcessTable->insertRow(ui->endedProcessTable->rowCount());
    ui->endedProcessTable->setItem(ui->endedProcessTable->rowCount()-1, id, new QTableWidgetItem(newId));
    ui->endedProcessTable->setItem(ui->endedProcessTable->rowCount()-1, name, new QTableWidgetItem(newName));
    ui->endedProcessTable->setItem(ui->endedProcessTable->rowCount()-1, state, new QTableWidgetItem(newState));
    ui->endedProcessTable->setItem(ui->endedProcessTable->rowCount()-1, burstTime, new QTableWidgetItem(newBurstTime));
    ui->endedProcessTable->setItem(ui->endedProcessTable->rowCount()-1, arrivalTime, new QTableWidgetItem(newArrivalTime));
    ui->endedProcessTable->setItem(ui->endedProcessTable->rowCount()-1, completionTime, new QTableWidgetItem(newCompletionTime));
    ui->endedProcessTable->setItem(ui->endedProcessTable->rowCount()-1, turnAroundTime, new QTableWidgetItem(newTurnAroundTime));
    ui->endedProcessTable->setItem(ui->endedProcessTable->rowCount()-1, waitingTime, new QTableWidgetItem(newWaitingTime));

    ui->pb_continue->setEnabled(false);
    ui->pb_pause->setEnabled(false);
    ui->pb_end->setEnabled(false);
}


void MainWindow::updateProcess() {
    for(auto& process : this->processes) {
        if(process.getId() == this->actualProcess.getId()) {
                process.setState(this->actualProcess.getState());
        }
    }
}

void MainWindow::calculateFCFS() {
    // Estado Terminado
    this->actualProcess.setState(ended);

    // CompletionTime - Tiempo que terminó su ejecución
    this->actualProcess.setCompletionTime(this->counterTime);

    // TurnAroundTime - Tiempo que tardó el proceso en ejecutarse
    // Puede no ser el mismo que el BurstTime debido a que se puede pausar su ejecución o finalizar
    this->actualProcess.setTurnAroundTime(
                (this->actualProcess.getCompletionTime() -
                 this->actualProcess.getArrivalTime()) -
                this->actualProcess.getWaitingTime());
}


void MainWindow::on_le_processName_textChanged(const QString &arg1) {
    if(arg1.length() > 0 && ui->sb_processBurstTime->text().toInt() > 0) {
        ui->pb_process->setEnabled(true);
    }
    else {
        ui->pb_process->setEnabled(false);
    }
}


void MainWindow::on_sb_processBurstTime_valueChanged(int arg1) {
    if(ui->le_processName->text().length() > 0 && arg1 > 0) {
        ui->pb_process->setEnabled(true);
    }
    else {
        ui->pb_process->setEnabled(false);
    }
}


void MainWindow::on_pb_pause_clicked() {
    this->pausedFlag = !this->pausedFlag;
    ui->pb_pause->setEnabled(false);
    ui->pb_continue->setEnabled(true);
    this->actualProcess.setState(paused);
    ui->lb_actualStateData->setText(QString::fromStdString(this->actualProcess.getStateStr()));
    updateProcess();
    updateProcessTable();
}


void MainWindow::on_pb_continue_clicked() {
    this->pausedFlag = !this->pausedFlag;
    ui->pb_pause->setEnabled(true);
    ui->pb_continue->setEnabled(false);
    this->actualProcess.setState(execution);
    ui->lb_actualStateData->setText(QString::fromStdString(this->actualProcess.getStateStr()));
    updateProcess();
    updateProcessTable();
}


void MainWindow::on_pb_end_clicked() {
    ui->lb_actualNameData->clear();
    ui->lb_actualDurationData->clear();
    ui->lb_actualStateData->clear();
    this->processes.erase(this->processes.begin());
    this->processFlag = false;
        // Parte del algoritmo FCFS
    this->calculateFCFS();
        // -----------------------
    this->endedProcesses.push_back(this->actualProcess);
    updateProcessTable();
    updateEndedProcessTable();
}

