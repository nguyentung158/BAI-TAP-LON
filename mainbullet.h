#include<vector>
using namespace std;
class bullet{
private:
    int bPosX;
    int bPosY;
    SDL_Rect bRect;
public:
    static const int B_VEL = 15;
    bullet();
    void handleBullet(SDL_Event &e);
    void movebullet(int &i);
    void renderBullet(int &i);
    vector<int> bulletX;
    vector<int> bulletY;

};
