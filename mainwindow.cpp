#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <windows.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)    
    , pausedFlag(false)
    , idCounter(1)
    , counterTime(0)
    , quantum(10)
    , currentQuantum(10)
    , executeRoundRobinThread(&MainWindow::executeRoundRobin, this)
    , executeGlobalCounterThread(&MainWindow::executeGlobalCounter, this)
{
    ui->setupUi(this);
    setWindowTitle("Round Robin");

    // Configuración inicial del frame Proceso Actual
    ui->pb_continue->setEnabled(false);
    ui->pb_pause->setEnabled(false);
    ui->pb_end->setEnabled(false);

    // Deshabilitamos el botón de agregar proceso hasta que haya datos válidos
    ui->pb_process->setEnabled((false));
    ui->sb_processBurstTime->setMinimum(1);
    ui->sb_processBurstTime->clear();

    // Cambiar el valor del spinbox del quantum
    ui->sb_quantum->setValue(this->quantum);

    // Editar el encabezado de la tabla de la cola de procesos
    QStringList processesTitles;
    processesTitles << "ID" << "Nombre" << "Estado" << "Duración" << "Tiempo restante" << "Llegada";
    ui->processTable->setColumnCount(6);
    ui->processTable->setHorizontalHeaderLabels(processesTitles);

    // Editar el encabezado de la tabla de proceso finalizados
    QStringList endedProcessesTitles;
    endedProcessesTitles << "ID" << "Nombre" << "Estado" << "Burst Time" << "Arrival Time" << "Completion Time" << "Turn Around Time" << "Waiting Time";
    ui->endedProcessTable->setColumnCount(8);
    ui->endedProcessTable->setHorizontalHeaderLabels(endedProcessesTitles);
}


MainWindow::~MainWindow() {
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

    ui->le_processName->clear();
    ui->sb_processBurstTime->clear();
}

void MainWindow::updateProcessTable() {
    ui->processTable->setRowCount(0);
    for(Process& newProcess : this->processes) {
        // Obtenemos los valores del proceso en cuestión
        // Para agregarlo a la tabla
        QString newId = QString::number(newProcess.getId());
        QString newName = QString::fromStdString(newProcess.getName());
        QString newState = QString::fromStdString(newProcess.getStateStr());
        QString newDuration = QString::number(newProcess.getTotalDuration());
        QString newBurstTime = QString::number(newProcess.getBurstTime());
        QString newArrivalTime = QString::number(newProcess.getArrivalTime());

        // Creamos una fila para insertar los datos del proceso
        ui->processTable->insertRow(ui->processTable->rowCount());

        // Insertamos en cada columna el respectivo valor de cada dato
        ui->processTable->setItem(ui->processTable->rowCount()-1, id, new QTableWidgetItem(newId));
        ui->processTable->setItem(ui->processTable->rowCount()-1, name, new QTableWidgetItem(newName));
        ui->processTable->setItem(ui->processTable->rowCount()-1, state, new QTableWidgetItem(newState));
        ui->processTable->setItem(ui->processTable->rowCount()-1, state+1, new QTableWidgetItem(newDuration));
        ui->processTable->setItem(ui->processTable->rowCount()-1, burstTime+1, new QTableWidgetItem(newBurstTime));
        ui->processTable->setItem(ui->processTable->rowCount()-1, arrivalTime+1, new QTableWidgetItem(newArrivalTime));
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


void MainWindow::updateProcess(const Process& p) {
    ui->lb_actualIdData->setText(QString::number(p.getId()));
    ui->lb_actualNameData->setText(QString::fromStdString(p.getName()));
    ui->lb_actualDurationData->setText(QString::number(p.getBurstTime()));
    ui->lcd_counterQuantum->display(QString::number(this->currentQuantum--));
    ui->lb_actualStateData->setText(QString::fromStdString(p.getStateStr()));
}

void MainWindow::clearProcessFrame() {
    ui->lb_actualIdData->clear();
    ui->lb_actualNameData->clear();
    ui->lb_actualDurationData->clear();
    ui->lb_actualStateData->clear();
}


void MainWindow::calculateRoundRobin(Process& p) {
    // Estado Terminado
    p.setState(ended);

    // CompletionTime - Tiempo que terminó su ejecución
    p.setCompletionTime(this->counterTime);

    // TurnAroundTime - Tiempo que tardó el proceso en ejecutarse
    // Puede no ser el mismo que el BurstTime debido a que se puede pausar su ejecución o finalizar
    p.setTurnAroundTime((p.getCompletionTime() - p.getArrivalTime()));

    // WaitingTime - Tiempo que espero el proceso para ser ejecutado
    p.setWaitingTime(p.getTurnAroundTime() - p.getTotalDuration());
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
    this->processes[index].setState(paused);
    ui->lb_actualStateData->setText(QString::fromStdString(this->processes[index].getStateStr()));
    updateProcess(this->processes[index]);
    updateProcessTable();
}


void MainWindow::on_pb_continue_clicked() {
    this->pausedFlag = !this->pausedFlag;
    ui->pb_pause->setEnabled(true);
    ui->pb_continue->setEnabled(false);
    this->processes[index].setState(execution);
    ui->lb_actualStateData->setText(QString::fromStdString(this->processes[index].getStateStr()));
    updateProcess(this->processes[index]);
    updateProcessTable();
}


void MainWindow::on_pb_end_clicked() {
    ui->lb_actualNameData->clear();
    ui->lb_actualDurationData->clear();
    ui->lb_actualStateData->clear();
    this->processes.erase(this->processes.begin()+index);
        // Parte del algoritmo FCFS
    this->calculateRoundRobin(this->processes[index]);
        // -----------------------
    this->endedProcesses.push_back(this->processes[index]);
    updateProcessTable();
    updateEndedProcessTable();
}


void MainWindow::on_pushButton_clicked() {
    this->quantum = ui->sb_quantum->value();
    this->currentQuantum = this->quantum;

    QMessageBox msg;
    QString message = "Se han guardado cambios. \n Valor del Quantum: " + QString::number(this->quantum);
    msg.setText(message);
    msg.exec();
}


/**
 * @brief MainWindow::executeRoundRobin
 *
 * Función para ejecutar el RoundRobin.
 * Itera sobre la cola de procesos y le da a
 * cada uno el tiempo que equivale el quantum.
 *
 * Cuando el quantum termina, este se reinicia y
 * pasa al siguiente proceso en cola.
 */
void MainWindow::executeRoundRobin() {
    while(1) {
        // Iteramos sobre los procesos que haya en la cola
        for(index = 0; index<this->processes.size(); index++) {

            // Le asignamos el valor total del quantum al contador del quantum
            this->currentQuantum = this->quantum;

            // Ejecutaremos el proceso en turno mientras el quantum sea superior a 0
            while(this->currentQuantum > 0) {

                // Establecemos el estado del proceso a "ejecución"
                this->processes[index].setState(execution);

                // Actualizamos los valores del proceso y de la tabla
                updateProcess(this->processes[index]);
                updateProcessTable();

                // Esperamos 1 segundo
                Sleep(1000);

                // Decrementamos el valor del Bursttime del proceso en 1
                this->processes[index].setBurstTime(this->processes[index].getBurstTime()-1);

                // Verificar si el proceso en turno ya terminó su ejecución
                if(this->processes[index].getBurstTime() == 0) {
                    updateProcess(this->processes[index]);
                    calculateRoundRobin(this->processes[index]);

                    // Agregamos el proceso a la lista de procesos terminados
                    this->endedProcesses.push_back(this->processes[index]);

                    // Eliminamos el proceso de la cola de procesos
                    this->processes.erase(this->processes.begin()+index);

                    // Actualizamos el contador del Quantum
                    ui->lcd_counterQuantum->display(QString::number(this->quantum));

                    // Limpiamos y actualizamos valores de la interfaz
                    clearProcessFrame();
                    updateProcessTable();
                    updateEndedProcessTable();

                    // Rompemos el ciclo ya que el proceso terminó su ejecución
                    break;
                }

                // En caso de que se termine el Quantum y no se haya terminado el Bursttime
                // Actualizamos el estado del proceso a "espera"
                this->processes[index].setState(waiting);
                updateProcessTable();
            }
        }
    }
}
