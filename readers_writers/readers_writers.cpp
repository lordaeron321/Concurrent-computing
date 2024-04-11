#include <iostream>
#include <thread>
#include <random>
#include <semaphore>
#include <mutex>
#include <windows.h>
#include <vector>
#include <chrono>
#include<conio.h>
#include "SDL.h"
#include "WritersAndReadersProblem.h"
#include "book.h"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

int main(int argc, char* args[]) {

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Init(SDL_INIT_EVERYTHING);
	
	window = SDL_CreateWindow("Readers Writers Problem", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	WritersAndReadersProblem simulation;
	simulation.StartSimulation();

	int NUM_WR = 1;
	int NUM_RE = 1;
	int NUM_BO = 1;

	vector<SDL_Rect> writers;
	vector<SDL_Rect> readers;
	vector<SDL_Rect> books;

	SDL_Rect plus1[3], plus2[3], minus[3];
	for (int i = 0; i < 3; i++){
		plus1[i].x = 905;
		plus1[i].y = 420-60*i;
		plus1[i].w = 40;
		plus1[i].h = 10;
		plus2[i].x = 920;
		plus2[i].y = 406 - 60 * i;
		plus2[i].w = 10;
		plus2[i].h = 40;

		minus[i].x = plus1[i].x + 60;
		minus[i].y = 420 - 60 * i;
		minus[i].h = 10;
		minus[i].w = 40;
	}

	SDL_Rect Buttons[6];
	for (int i = 0; i < 6; i++)
	{
		if (i % 2 == 0) {
			Buttons[i].x = 900;
			Buttons[i].y = 400 - 30 * i;		
			Buttons[i].w = 50;		
			Buttons[i].h = 50;		
		}
		else {
			Buttons[i].x = 960;
			Buttons[i].y = 400 - 30 * (i-1);
			Buttons[i].w = 50;
			Buttons[i].h = 50;
		}
	}

	for (int i = 0; i < NUM_WR; i++){
		SDL_Rect rect;
		rect.x = 80;
		rect.y = SCREEN_HEIGHT / (NUM_WR + 1) * (i + 1) - 20;
		rect.w = 50;
		rect.h = 50;
		writers.push_back(rect);
	}

	for (int i = 0; i < NUM_RE; i++){
		SDL_Rect rect;
		rect.x = 640;
		rect.y = SCREEN_HEIGHT / (NUM_RE + 1) * (i + 1) - 20;
		rect.w = 50;
		rect.h = 50;
		readers.push_back(rect);
	}

	for (int i = 0; i < NUM_BO; i++){
		SDL_Rect rect;
		rect.x = 360;
		rect.y = SCREEN_HEIGHT / (NUM_BO + 1) * (i + 1) - 20;
		rect.w = 50;
		rect.h = 50;
		books.push_back(rect);
	}

	vector<int> WritersInteractionArray;
	vector<int> ReadersInteractionArray;
	vector<shared_ptr<book>> booksInfo;

	while (true) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {	
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					int mouseX, mouseY;
					SDL_GetMouseState(&mouseX, &mouseY);
					for (int i = 0; i < 6; i++){
						if (mouseX >= Buttons[i].x && mouseX <= Buttons[i].x + 50
							&& mouseY >= Buttons[i].y && mouseY <= Buttons[i].y + 50) {
							
							if (i == 0) {
								if (NUM_RE < 8) {
									NUM_RE++;
									simulation.addReader();
									readers.clear();

									for (int i = 0; i < NUM_RE; i++){
										SDL_Rect rect;
										rect.x = 640;
										rect.y = SCREEN_HEIGHT / (NUM_RE + 1) * (i + 1) - 20;
										rect.w = 50;
										rect.h = 50;
										readers.push_back(rect);
									}
								}
							}
							else if (i == 1) {
								if (NUM_RE > 1) {
									NUM_RE--;
									simulation.removeReader();
									readers.clear();
									for (int i = 0; i < NUM_RE; i++){
										SDL_Rect rect;
										rect.x = 640;
										rect.y = SCREEN_HEIGHT / (NUM_RE + 1) * (i + 1) - 20;
										rect.w = 50;
										rect.h = 50;
										readers.push_back(rect);
									}
								}
							}
							else if (i == 2) {
								if (NUM_WR < 8) {
									NUM_WR++;
									simulation.addWriter();
									writers.clear();
									for (int i = 0; i < NUM_WR; i++){
										SDL_Rect rect;
										rect.x = 80;
										rect.y = SCREEN_HEIGHT / (NUM_WR + 1) * (i + 1) - 20;
										rect.w = 50;
										rect.h = 50;
										writers.push_back(rect);
									}
								}
							}								
							else if (i == 3) {
								if (NUM_WR > 1) {
									NUM_WR--;
									simulation.removeWriter();
									writers.clear();
									for (int i = 0; i < NUM_WR; i++){
										SDL_Rect rect;
										rect.x = 80;
										rect.y = SCREEN_HEIGHT / (NUM_WR + 1) * (i + 1) - 20;
										rect.w = 50;
										rect.h = 50;
										writers.push_back(rect);
									}
								}
						
							}
							else if (i == 4) {
								if (NUM_BO < 8) {
									NUM_BO++;
									simulation.addBook();
									books.clear();
									for (int i = 0; i < NUM_BO; i++){
										SDL_Rect rect;
										rect.x = 360;
										rect.y = SCREEN_HEIGHT / (NUM_BO + 1) * (i + 1) - 20;
										rect.w = 50;
										rect.h = 50;
										books.push_back(rect);
									}
								}
							}
							else if (i == 5) {
								if (NUM_BO > 1) {
									NUM_BO--;
									simulation.removeBook();
									books.clear();
									for (int i = 0; i < NUM_BO; i++){
										SDL_Rect rect;
										rect.x = 360;
										rect.y = SCREEN_HEIGHT / (NUM_BO + 1) * (i + 1) - 20;
										rect.w = 50;
										rect.h = 50;
										books.push_back(rect);
									}
								}
							}
						}
					}
				}
			}
		}
		WritersInteractionArray = simulation.GetInteractionArrayForWriters();
		ReadersInteractionArray = simulation.GetInteractionArrayForReaders();
		booksInfo = simulation.GetBooks();
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		for (int i = 0; i < 6; i++)
		{
			
			if (i >= 0 && i <= 1) {
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				SDL_RenderFillRect(renderer, &Buttons[i]);
			}
			else if (i >= 2 && i <= 3) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderFillRect(renderer, &Buttons[i]);
			}
			else if (i >= 4 && i <= 6) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				SDL_RenderFillRect(renderer, &Buttons[i]);
			}
		}
	
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		for (int i = 0; i < 3; i++){
			SDL_RenderFillRect(renderer, &plus1[i]);
			SDL_RenderFillRect(renderer, &plus2[i]);
			SDL_RenderFillRect(renderer, &minus[i]);
		}

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		for (int i = 0; i < NUM_WR; i++){
			
			if ((WritersInteractionArray[i]) > -1 && WritersInteractionArray[i] < books.size()) {
				SDL_RenderDrawLine(renderer, writers[i].x + 25, writers[i].y + 25,
					books[(WritersInteractionArray[i])].x + 25, books[(WritersInteractionArray[i])].y + 25);
			}
		}
		for (int i = 0; i < NUM_WR; i++){
			SDL_RenderFillRect(renderer, &writers[i]);
		}

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	
		for (int i = 0; i <NUM_RE; i++){
			if ((ReadersInteractionArray[i]) > -1 && ReadersInteractionArray[i] < books.size()) {
				SDL_RenderDrawLine(renderer, readers[i].x + 25, readers[i].y + 25,
					books[(ReadersInteractionArray[i])].x + 25, books[(ReadersInteractionArray[i])].y + 25);	
			}
		}

		for (int i = 0; i < NUM_RE; i++){
			SDL_RenderFillRect(renderer, &readers[i]);
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		for (int i = 0; i < NUM_BO; i++){
			SDL_RenderFillRect(renderer, &books[i]);
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}