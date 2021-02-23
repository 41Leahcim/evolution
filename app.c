#include <stdio.h>
#include <stdlib.h>
#include "userFile.h"

typedef struct creature{
    int size, sight, hunger, health, speed, id;
    char eatable;
} creature;

creature* newCreature(int id){
    creature* c = malloc(sizeof(creature));
    c->size = rand() % MAX_SIZE + 1;
    c->sight = rand() % MAX_SIGHT + 1;
    c->health = MAX_HEALTH;
    c->hunger = MAX_HUNGER;
    c->speed = rand() % MAX_SPEED + 1;
    c->id = id;
    c->eatable = 0;
    return c;
}

creature* newChild(creature* parent, int id){
    creature* child = malloc(sizeof(creature));
    child->size = parent->size + (parent->size * MUTATION_RATE - (MAX_SIZE * MUTATION_RATE / 2)) / 100;
    child->size = parent->sight + (parent->sight * MUTATION_RATE - (MAX_SIGHT * MUTATION_RATE / 2)) / 100;
    child->health = MAX_HEALTH;
    child->hunger = MAX_HUNGER / 2;
    child->size = parent->speed + (parent->speed * MUTATION_RATE - (MAX_SPEED * MUTATION_RATE / 2)) / 100;
    child->id  = id;
    child->eatable = 0;
    parent->hunger /= 2;
    return child;
}

char validx(int x){
    return x >= 0 && x <= AREA_WIDTH - 1;
}

char validy(int y){
    return y >= 0 && y <= AREA_HEIGHT - 1;
}

void printInfo(int step, int born, int alive, int died){
    printf("\n\n==========================\n\nStep: #%d\nborn: %d\nalive: %d\ndied: %d\n\n==========================\n\n", step, born, alive, died);
}

int main(){
    int creatures = 0;
    int nextId = 0;
    int stable = 0;
    int born = 0, died = 0, step = 0;
    creature* area[AREA_HEIGHT][AREA_WIDTH];
    for(int y = 0;y < AREA_HEIGHT;y++){
        for(int x = 0;x < AREA_WIDTH;x++){
            if(rand() & (1 << 11)){
                area[y][x] = newCreature(nextId);
                nextId++;
                creatures++;
                born++;
            }else{
                area[y][x] = NULL;
            }
        }
    }
    while(creatures && stable < STABLE_POP_TIME){
        printInfo(step, born, creatures, died);
        printf("Press enter to continue... ");
        while(getchar() != '\n');
        step++;
        born = 0;
        died = 0;
        for(int y = 0;y < AREA_HEIGHT;y++){
            for(int x = 0;x < AREA_WIDTH;x++){
                if(area[y][x] != NULL){
                    area[y][x]->eatable = 1;
                    int bestx = x, besty = y, bestScore = 0;
                    area[y][x]->health--;
                    area[y][x]->hunger--;
                    for(int y2 = validy(y - area[y][x]->sight)?(y - area[y][x]->sight):0;y2 < AREA_HEIGHT;y2++){
                        for(int x2 = validx(x - area[y][x]->sight)?x - area[y][x]->sight:0;x2 < AREA_WIDTH;x2++){
                            if(area[y2][x2] != NULL && (area[y][x]->size + area[y][x]->health) - (area[y2][x2]->size + area[y2][x2]->health) > bestScore && area[y][x]->hunger < MAX_HUNGER * 0.7 && area[y][x]->health > area[y][x]->size - area[y2][x2]->size){
                                bestx = x2;
                                besty = y2;
                            }
                        }
                    }
                    if(bestx == x || besty == y){
                        if(y > 0 && area[y - 1][x] ==  NULL) besty--;
                        else if(x > 0 && area[y][x - 1] == NULL) bestx--;
                        else if(y + 1 < AREA_HEIGHT && area[y + 1][x] == NULL) besty++;
                        else if(x + 1 < AREA_WIDTH && area[y][x + 1] == NULL) bestx++;
                    }
                    if(area[besty][bestx] != NULL){
                        area[y][x]->health -= area[besty][bestx]->size + area[besty][bestx]->health;
                        area[y][x]->hunger += area[besty][bestx]->size;
                        free(area[besty][bestx]);
                        area[besty][bestx] = area[y][x];
                        area[y][x] = NULL;
                        died++;
                        creatures--;
                    }else if(area[y][x]->hunger <= 0 || area[y][x]->health <= 0){
                        free(area[y][x]);
                        area[y][x] = NULL;
                        creatures--;
                        died++;
                    }else if(area[y][x]->hunger < MAX_HEALTH / 3 || area[y][x]->hunger < MAX_HUNGER / 3){
                        area[besty][bestx] = area[y][x];
                        area[y][x] = NULL;
                    }else{
                        area[besty][bestx] = area[y][x];
                        area[y][x] = newCreature(nextId);
                        area[y][x] = newChild(area[besty][bestx], nextId);
                        nextId++;
                        creatures++;
                        born++;
                    }
                }
            }
        }
        if(born == died){
            stable++;
        }else{
            stable = 0;
        }
    }
}