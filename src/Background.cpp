#include "Background.h"

Background::Background()
{
    ofBackground(ofColor::lavender);
}

void Background::changeBackground()
{
    //get the color of the terrain
    ofColor terrain_color = terrain->getColor();
    //create a new variable with the complement of the color
    ofColor complement_terrain= terrain_color.invert();
    //assign this complement to the current color variable of background
    color=complement_terrain;
    //draw the background with its color variable
    ofBackground(color);
    
}
