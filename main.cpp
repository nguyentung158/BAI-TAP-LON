#include<iostream>
#include<string.h>
#include<SDL.h>
#include<time.h>
#include<vector>
#include<SDL_image.h>
#include"Texture.h"
#include"dichuyenmain.h"
#include"kedich.h"
#include"mainbullet.h"
const int SCREEN_WIDTH = 1240;
const int SCREEN_HEIGHT =680;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
Texture background;
Texture mainShip;
Texture enemyShip;
Texture laser;
Dot mainship;

bool init();
bool loadMedia();
void close();
//bool kiemtravacham();
Texture::Texture(){
    mHeight =0; mWidth = 0;
    mTexture = NULL;
}
Texture::~Texture(){
 free();
}
bool Texture::loadFromFile(string path){
    free();
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    SDL_Texture* newtexture = NULL;
    if(loadedSurface == NULL){
        IMG_GetError();
    }
     else{
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format,0xff,0xff,0xff));

        newtexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(newtexture == NULL){
            SDL_GetError();
        }else{
            mHeight = loadedSurface->h;
            mWidth = loadedSurface->w;
        }
        SDL_FreeSurface(loadedSurface);
     }
     mTexture = newtexture;
    return mTexture!= NULL;
}
void Texture::free(){
    SDL_DestroyTexture(mTexture);
    mHeight =0;
    mWidth =0;
}
void Texture::render(int x, int y, SDL_Rect* clips, double angle, SDL_Point* center, SDL_RendererFlip flip){
    SDL_Rect renderQuad ={x,y,mWidth, mHeight};
    if(clips != NULL){
        renderQuad.w = clips->w;
        renderQuad.h = clips->h;
    }
    SDL_RenderCopyEx(gRenderer,mTexture,clips, &renderQuad, angle, center, flip);

}
int Texture::getHeight(){
return mHeight;
}
int Texture::getWidth(){
    return mWidth;
}

Dot::Dot(){
     mPosX = 0;
    mPosY =0;
    //khoi tao van toc
    mVelX = 0;
    mVelY =0;
    mainCollider.h =mainShip.getHeight();
    mainCollider.w =mainShip.getWidth();

}
void Dot::handleEvent(SDL_Event& e){
     //neu ta an 1 phim
    if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
        switch(e.key.keysym.sym){
            case SDLK_w:mVelY -= DOT_VEL; break;
            case SDLK_s: mVelY += DOT_VEL; break;
            case SDLK_d: mVelX += DOT_VEL; break;
            case SDLK_a: mVelX -= DOT_VEL;break;
        }
    } else if(e.type == SDL_KEYUP && e.key.repeat == 0){
        switch(e.key.keysym.sym){
            case SDLK_w:mVelY += DOT_VEL; break;
            case SDLK_s: mVelY -= DOT_VEL; break;
            case SDLK_d: mVelX -= DOT_VEL; break;
            case SDLK_a: mVelX += DOT_VEL;break;
    }
    }


}
void Dot::move(){
    mPosX += mVelX;
    	mainCollider.x = mPosX;

    //If the dot collided or went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + mainShip.getWidth() > SCREEN_WIDTH ) )
    {
        //Move back
        mPosX -= mVelX;
		mainCollider.x = mPosX;
    }

    //Move the dot up or down
    mPosY += mVelY;
	mainCollider.y = mPosY;

    //If the dot collided or went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + mainShip.getHeight() > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
		mainCollider.y = mPosY;
    }

}
void Dot::render(){
    mainShip.render(mPosX,mPosY,NULL);
}
int Dot::getPosX(){
    return mPosX;
}
int Dot::getPosY(){
    return mPosY;
}
enemy::enemy(){
    ePosX =0;
    ePosY =0;
    eRect.h =enemyShip.getHeight();
    eRect.w = enemyShip.getWidth();

}

void enemy::emove(){
    ePosX = SCREEN_WIDTH;

    eRect.x =ePosX;
    eRect.y =ePosY;

}
void enemy::render(int &i) {
    ePosX =i;
        ePosX -= eVel;
        eRect.x = ePosX;

    if(i <= 0){
        i =SCREEN_WIDTH;
        ePosX =i;
        eRect.x =ePosX;
        ePosY = rand() % (SCREEN_HEIGHT  - enemyShip.getHeight());
    }
    enemyShip.render(i,ePosY,NULL);
    i-= eVel;
}

bullet::bullet(){
    bPosX = 0;
    bPosY =0;
    bRect.w = laser.getWidth();
    bRect.h = laser.getHeight();

}

void bullet::handleBullet(SDL_Event& e){
    if(e.type == SDL_MOUSEBUTTONDOWN){

        bulletX.push_back( mainship.getPosX() +mainShip.getWidth());
        bulletY.push_back((mainship.getPosY() + mainShip.getHeight()/2));




    }



}
void bullet::movebullet(int &i){
    bulletX[i] += B_VEL;
    if(bulletX[i] + laser.getWidth()>=  SCREEN_WIDTH -10){
         bulletX.erase(bulletX.begin() );
         bulletY.erase(bulletY.begin() );

    }
}
void bullet::renderBullet(int &i) {
    laser.render(bulletX[i] ,bulletY[i],NULL);
}
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia(){
    bool success =true;
    if(!mainShip.loadFromFile("tutorial_sample64cc.png")){
        success =false;
    }
    if(!background.loadFromFile("1624.1.jpg")){
        success =false;
    }
    if(!enemyShip.loadFromFile("tutorial_sample64b.png")){
        success =false;
    }
    if(!laser.loadFromFile("laser.png")){
        success =false;
    }
    return success;

}

void close(){
    //Free loaded images
	mainShip.free();
    background.free();
    enemyShip.free();
    laser.free();
	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
int main(int argc, char* args[]){
    srand(time(0));
    init();
    loadMedia();
    SDL_Event e;
    bool quit =false;
    bullet lazer;
    enemy enemyship;
    int i = SCREEN_WIDTH;

    while(!quit){

        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                quit =true;
            }
            mainship.handleEvent(e);
           lazer.handleBullet(e);
        }
        SDL_SetRenderDrawColor(gRenderer,0xff,0xff,0xff,0xff);
        SDL_RenderClear(gRenderer);
        background.render(0,0,NULL);
        enemyship.emove();
        mainship.move();
        for(int j =0; j<lazer.bulletX.size();j++){
        lazer.movebullet(j);
        lazer.renderBullet(j);}
        mainship.render();
        enemyship.render(i);
        SDL_RenderPresent(gRenderer);

    }
    close();
}
