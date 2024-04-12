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
#include "DiningPhilosophers.h"
#include "SDL.h"

using namespace std;


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;


int main(int argc, char* args[])
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Smokers Problem", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    DiningPhilosophers problem;

    vector<SDL_Rect> Philosophers;
    vector<SDL_Rect> Forks;
    SDL_Rect Buttons[6];
    SDL_Rect plus1[3], plus2[3], minus[3];

    int PHI_NUM = problem.GetNumOfPhilosophers();

    pair<int, int> Center(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2);
    int Radius = 200;

    vector<int> ForksStateArray = problem.GetStatesArrayforPhilosophers();
  

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

    float AnglePart = (2 * M_PI) / PHI_NUM;
    for (int i = 0; i < PHI_NUM; i++) {

        SDL_Rect rect;
        rect.x = Center.first + (int)(Radius * cos(AnglePart * i));
        rect.y = Center.second + (int)(Radius * sin(AnglePart * i));
        rect.w = 50;
        rect.h = 50;
        Philosophers.push_back(rect);
    }


    for (int i = 0; i < PHI_NUM; i++) {

        SDL_Rect rect;
        rect.x = Center.first + (int)(Radius * cos(AnglePart * i + AnglePart / 2));
        rect.y = Center.second + (int)(Radius * sin(AnglePart * i + AnglePart / 2));
        rect.w = 10;
        rect.h = 30;
        Forks.push_back(rect);
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
                                if (PHI_NUM < 8) {
                                    PHI_NUM++;
   
                                    problem.addPhilosopher();
                                    Philosophers.clear();
                                    Forks.clear();

                                    float AnglePart = (2 * M_PI)/ PHI_NUM;

                                    for (int i = 0; i < PHI_NUM; i++) {
                                         
                                         SDL_Rect rect;
                                         rect.x = Center.first + (int)(Radius * cos(AnglePart * i));
                                         rect.y = Center.second + (int)(Radius * sin(AnglePart * i));
                                         rect.w = 50;
                                         rect.h = 50;
                                         Philosophers.push_back(rect);     
                                    }

                                   
                                    for (int i = 0; i < PHI_NUM; i++) {
                                       
                                        SDL_Rect rect;
                                        rect.x = Center.first + (int)(Radius * cos(AnglePart * i + AnglePart / 2));
                                        rect.y = Center.second + (int)(Radius * sin(AnglePart * i + AnglePart / 2));
                                        rect.w = 10;
                                        rect.h = 30;
                                        Forks.push_back(rect);
                                    }
                                }
                            }
                            if (i == 1) {
                                if (PHI_NUM > 2) {
                                    PHI_NUM--;

                                    problem.removePhilosopher();
                                    Philosophers.clear();
                                    Forks.clear();

                                    float AnglePart = (2 * M_PI) / PHI_NUM;

                                    for (int i = 0; i < PHI_NUM; i++) {

                                        SDL_Rect rect;
                                        rect.x = Center.first + (int)(Radius * cos(AnglePart * i));
                                        rect.y = Center.second + (int)(Radius * sin(AnglePart * i));


                                        rect.w = 50;
                                        rect.h = 50;
                                        Philosophers.push_back(rect);
                                    }

                                    
                                    for (int i = 0; i < PHI_NUM; i++) {
                                        
                                        SDL_Rect rect;
                                        rect.x = Center.first + (int)(Radius * cos(AnglePart * i + AnglePart/2));
                                        rect.y = Center.second + (int)(Radius * sin(AnglePart * i + AnglePart / 2));
                                        rect.w = 10;
                                        rect.h = 30;
                                        Forks.push_back(rect);
                                    }
                                }
                            }
                        }
                    }

                }
            }
        }


        ForksStateArray = problem.GetStatesArrayforPhilosophers();
      
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < 2; i++)
        {
           
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(renderer, &Buttons[i]);
            
        
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        for (int i = 0; i < 1; i++) {
            SDL_RenderFillRect(renderer, &plus1[i]);
            SDL_RenderFillRect(renderer, &plus2[i]);
            SDL_RenderFillRect(renderer, &minus[i]);
        }
        

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (int i = 0; i < Philosophers.size(); i++) {
        
             if (ForksStateArray[i] == 1) {
                 SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                 SDL_RenderFillRect(renderer, &Philosophers[i]);
             }
             else {
                 SDL_SetRenderDrawColor(renderer, 255,0,0, 255);
                 SDL_RenderFillRect(renderer, &Philosophers[i]);
             }
   
        }
        for (int i = 0; i < Philosophers.size(); i++) {

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &Forks[i]);
        }
        for (int i = 0; i < Philosophers.size(); i++) {
       
            if (ForksStateArray[i] == 1) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

                SDL_RenderDrawLine(renderer, Forks[i].x+5, Forks[i].y+15, Philosophers[i].x+25, Philosophers[i].y + 25);

                SDL_RenderDrawLine(renderer, Forks[(i + Philosophers.size() - 1) % Philosophers.size()].x + 5,
                    Forks[(i + Philosophers.size() - 1) % Philosophers.size()].y + +5, Philosophers[i].x + 25, Philosophers[i].y + 25);

                SDL_RenderFillRect(renderer, &Forks[i]);
                SDL_RenderFillRect(renderer, &Forks[(i + Philosophers.size()-1) % Philosophers.size()]);
            }
        }
   
        SDL_RenderPresent(renderer);
        SDL_Delay(10);

    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

}
