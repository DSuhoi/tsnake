#ifndef __COORDS_HPP__
#define __COORDS_HPP__

// Coordinate class
class Coords
{  
public:
    int x;
    int y;
    
    // Overloading comparison operators for coordinates
    friend bool operator==(Coords &cd1, Coords &cd2);
    friend bool operator!=(Coords &cd1, Coords &cd2);
};

#endif
