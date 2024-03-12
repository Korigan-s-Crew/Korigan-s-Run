#include "procedural_generation.h"

#include "main.h"

Map* generated_pattern() {
    return create_map("Patterns/pattern.txt");
}
