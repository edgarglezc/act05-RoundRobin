#include "process.h"

/**
 * @brief Process::Process
 *
 * Consturctor por defecto de Process.
 * Asigna todos los valores a 0.
 */
int Process::getTotalDuration() const
{
    return totalDuration;
}

void Process::setTotalDuration(int newTotalDuration)
{
    totalDuration = newTotalDuration;
}

Process::Process() {
    this->id = 0;
    this->name = "NONAME";
    this->burstTime = 0;
    this->arrivalTime = 0;
    this->completionTime = 0;
    this->turnAroundTime = 0;
    this->waitingTime = 0;
    this->setState(0);
}


/**
 * @brief Process::Process
 * @param id
 * @param name
 * @param bt
 * @param at
 * @param state
 *
 * Constructor parametrizado de Process.
 * Recibe los valores que necesita el proceso obligatoriamente
 * para poder ser ejecutado.
 */
Process::Process(const int& id, const std::string& name, const int& bt,
                 const int& at, const int& state) {
    this->id = id;
    this->name = name;
    this->burstTime = bt;
    this->arrivalTime = at;
    this->completionTime = 0;
    this->turnAroundTime = 0;
    this->waitingTime = 0;
    this->setState(state);
    this->totalDuration = bt;
}


int Process::getBurstTime() const {
    return burstTime;
}

void Process::setBurstTime(int newBurstTime) {
    burstTime = newBurstTime;
}

int Process::getArrivalTime() const {
    return arrivalTime;
}

void Process::setArrivalTime(int newArrivalTime) {
    arrivalTime = newArrivalTime;
}

int Process::getCompletionTime() const {
    return completionTime;
}

void Process::setCompletionTime(int newCompletionTime) {
    completionTime = newCompletionTime;
}

int Process::getTurnAroundTime() const {
    return turnAroundTime;
}

void Process::setTurnAroundTime(int newTurnAroundTime) {
    turnAroundTime = newTurnAroundTime;
}

int Process::getWaitingTime() const {
    return waitingTime;
}

void Process::setWaitingTime(int newWaitingTime) {
    waitingTime = newWaitingTime;
}

int Process::getId() const {
    return id;
}

void Process::setId(int newId) {
    id = newId;
}


const std::string &Process::getStateStr() const {
    return stateStr;
}


void Process::setStateStr(const std::string &newStateStr) {
    stateStr = newStateStr;
}


const std::string &Process::getName() const {
    return name;
}


void Process::setName(const std::string &newName) {
    name = newName;
}


const int& Process::getState() const {
    return state;
}


/**
 * @brief Process::setState
 * @param newState
 *
 * Recibe el estado del proceso en un número
 * y se le asgina un mensaje dependiendo de su valor.
 * 0 -> No iniciado
 * 1 -> En ejecución
 * 2 -> Pausado
 * 3 -> Finalizado
 */
void Process::setState(int newState) {
    state = newState;
    switch(newState) {
    case created: this->setStateStr("No iniciado"); break;
    case execution: this->setStateStr("En ejecución"); break;
    case paused: this->setStateStr("Pausado"); break;
    case ended: this->setStateStr("Finalizado"); break;
    }
}

