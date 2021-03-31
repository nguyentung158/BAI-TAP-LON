using namespace std;
class enemy{
private:
    int ePosX;
    int ePosY;
    SDL_Rect eRect;
public:
    static const int eVel = 9;
    static const int E_WIDTH = 180;
    static const int E_HEIGHT =85;
    enemy();
    void emove();
    void render(int &i);

};
