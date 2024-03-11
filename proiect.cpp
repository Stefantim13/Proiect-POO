#include <iostream>
#include <vector>


class Doodle{
    double jump_height;
    double x, y;
private:
    bool operator <=> (const Doodle&) const = default;
};

class Platform{
    const double length;
    int type; /// tip de platforma (rezista la infinit, se distruge dupa ce sari o data pe ea, te propulseaza mai mult in sus...)
    double x, y; 
private:
    bool operator <=> (const Platform&) const = default;
};

class Game{
    std::string name;
    int high_score;
    std::vector <Platform> platforms;
private:

    Game() = default;
    Game(std::string name_) : name(name_) {}
    Game(const Game& other) : name(other.name), high_score(other.high_score) {}
    Game(std::string name_, int high_score_) : name(name_), high_score(high_score_) {} 
    Game& operator=(const Game& other) {
        std::cout<< "op = Game\n";
        this->name = other.name;
        this->high_score = other.high_score;
        return *this;
    }
    ~Game(){
        std::cout << "destructor Game\n";
    }


    bool operator <=> (const Game&) const = default;
}; 
int main()
{
    
    return 0;
}