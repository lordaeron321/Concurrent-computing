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
#include "book.h"

using namespace std;
class WritersAndReadersProblem {

	vector<shared_ptr<counting_semaphore<10>>> write;

	vector<shared_ptr<counting_semaphore<10>>> read;

	vector<shared_ptr<counting_semaphore<10>>> queue;

	vector<thread> writers_threads;

	vector<thread> readers_threads;

	vector<int> InteractionArrayForReaders;

	vector<int> InteractionArrayForWriters;

	vector<int> bookReadedCount;

	vector<shared_ptr<book>> books;

	const int MAX_READERS_NUM = 8;

	const int MAX_WRITERS_NUM = 8;

	int amountOfBooks = 1;

	int numOfWriters = 1;

	int numOfReaders = 1;

	public:

	WritersAndReadersProblem();

	void writer(int i);

	void reader(int i);
	
	vector<int> GetInteractionArrayForReaders();

	vector<int> GetInteractionArrayForWriters();

	vector<shared_ptr<book>> GetBooks();

	void addWriter();

	void removeWriter();

	void addReader();

	void removeReader();

	void addBook();

	void removeBook();

	void StartSimulation();

};

