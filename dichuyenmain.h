using namespace std;
class Dot{
private:
    int mPosX;
    int mPosY;
    //van toc dau cham
    int mVelX;
    int mVelY;
    SDL_Rect mainCollider;
public:
    static const int DOT_VEL = 10;
    static const int DOT_WIDTH = 180;
    static const int DOT_HEIGHT  = 80;
    Dot();
    void handleEvent(SDL_Event &e);
    void move();
    void render();
    int getPosX();
    int getPosY();

};
