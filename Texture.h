using namespace std;
class Texture{
    private:
        int mHeight;
        int mWidth;
        SDL_Texture* mTexture;
    public:
        Texture();
        ~Texture();
        bool loadFromFile(string path);
        void free();
        void render(int x,int y, SDL_Rect* clips, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
        int getHeight();
        int getWidth();

};
