#include "card.h"
#include <string.h>

const char* rarity_to_string(Rarity rarity) {
    switch(rarity) {
        case RARITY_B: return "B";
        case RARITY_A: return "A";
        case RARITY_S: return "S";
        default: return "?";
    }
}