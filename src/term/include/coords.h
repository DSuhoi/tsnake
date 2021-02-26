#ifndef __COORDS_H__
#define __COORDS_H__

// Coordinate class
class Coords {  
public:
    int x;
    int y;
    
    // Overloading comparison operators for coordinates
    friend bool operator==(Coords &cd1, Coords &cd2);
    friend bool operator!=(Coords &cd1, Coords &cd2);
};

#endif
