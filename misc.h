#ifndef MISC_H
#define MISC_H

#include <SFML/System.hpp>
#include <SFML/Network.hpp>


sf::Vector2i ipToDim(sf::IpAddress ip, int w);
sf::IpAddress dimToIp(sf::Vector2i vec, int w);
std::vector<std::string> explode(std::string const & s, char delim);
std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif // MISC_H
