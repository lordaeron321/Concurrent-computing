#include "WritersAndReadersProblem.h"

using namespace std;

void WritersAndReadersProblem::writer(int i) {

	srand(time(NULL));

	while (true) {

		if (i <= numOfWriters) {
			int bookNumber = rand() % amountOfBooks;

			queue[bookNumber]->acquire();
			write[bookNumber]->acquire();
			queue[bookNumber]->release();

			InteractionArrayForWriters[i - 1] = bookNumber;

			int newBookText = rand() % 10 + 1;

			if (bookReadedCount[bookNumber] > 3 || books[bookNumber]->hasAlreadyBeenWritten == false) {
				cout << "writer nr " << i << " is changing book " << bookNumber + 1 << " text from " << books[bookNumber]->bookText << " to " << newBookText << "\n";
				books[bookNumber]->bookText = newBookText;
				if (books[bookNumber]->hasAlreadyBeenWritten == false) books[bookNumber]->hasAlreadyBeenWritten = true;
				bookReadedCount[bookNumber] = 0;
				std::this_thread::sleep_for(500ms);
			}

			InteractionArrayForWriters[i - 1] = -1;
			write[bookNumber]->release();
		}

	}
}
void WritersAndReadersProblem::reader(int i) {
	srand(time(NULL));
	while (true) {

		if (i <= numOfReaders) {
			int bookNumber = rand() % amountOfBooks;
			if (books[bookNumber]->hasAlreadyBeenWritten != false) {
				queue[bookNumber]->acquire();
				read[bookNumber]->acquire();

				books[bookNumber]->readCount++;
				bookReadedCount[bookNumber]++;


				if (books[bookNumber]->readCount == 1) {
					write[bookNumber]->acquire();
				}
				InteractionArrayForReaders[i - 1] = bookNumber;
				queue[bookNumber]->release();
				read[bookNumber]->release();

				cout << "reader nr " << i << " is reading book nr " << bookNumber + 1 << " and there is text: " << books[bookNumber]->bookText << "\n";
				std::this_thread::sleep_for(500ms);

				read[bookNumber]->acquire();
				books[bookNumber]->readCount--;


				InteractionArrayForReaders[i - 1] = -1;

				if (books[bookNumber]->readCount == 0) {

					write[bookNumber]->release();

				}
				read[bookNumber]->release();
			}
		}

	}
}


WritersAndReadersProblem::WritersAndReadersProblem() {
			for (int i = 0; i < MAX_READERS_NUM; i++)InteractionArrayForReaders.push_back(-1);
			for (int i = 0; i < MAX_WRITERS_NUM; i++)InteractionArrayForWriters.push_back(-1);

}
vector<int> WritersAndReadersProblem::GetInteractionArrayForReaders() { return InteractionArrayForReaders; }

vector<int> WritersAndReadersProblem::GetInteractionArrayForWriters() { return InteractionArrayForWriters; }

vector<shared_ptr<book>> WritersAndReadersProblem::GetBooks() { return books; }

void WritersAndReadersProblem::addWriter() {
	if (numOfWriters < 9) {
		numOfWriters++;
		if (numOfWriters > writers_threads.size()) {
			writers_threads.emplace_back(thread([this]() {writer(numOfWriters); }));
		}
		cout << "new writer added " << numOfWriters << endl;
	}
	else cout << "max level of writers reached! " << endl;
}

void WritersAndReadersProblem::removeWriter() {
	numOfWriters--;
	cout << "last writer removed. Current amount of writers: " << numOfWriters << endl;
}

void WritersAndReadersProblem::addReader() {
	if (numOfReaders < 9) {
		numOfReaders++;
		if (numOfReaders > readers_threads.size()) {
			readers_threads.emplace_back(thread([this]() {reader(numOfReaders); }));
		}
		cout << "new reader added " << numOfReaders << endl;
	}
	else cout << "max level of readers reached! " << endl;
}

void WritersAndReadersProblem::removeReader() {
	numOfReaders--;
	cout << "last reader removed. Current amount of readers: " << numOfReaders << endl;
}

void WritersAndReadersProblem::addBook() {
	if (amountOfBooks < 9) {
		amountOfBooks++;
		if (amountOfBooks > books.size()) {
			write.push_back(make_shared<counting_semaphore<10>>(1));
			read.push_back(make_shared<counting_semaphore<10>>(1));
			queue.push_back(make_shared<counting_semaphore<10>>(1));
			auto newBook = make_shared<book>();
			books.push_back(newBook);
			bookReadedCount.push_back(0);

			cout << "new book added " << amountOfBooks << endl;
		}
		else cout << "new book added " << amountOfBooks << endl;
	}
	else cout << "max level of books reached! " << endl;
}

void WritersAndReadersProblem::removeBook() {

	if (amountOfBooks > 1) {
		amountOfBooks--;
		cout << "last book removed. Current amount of books: " << amountOfBooks << endl;
	}
	else cout << "there is no purpose here to have 0 books" << endl;
}
void WritersAndReadersProblem::StartSimulation() {
	write.push_back(make_shared<counting_semaphore<10>>(1));
	read.push_back(make_shared<counting_semaphore<10>>(1));
	queue.push_back(make_shared<counting_semaphore<10>>(1));

	writers_threads.emplace_back(thread([this]() {writer(1); }));
	readers_threads.emplace_back(thread([this]() {reader(1); }));


	auto b1 = make_shared<book>();
	books.push_back(b1);

	bookReadedCount.push_back(0);
}