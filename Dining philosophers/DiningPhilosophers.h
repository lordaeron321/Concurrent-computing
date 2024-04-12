#pragma once
#include <iostream>
#include <thread>
#include <random>
#include <semaphore>
#include <mutex>
#include <windows.h>
#include <vector>
#include <chrono>
#include<conio.h>
#include <cmath>
using namespace std;
class DiningPhilosophers {
protected:
    const int INIT_VAL_OF_PHILOSOPHERS = 3;
    int numOfPHILOSOPHERS = INIT_VAL_OF_PHILOSOPHERS;

    vector<int> StatesArrayforPhilosophers;
    vector<int> testv{ 0 };
    vector<shared_ptr<counting_semaphore<10>>> ArePhilosopherseating;
    vector<thread> Philosophers;
    mutex queue;

    void test(int i);

    void philosopher(int i);
    


public:

    DiningPhilosophers();

    void addPhilosopher();

    void removePhilosopher();

    vector<int> GetStatesArrayforPhilosophers();

    int GetNumOfPhilosophers();

    vector<int> Gettest();
};


