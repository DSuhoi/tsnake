#ifndef __GAME_SESSION_HPP__
#define __GAME_SESSION_HPP__

#include <unordered_map>
#include "clients.hpp"
#include "snake.hpp"
#include "borders.hpp"
#include "fruits.hpp"


class Game_session
{
public:
    Game_session() = default;
    Game_session(std::list<Client_data>);
private:
    std::unordered_map<boost::uuids::uuid, Snake> _snakes;      // (temporarily)
    Fruits _fruits;
    Borders _borders;
};

#endif
