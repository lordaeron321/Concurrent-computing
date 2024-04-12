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
using namespace std;
	
	

class SmokersProblem {

	const int INIT_val_CigaretteTampers = 1;
	const int INIT_val_Matchboxes = 1;
	int numOfMatchboxes = INIT_val_Matchboxes;
	int numOfCigaretteTampers = INIT_val_CigaretteTampers;
	int numOfSmokers = 0;
	
	counting_semaphore<10> CigaretteTampers{ INIT_val_CigaretteTampers };
	counting_semaphore<10> Matchboxes{ INIT_val_Matchboxes };
	counting_semaphore<10> queue{ 1 };


	vector<thread> smokers;
	
	vector<int> StatesArrayforTempers;
	vector<int> StatesArrayforMatches;
	
	static int nextSmokerID;
	
	mutex mtxSmokers;
	mutex mtxMatches;
	mutex mtxTempers;

	mutex mtxTempersArray;
	mutex mtxMatchesArray;


	void smoker(int i) {
		while (true) {
			if (i <= numOfSmokers) {
				queue.acquire();

                mtxTempers.lock();
				    CigaretteTampers.acquire();
                    
                mtxTempers.unlock();

				int usableTampers = 0;
			        mtxTempersArray.lock(); 
			        while (usableTampers < StatesArrayforTempers.size() && StatesArrayforTempers[usableTampers] != 0)
			        	usableTampers++;
			        if (usableTampers < StatesArrayforTempers.size())
			        	StatesArrayforTempers[usableTampers] = i;
                        cout << "smoker nr " << i + 1 << " is taking cigarette tamper nr: "<<usableTampers+1<<"\n";
			        mtxTempersArray.unlock(); 

				queue.release();

                mtxMatches.lock();
				    Matchboxes.acquire();
                mtxMatches.unlock();

				int usableMatches = 0;
			     mtxMatchesArray.lock(); 
			     while (usableMatches < StatesArrayforMatches.size() && StatesArrayforMatches[usableMatches] != 0)
			     	usableMatches++;
			     if (usableMatches < StatesArrayforMatches.size())
			     	StatesArrayforMatches[usableMatches] = i;
                 cout << "smoker nr " << i+1 << " is taking matchbox nr: " << usableMatches + 1 << "\n";
			     mtxMatchesArray.unlock(); 
                
				cout << "smoker nr " << i + 1 << " is smoking\n";
				Sleep(rand()%1000+1000);

				StatesArrayforMatches[usableMatches] = 0;

				Matchboxes.release();

				StatesArrayforTempers[usableTampers] = 0;

				CigaretteTampers.release();
			}
		}
	}

	public:

	SmokersProblem(){ }
		
	void addSmoker() {

		if (numOfSmokers < 8) {
		
			numOfSmokers++;
			int currentSmokerID = numOfSmokers;
            if (numOfSmokers > smokers.size())smokers.emplace_back([this, currentSmokerID]() { smoker(currentSmokerID); });
			cout << "New smoker added. Actual amont of smokers: " << numOfSmokers << endl;

		}

	}

	void addMatchboxes() {
		
		
        if ( numOfMatchboxes < 9) {
            numOfMatchboxes++;
            if(numOfMatchboxes > StatesArrayforMatches.size())StatesArrayforMatches.push_back(0);
            mtxMatches.lock();
            Matchboxes.release();
            mtxMatches.unlock();
        }

		cout << "New matchbox added. Actual amont of matchboxes: " << numOfMatchboxes << endl;
	}
	void addCigaretteTampers() {
	    

        if ( numOfCigaretteTampers < 9) {
            numOfCigaretteTampers++;
            if(numOfCigaretteTampers > StatesArrayforTempers.size())StatesArrayforTempers.push_back(0);
            mtxTempers.lock();
	        CigaretteTampers.release();
            mtxTempers.unlock();
        }
	
		cout << "New cigaretter tamper added. Actual amont of cigarette tampers: " << numOfCigaretteTampers << endl;
	
	}
	
	void removeSmoker() {
		
        if (numOfSmokers > 1) {
            numOfSmokers--;      
        }
		cout << "Matchbox deleted. Actual amont of matchboxes: "<< numOfSmokers<<endl;
		
	}
	void removeMatchboxes() {
		
           
		if (numOfMatchboxes > 0) {
            numOfMatchboxes--;
        
            mtxMatches.lock();
            Matchboxes.acquire();
            mtxMatches.unlock();
           
			cout << "Matchbox deleted. Actual amont of matchboxes: " << numOfMatchboxes << endl;
		}
			
	}
	void removeCigaretteTampers() {
		

		if (numOfCigaretteTampers > 0) {
			numOfCigaretteTampers--;
            mtxTempers.lock();
	        CigaretteTampers.acquire();
            mtxTempers.unlock();
			
			cout << "Cigarette tamper deleted. Actual number of cigarette tampers: " << numOfCigaretteTampers << endl;
		}
		
	}

	void StartSimulation() {
		for (int i = 0; i < INIT_val_CigaretteTampers; i++) {
			StatesArrayforTempers.push_back(0);
		}
		for (int i = 0; i < INIT_val_Matchboxes; i++) {
			StatesArrayforMatches.push_back(0);
		}
		addSmoker(); 
	}
    int GetRealSmokersSize() { return smokers.size(); }

	int GetNumOfSmokers() { return numOfSmokers ; }
	
	int GetNumOfCigaretteTempers() { return numOfCigaretteTampers; }
	
	int GetNumOfMatchboxes() { return numOfMatchboxes; }
	
	vector<int> GetStatesArrayforTempers(){ return StatesArrayforTempers; }
	
	vector<int> GetStatesArrayforMatches(){ return StatesArrayforMatches; }

	vector<bool> GetCurrentSmokingSmokersArray() {
		vector<bool> CurrentSmokingSmokersArray;

		CurrentSmokingSmokersArray.resize(numOfSmokers, 0);
    
	for (int i = 0; i < StatesArrayforTempers.size(); i++)
	{
        if (StatesArrayforTempers[i] > 0) {
			int currentSmoker = StatesArrayforTempers[i];
			for (int j = 0; j < StatesArrayforMatches.size(); j++){
				if (StatesArrayforMatches[j] == currentSmoker && currentSmoker - 1 < CurrentSmokingSmokersArray.size())CurrentSmokingSmokersArray[currentSmoker-1] = 1;
			}
		}
	}
		return CurrentSmokingSmokersArray;
	}

};
int SmokersProblem::nextSmokerID = 1;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

int main(int argc, char* args[])
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Smokers Problem", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SmokersProblem problem;
	problem.StartSimulation();
	
    vector<SDL_Rect> Tampers;
    vector<SDL_Rect> Matchboxes;
    vector<SDL_Rect> Smokers;
    SDL_Rect Buttons[6];
    SDL_Rect plus1[3], plus2[3], minus[3];
    
    int NUM_OF_TAMPERS = problem.GetNumOfCigaretteTempers();
    int NUM_OF_MATCHBOXES = problem.GetNumOfMatchboxes();
    int NUM_OF_SMOKERS = problem.GetNumOfSmokers();
    int total_items = NUM_OF_TAMPERS + NUM_OF_MATCHBOXES;
    vector<int> TampersArray = problem.GetStatesArrayforTempers();
    vector<int> MatchesArray = problem.GetStatesArrayforMatches();
    int RealSmokersSize = problem.GetRealSmokersSize();
    vector<bool> SmokersArray;

    SmokersArray.resize(NUM_OF_SMOKERS, 0);

  
   
    for (int i = 0; i < 3; i++) {
    	plus1[i].x = 905;
    	plus1[i].y = 420 - 60 * i;
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
    		Buttons[i].y = 400 - 30 * (i - 1);
    		Buttons[i].w = 50;
    		Buttons[i].h = 50;
    	}
    }
    
    for (int i = 0; i < total_items; i++)
    {
    	if (i < NUM_OF_TAMPERS) {
    		SDL_Rect rect;
    		rect.x = 80;
    		rect.y = SCREEN_HEIGHT / (total_items + 1) * (i + 1) - 20;
    		rect.w = 50;
    		rect.h = 50;
    		Tampers.push_back(rect);
    	}
    
    	else {
    		SDL_Rect rect;
    		rect.x = 80;
    		rect.y = SCREEN_HEIGHT / (total_items + 1) * (i + 1) - 20;
    		rect.w = 50;
    		rect.h = 50;
    		Matchboxes.push_back(rect);
    	}
    }
    
    for (int i = 0; i < NUM_OF_SMOKERS; i++)
    {
    	SDL_Rect rect;
    	rect.x = 590;
    	rect.y = SCREEN_HEIGHT / (NUM_OF_SMOKERS + 1) * (i + 1) - 20;
    	rect.w = 50;
    	rect.h = 50;
    	Smokers.push_back(rect);
    
    }
    

    while (true) {
    
    	
    
    	SDL_Event e;
    	while (SDL_PollEvent(&e) != 0) {
    		if (e.type == SDL_MOUSEBUTTONDOWN) {
    			if (e.button.button == SDL_BUTTON_LEFT) {
    
    				int mouseX, mouseY;
    				SDL_GetMouseState(&mouseX, &mouseY);
    				for (int i = 0; i < 6; i++) {
    					if (mouseX >= Buttons[i].x && mouseX <= Buttons[i].x + 50
    						&& mouseY >= Buttons[i].y && mouseY <= Buttons[i].y + 50) {
    
    						if (i == 0) {
    							if (NUM_OF_TAMPERS < 8) {
    								NUM_OF_TAMPERS++;
    								total_items = NUM_OF_TAMPERS + NUM_OF_MATCHBOXES;
    								problem.addCigaretteTampers();
    								Tampers.clear();
    								Matchboxes.clear();
    								
    									for (int i = 0; i < total_items; i++)
    									{
    										if (i < NUM_OF_TAMPERS) {
    											SDL_Rect rect;
    											rect.x = 80;
    											rect.y = SCREEN_HEIGHT / (total_items + 1) * (i + 1) - 20;
    											rect.w = 50;
    											rect.h = 50;
    											Tampers.push_back(rect);
    										}
    								
    										else {
    											SDL_Rect rect;
    											rect.x = 80;
    											rect.y = SCREEN_HEIGHT / (total_items + 1) * (i + 1) - 20;
    											rect.w = 50;
    											rect.h = 50;
    											Matchboxes.push_back(rect);

    										}
    									}
    								
    							}
    						}
    					    else if (i == 1) {
    						if (NUM_OF_TAMPERS > 1) {
    							NUM_OF_TAMPERS--;
    							total_items = NUM_OF_TAMPERS + NUM_OF_MATCHBOXES;
    							problem.removeCigaretteTampers();
    							Tampers.clear();
    							Matchboxes.clear();
    							for (int i = 0; i < total_items; i++)
    							{
    								if (i < NUM_OF_TAMPERS) {
    									SDL_Rect rect;
    									rect.x = 80;
    									rect.y = SCREEN_HEIGHT / (total_items + 1) * (i + 1) - 20;
    									rect.w = 50;
    									rect.h = 50;
    									Tampers.push_back(rect);
  
    								}
    							
    								else {
    									SDL_Rect rect;
    									rect.x = 80;
    									rect.y = SCREEN_HEIGHT / (total_items + 1) * (i + 1) - 20;
    									rect.w = 50;
    									rect.h = 50;
    									Matchboxes.push_back(rect);

    								}
    							}
    						}
    					    }
    						else if (i == 2) {
    							if (NUM_OF_MATCHBOXES < 8) {
    								NUM_OF_MATCHBOXES++;
    								total_items = NUM_OF_TAMPERS + NUM_OF_MATCHBOXES;
    								problem.addMatchboxes();
    								Tampers.clear();
    								Matchboxes.clear();
    								for (int i = 0; i < total_items; i++)
    								{
    									if (i < NUM_OF_TAMPERS) {
    										SDL_Rect rect;
    										rect.x = 80;
    										rect.y = SCREEN_HEIGHT / (total_items + 1) * (i + 1) - 20;
    										rect.w = 50;
    										rect.h = 50;
    										Tampers.push_back(rect);

    									}
    								
    									else {
    										SDL_Rect rect;
    										rect.x = 80;
    										rect.y = SCREEN_HEIGHT / (total_items + 1) * (i + 1) - 20;
    										rect.w = 50;
    										rect.h = 50;
    										Matchboxes.push_back(rect);

    									}
    								}
    							}
    						}
    						else if (i == 3) {
    							if (NUM_OF_MATCHBOXES > 1) {
    								NUM_OF_MATCHBOXES--;
    								total_items = NUM_OF_TAMPERS + NUM_OF_MATCHBOXES;
    								problem.removeMatchboxes();
    								Tampers.clear();
    								Matchboxes.clear();
    								
    								for (int i = 0; i < total_items; i++)
    								{
    									if (i < NUM_OF_TAMPERS) {
    										SDL_Rect rect;
    										rect.x = 80;
    										rect.y = SCREEN_HEIGHT / (total_items + 1) * (i + 1) - 20;
    										rect.w = 50;
    										rect.h = 50;
    										Tampers.push_back(rect);
    									}
    								
    									else {
    										SDL_Rect rect;
    										rect.x = 80;
    										rect.y = SCREEN_HEIGHT / (total_items + 1) * (i + 1) - 20;
    										rect.w = 50;
    										rect.h = 50;
    										Matchboxes.push_back(rect);

    									}
    								}
    							}
                            
    						}
    						else if (i == 4) {
    							if (NUM_OF_SMOKERS < 8) {
    								NUM_OF_SMOKERS++;
    								problem.addSmoker();
    								Smokers.clear();
    								for (int i = 0; i < NUM_OF_SMOKERS; i++)
    								{
    									SDL_Rect rect;
    									rect.x = 590;
    									rect.y = SCREEN_HEIGHT / (NUM_OF_SMOKERS + 1) * (i + 1) - 20;
    									rect.w = 50;
    									rect.h = 50;
    									Smokers.push_back(rect);
    								
    									cout << endl;
    								}
    							}
    						}
    						else if (i == 5) {
    							if (NUM_OF_SMOKERS > 1) {
    								NUM_OF_SMOKERS--;
    								problem.removeSmoker();
    								Smokers.clear();
    								for (int i = 0; i < NUM_OF_SMOKERS; i++)
    								{
    									SDL_Rect rect;
    									rect.x = 590;
    									rect.y = SCREEN_HEIGHT / (NUM_OF_SMOKERS + 1) * (i + 1) - 20;
    									rect.w = 50;
    									rect.h = 50;
    									Smokers.push_back(rect);
    								
    									cout << endl;
                            
    								}
    							}
    						}
    					}
    				}
    
    			}
    		}
    	}
    		
    	TampersArray = problem.GetStatesArrayforTempers();
    	MatchesArray = problem.GetStatesArrayforMatches();
    	SmokersArray = problem.GetCurrentSmokingSmokersArray();

        while (TampersArray.size() > NUM_OF_TAMPERS)TampersArray.pop_back();
        while (MatchesArray.size() > NUM_OF_MATCHBOXES)MatchesArray.pop_back();
        while (SmokersArray.size() > NUM_OF_SMOKERS)SmokersArray.pop_back();

     
    	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    	SDL_RenderClear(renderer);
    	
    	for (int i = 0; i < 6; i++)
    	{
    	
    		if (i >= 0 && i <= 1) {
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    			SDL_RenderFillRect(renderer, &Buttons[i]);
    		}
    		else if (i >= 2 && i <= 3) {
                SDL_SetRenderDrawColor(renderer, 130, 23, 40, 255);
    			SDL_RenderFillRect(renderer, &Buttons[i]);
    		}
    		else if (i >= 4 && i <= 6) {
    			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    			SDL_RenderFillRect(renderer, &Buttons[i]);
    		}
    	}
    	
    	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    	for (int i = 0; i < 3; i++) {
    		SDL_RenderFillRect(renderer, &plus1[i]);
    		SDL_RenderFillRect(renderer, &plus2[i]);
    		SDL_RenderFillRect(renderer, &minus[i]);
    	}
    	
    	for (int i = 0; i < SmokersArray.size(); i++) {
    	
    		if (i < Smokers.size() ) {
    				if (SmokersArray[i] == 1) {
    				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    				SDL_RenderFillRect(renderer, &Smokers[i]);
    			}
    			else {
    				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    				SDL_RenderFillRect(renderer, &Smokers[i]);
    			}
    		}
    		
    	
    	}
    	
    	
    	for (int i = 0; i < TampersArray.size(); i++) {
               
            if (TampersArray[i] > 0 && TampersArray[i] - 1 < SmokersArray.size()) {

                if (TampersArray[i] != 0 && SmokersArray[TampersArray[i] - 1] == 0) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderDrawLine(renderer, Smokers[TampersArray[i] - 1].x + 25, Smokers[TampersArray[i] - 1].y + 25,
                        Tampers[i].x + 25, Tampers[i].y + 25);
                }
                else if (TampersArray[i] != 0 && SmokersArray[TampersArray[i] - 1] == 1 && TampersArray[i] - 1 < Smokers.size() && TampersArray[i] - 1 > -1) {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    SDL_RenderDrawLine(renderer, Smokers[TampersArray[i] - 1].x + 25, Smokers[TampersArray[i] - 1].y + 25,
                        Tampers[i].x + 25, Tampers[i].y + 25);
                }
            }
    		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    		if (i < Tampers.size())SDL_RenderFillRect(renderer, &Tampers[i]);
    	}
    	
    	
    	for (int i = 0; i < MatchesArray.size(); i++) {
    	
            if (MatchesArray[i] > 0 && MatchesArray[i] - 1 < SmokersArray.size()) {
                if (MatchesArray[i] != 0 && SmokersArray[MatchesArray[i] - 1] == 0 && MatchesArray[i] - 1 < Smokers.size() && MatchesArray[i] - 1 > -1) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderDrawLine(renderer, Smokers[MatchesArray[i] - 1].x + 25, Smokers[MatchesArray[i] - 1].y + 25,
                        Matchboxes[i].x + 25, Matchboxes[i].y + 25);
                }
                else if (MatchesArray[i] != 0 && SmokersArray[MatchesArray[i] - 1] == 1 && MatchesArray[i] - 1 < Smokers.size() && MatchesArray[i] - 1 > -1) {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    SDL_RenderDrawLine(renderer, Smokers[MatchesArray[i] - 1].x + 25, Smokers[MatchesArray[i] - 1].y + 25,
                        Matchboxes[i].x + 25, Matchboxes[i].y + 25);
                }
            }
    			SDL_SetRenderDrawColor(renderer, 130, 23, 40, 255);
    			if(i < Matchboxes.size())SDL_RenderFillRect(renderer, &Matchboxes[i]);
    	}
    	
    	SDL_RenderPresent(renderer);
    	SDL_Delay(10);
    
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
		return 0;
	
}