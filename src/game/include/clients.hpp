#ifndef __CLIENTS_HPP__
#define __CLIENTS_HPP__

#include <iostream>
#include <boost/uuid/uuid.hpp>

class Client_info
{
public:
    Client_info() = default;
private:
    boost::uuids::uuid _uid;
};

#endif
