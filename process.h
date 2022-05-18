#ifndef PROCESS_H
#define PROCESS_H

#include <string>

/**
 * @brief The Process class
 * Clase para almacenar un proceso
 *
 * id: el ID que le corresponde al proceso en la ejecución del programa
 * name: el nombre del proceso
 * burstTime: la duración del proceso en segundos
 * arrivalTime: tiempo del contador global en el que el proceso llegó
 * completionTime: tiempo en el que se completó el proceso
 * turnAroundTime: pendiente
 * waitingTime: tiempo que el proceso esperó en la cola para ser ejecutado
 *
 * state: situación actual del proceso representado en un número
 * stateStr: situación actual del proceso representado en un mensaje
 *
 *          state |  stateStr
 *          --------------------
 *          0     |  no iniciado
 *          1     |  en ejecución
 *          2     |  pausado
 *          3     |  finalizado
 *
 */

class Process {
private:
    int id;
    std::string name;
    int burstTime;
    int arrivalTime;
    int completionTime;
    int turnAroundTime;
    int waitingTime;

    int totalDuration;
    int state;
    std::string stateStr;

public:
    Process();
    Process(const int&, const std::string&,
            const int&, const int&, const int&);

    void setName(const std::string &newName);
    const std::string &getName() const;

    void setState(int newState);
    const int &getState() const;

    const std::string &getStateStr() const;
    void setStateStr(const std::string &newStateStr);

    enum State {
        created, execution, paused, ended
    };
    int getBurstTime() const;
    void setBurstTime(int newBurstTime);
    int getArrivalTime() const;
    void setArrivalTime(int newArrivalTime);
    int getCompletionTime() const;
    void setCompletionTime(int newCompletionTime);
    int getTurnAroundTime() const;
    void setTurnAroundTime(int newTurnAroundTime);
    int getWaitingTime() const;
    void setWaitingTime(int newWaitingTime);
    int getId() const;
    void setId(int newId);
    int getTotalDuration() const;
    void setTotalDuration(int newTotalDuration);
};

#endif // PROCESS_H
