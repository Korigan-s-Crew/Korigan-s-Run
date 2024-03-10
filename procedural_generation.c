#include "procedural_generation.h"
#include "example_2d.h"

Map* generated_pattern()
{
    return create_map("Patterns/pattern.txt");
}
