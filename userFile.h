// alleen de getallen in dit bestand veranderen en het andere bestand compileren

// bepalen de oppervlakte van het gebied (geheugen verbruik is AREA_HEIGHT * AREA_WIDTH * sizeof(creature))
// doordat dit op de "stack" gealloceert is, kan dit niet zo groot worden.
#define AREA_HEIGHT 64
#define AREA_WIDTH 64

// bepaalt de eigenschappen van de populatie
#define MAX_SIZE 10
#define MAX_SIGHT 10
#define MAX_HEALTH 8
#define MAX_HUNGER 8
#define MAX_SPEED 10
#define MUTATION_RATE 25

// bepaalt de tijd die de populatie stabiel (overleden == geboren) mag zijn
#define STABLE_POP_TIME 10
