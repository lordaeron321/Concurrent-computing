#pragma once
class book {
public :
	int bookText;
	bool hasAlreadyBeenWritten;
	int readCount;

	book() {
		bookText = 0;
		hasAlreadyBeenWritten = false;
		readCount = 0;
	}
};


