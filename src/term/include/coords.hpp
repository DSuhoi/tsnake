#ifndef __COORDS_HPP__
#define __COORDS_HPP__

// Coordinate class
struct Coords
{  
    int x;
    int y;
    
    // Overloading comparison operators for coordinates
    friend bool operator==(const Coords &cd1, const Coords &cd2);
    friend bool operator!=(const Coords &cd1, const Coords &cd2);
};

#endif
