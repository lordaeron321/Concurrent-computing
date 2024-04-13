#include "DiningPhilosophers.h"

    void DiningPhilosophers::test(int i) {
    
        if (StatesArrayforPhilosophers[(i + numOfPHILOSOPHERS - 1) % numOfPHILOSOPHERS] == 0 && StatesArrayforPhilosophers[(i + 1) % numOfPHILOSOPHERS] == 0) {
            StatesArrayforPhilosophers[i] = 1;
            cout << "filozof nr: " << i << " je i uzywa widelcow nr :" << (i + numOfPHILOSOPHERS - 1) % numOfPHILOSOPHERS << " oraz " << (i + 1) % numOfPHILOSOPHERS << endl;
            Sleep(rand() % 1000 + 1000);
    
            StatesArrayforPhilosophers[i] = 0;
    
            ArePhilosopherseating[i]->release();
            testv[i]++;
            while (testv[i] > 1) {
                ArePhilosopherseating[i]->acquire();
                testv[i]--;
            }
        }
    }
    void DiningPhilosophers::philosopher(int i) {
        while (true) {
            if (i < numOfPHILOSOPHERS) {
                Sleep(rand() % 1000 + 1000);
                queue.lock();
                test(i);
                queue.unlock();
    
                ArePhilosopherseating[i]->acquire();
                testv[i]--;
    
                test((i + (numOfPHILOSOPHERS - 1)) % numOfPHILOSOPHERS);
                test((i + 1) % numOfPHILOSOPHERS);
            }
        }
    }

     DiningPhilosophers::DiningPhilosophers() {
         for (int i = 0; i < INIT_VAL_OF_PHILOSOPHERS; i++) {
             int actualPhilosopherNumber = i;

             ArePhilosopherseating.push_back(make_shared<counting_semaphore<10>>(0));
             Philosophers.emplace_back(thread([this, actualPhilosopherNumber]() { philosopher(actualPhilosopherNumber); }));
             testv.push_back(0);
             StatesArrayforPhilosophers.push_back(0);
         }
     }

    void DiningPhilosophers::addPhilosopher() {

        if (numOfPHILOSOPHERS < 8) {
            numOfPHILOSOPHERS++;

            int currentPHILOSOPHERSID = numOfPHILOSOPHERS;
            if (numOfPHILOSOPHERS > ArePhilosopherseating.size()) ArePhilosopherseating.push_back(make_shared<counting_semaphore<10>>(0));
            if (numOfPHILOSOPHERS > Philosophers.size())Philosophers.emplace_back([this, currentPHILOSOPHERSID]() { philosopher(currentPHILOSOPHERSID); });
            if (numOfPHILOSOPHERS > StatesArrayforPhilosophers.size()) StatesArrayforPhilosophers.push_back(0);
            if (numOfPHILOSOPHERS > testv.size())testv.push_back(0);
            cout << "nowy filozof dodany. akuralna liczba filozofow: " << numOfPHILOSOPHERS << endl;

        }

    }

    void DiningPhilosophers::removePhilosopher() {
        if (numOfPHILOSOPHERS > 2) {

            numOfPHILOSOPHERS--;
            StatesArrayforPhilosophers[numOfPHILOSOPHERS - 1] = 0;

            cout << "filozof usuniety. akuralna liczba filozofow: " << numOfPHILOSOPHERS << endl;
        }
    }


    vector<int> DiningPhilosophers::GetStatesArrayforPhilosophers() { return StatesArrayforPhilosophers; }

    int DiningPhilosophers::GetNumOfPhilosophers() { return numOfPHILOSOPHERS; }

    vector<int> DiningPhilosophers::Gettest() { return testv; }
