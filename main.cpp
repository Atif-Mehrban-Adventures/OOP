#include <SFML/Graphics.hpp>  //Simple Fast Multimedia Library
#include <SFML/audio.hpp>
#include <time.h>
#include<iostream>
#include <list> //list is a container that supports constant
                //time insertion and removal of elements from
                //anywhere in the container. ... Adding, removing
                //and moving the elements within the list or
                //across several lists does not invalidate the iterators or references.
using namespace sf;

const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453;

class Animation
{
public:
	float Frame, speed;
	Sprite sprite;
    std::vector<IntRect> frames; // Here is a vector of frames which will compose an animation.

	Animation(){}

    Animation (Texture &t, int x, int y, int w, int h, int count, float Speed)
	{
	    Frame = 0;
        speed = Speed;

		for (int i=0;i<count;i++)
         frames.push_back( IntRect(x+i*w, y, w, h)  );

		sprite.setTexture(t);
		sprite.setOrigin(w/2,h/2);
        sprite.setTextureRect(frames[0]);
	}


	void update()
	{
    	Frame += speed;
		int n = frames.size();
		if (Frame >= n) Frame -= n;
		if (n>0) sprite.setTextureRect( frames[int(Frame)] );
	}

	bool isEnd()
	{
	  return Frame+speed>=frames.size();
	}

};


class Entity
{
public:
float x,y,dx,dy,R,angle;
bool life;
std::string name;
Animation anim;

Entity()
{
  life=1;
}

void settings(Animation &a,int X,int Y,float Angle=0,int radius=1)  // We defines settings here for entities:Overloaded constructor,
                                                                    // it contains the animations,coordinate, angle and radius for every entity
{
  anim = a;
  x=X; y=Y;
  angle = Angle;
  R = radius;
}

virtual void update(){};

void draw(RenderWindow &app)
{
  anim.sprite.setPosition(x,y);
  anim.sprite.setRotation(angle+90);
  app.draw(anim.sprite);

  CircleShape circle(R);
  circle.setFillColor(Color(255,0,0,170));
  circle.setPosition(x,y);
  circle.setOrigin(R,R);
  //app.draw(circle);
}

virtual ~Entity(){};
};


class asteroid: public Entity
{
public:
  asteroid()
  {
    dx=rand()%8-4;
    dy=rand()%8-4;
    name="asteroid";
  }

void  update()
  {
   x+=dx;
   y+=dy;

   if (x>W) x=0;  if (x<0) x=W;
   if (y>H) y=0;  if (y<0) y=H;
  }

};



class bullet: public Entity
{
public:
  bullet()
  {
    name="bullet";
  }

void  update()
  {
   dx=cos(angle*DEGTORAD)*8;
   dy=sin(angle*DEGTORAD)*8;
   //angle+=rand()%6-3;
   x+=dx;
   y+=dy;

   if (x>W || x<0 || y>H || y<0) life=0;
  }

};


class player: public Entity
{
public:
   bool thrust;

   player()
   {
     name="player";
   }

   void update()
   {
     if (thrust)
      { dx+=cos(angle*DEGTORAD)*0.2;
        dy+=sin(angle*DEGTORAD)*0.2; }
     else
      { dx*=0.99;       //creating effect of inertia
        dy*=0.99; }

   int maxSpeed=15;
    float speed = sqrt(dx*dx+dy*dy);            //limiting speed after reaching mix speed
    if (speed>maxSpeed)
     { dx *= maxSpeed/speed;
       dy *= maxSpeed/speed; }

    x+=dx;
    y+=dy;

    if (x>W) x=0; if (x<0) x=W;
    if (y>H) y=0; if (y<0) y=H;
   }

};


bool isCollide(Entity *a,Entity *b)
{
  return (b->x - a->x)*(b->x - a->x)+
   (b->y - a->y)*(b->y - a->y) <
   (a->R + b->R)*(a->R + b->R);
}
 // Code For Menu
int Menu()
{
    bool playgame=false, exitgame=false;

    RenderWindow window(VideoMode(500, 500), "Galaxy on Fire!");

    sf::Music music;
    if(!music.openFromFile("menu.wav"))
    std::cout<<"Can't find menu.wav"<<std::endl;
    music.setLoop(true);
    music.play();

    Texture menu , menuplay, menuexit;
    menu.loadFromFile("main.PNG");
    Sprite smenu(menu);

    menuplay.loadFromFile("play.PNG");
    Sprite smenuplay(menuplay);

    menuexit.loadFromFile("exit.PNG");
    Sprite smenuexit(menuexit);

    /*sf::SoundBuffer buffer2;
    sf::Sound sound2;
    if(!buffer2.loadFromFile("menu.wav"))  //! menu sound
        sound2.setBuffer(buffer2);
*/




    window.setFramerateLimit(15);

    while (window.isOpen())
    {
        Vector2i pos = Mouse::getPosition(window);
        // event processing
        Event event;
        while (window.pollEvent(event))
        // event instances are filled by the poll event function of the
        // window class .
        {
        	// request for closing the window
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed)
                if (event.key.code==Mouse::Left && playgame!=true && exitgame != true)


                // event.mouseButton.button
            {
                if (pos.x>300 && pos.y>160 && pos.x<450 &&pos.y<180.67)
                    {
                    	playgame = true;
                    	return 1;
					}
                else if (pos.x>300 && pos.y>300 && pos.x<450 &&pos.y<320)
                    {
                    	exitgame = true;
                    	return 2;
					}
            }
            if (event.type==Event::KeyPressed)
                if (event.key.code==Keyboard::Escape && playgame==true )
            {
                playgame=false;
            }

        }

        window.clear();

        if (playgame!=true && exitgame != true) {
            if (pos.x>300 && pos.y>160 && pos.x<450 &&pos.y<180.67)
                window.draw(smenuplay);
            else if (pos.x>300 && pos.y>300 && pos.x<450 &&pos.y<320)
                window.draw(smenuexit);
            else
                window.draw(smenu);
        }

        window.display();
    }

    return 0;
}

int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(W, H), "Asteroids!");
    app.setFramerateLimit(60);

    Texture t1,t2,t3,t4,t5,t6,t7;         // Texture is a built in SFML Class(It is used in our Program to Load Image files
    t1.loadFromFile("images/spaceship.png");
    t2.loadFromFile("images/background.jpg");
    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_blue.png");
    t6.loadFromFile("images/rock_small.png");
    t7.loadFromFile("images/explosions/type_B.png");

    t1.setSmooth(true); // setSmooth property allows one to smooth the texture.
    t2.setSmooth(true); // Smoothing a texture makes pixel boundaries less visible
                        // (but the image a little more blurry),
                        // which can be desirable if it is up-scaled.


    Sprite background(t2);

    Animation sExplosion(t3, 0,0,256,256, 48, 0.5);
    Animation sRock(t4, 0,0,64,64, 16, 0.2);
    Animation sRock_small(t6, 0,0,64,64, 16, 0.2);
    Animation sBullet(t5, 0,0,32,64, 16, 0.8);
    Animation sPlayer(t1, 40,0,40,40, 1, 0);
    Animation sPlayer_go(t1, 40,40,40,40, 1, 0);
    Animation sExplosion_ship(t7, 0,0,192,192, 64, 0.5);

    std::list<Entity*> entities;

    player *p;

    ///// score and live counters variables /////

    bool end_game = false;              // is it game over?
    int score_total = 0;                // current score count
    const int score_rock_big = 5;       // how many scores big rock costs
    const int score_rock_small = 10;    // how many scores small rock costs
    int lives = 3;                      // current live count

    sf::SoundBuffer buffer;
    sf::Sound sound;
    if(!buffer.loadFromFile("gun1.wav"))     //! gunshot sound
        std::cout<< "Cant find gun.wav"<<std::endl;
        sound.setBuffer(buffer);
        sound.setVolume(30);

    sf::SoundBuffer buffer1;
    sf::Sound sound1;
    if(!buffer1.loadFromFile("Explosion.wav"))  //! explosion sound
        std::cout<< "Cant find gun.wav"<<std::endl;
        sound1.setBuffer(buffer1);



    Font font;

    if ( !font.loadFromFile( "SourceCodePro-Regular.ttf" ) )
        return EXIT_FAILURE;

    const Color col_text_score( 255, 255, 255, 150 );
    const Color col_text_lives( 255, 255, 255, 150 );
    const Color col_text_game_over( 255, 255, 255, 180 );
    const Color col_text_final_score( 255, 255, 255, 180 );

    const int font_size_score = 40;
    const int font_size_lives = 40;
    const int font_size_game_over = 140;
    const int font_size_final_score = 110;

    Text text_score;
    Text text_lives;
    Text text_game_over;
    Text text_final_score;

    text_score.setString( "SCORE : 0" );
    text_score.setFont( font );
    text_score.setFillColor( col_text_score );
    text_score.setCharacterSize( font_size_score );
    text_score.setOrigin( Vector2f( 0.0, 0.0 ) );  //! Position of Score in Window
    text_score.setPosition( Vector2f( 20.0, 5.0 ) );

    text_lives.setString( "LIVES:" );
    text_lives.setFont( font );
    text_lives.setFillColor( col_text_lives );
    text_lives.setCharacterSize( font_size_lives );
    const FloatRect &bounds_lives = text_lives.getLocalBounds();
    text_lives.setOrigin( Vector2f( 0.0f, 0.0f ) );
    text_lives.setPosition( Vector2f( W - bounds_lives.width - 40.0f, 5.0f ) ); //!transformation of lives to the right

    text_game_over.setString( "GAME OVER!\n\n" );
    text_game_over.setFont( font );
    text_game_over.setFillColor( col_text_game_over );
    text_game_over.setCharacterSize( font_size_game_over );
    const FloatRect &bounds_go = text_game_over.getLocalBounds();
    text_game_over.setOrigin( bounds_go.width / 2.0f, bounds_go.height / 2.0f );
    text_game_over.setPosition( W / 2.0f, H / 2.0f );

    text_final_score.setFont( font );
    text_final_score.setFillColor( col_text_final_score );
    text_final_score.setCharacterSize( font_size_final_score );


    // Selection in menu
    int select;
    while(1)
    {

        select=Menu();

		if (select==0)
		break;
        else if (select==1)
        {




    //! clock and GoToGameOver introduces pause in GameOver screen to prevent immediate start of a new game
    //! without a chance to observe final score
    Clock clock;

    auto GoToGameOver = [&] () -> bool {//            sound2.play();
         return clock.getElapsedTime() > sf::seconds( 1 );    };

    auto SetFinalScore = [&] {
        text_final_score.setString( "SCORE:" + std::to_string( score_total ) );
        const FloatRect& bounds = text_final_score.getLocalBounds();
        text_final_score.setOrigin( bounds.width / 2.0f,bounds.height / 2.0f );
        text_final_score.setPosition( W / 2.0f, H / 2.0f );
    };

    auto UpdateScoreText = [&] {
        text_score.setString( "SCORE:" + std::to_string( score_total ) );   //!it is used to update and show final score at the end of game
    };

    auto UpdateLivesText = [&] {
        text_lives.setString( "LIVES:" + std::to_string( lives ) );         //!it is used for showing and updating lives
    };
    //!main game start from here
    //!here you can change initial values of score and life.
    // Set initial values upon starting new game
    auto StartNewGame = [&] {
        score_total = 0;
        lives = 4;            //! LIVES COUNT
        end_game = false;
        UpdateScoreText();
        UpdateLivesText();

        //! Remove existing entities
        //!As by default we have many random asteroids so first of all we have to ***clear the screen*** and ***release there memory***...
        for ( auto &e: entities )
        {
            delete e;
        }
        entities.clear();
        //!as we have removed many of the asteroids and not all so as the game start now we have create new asteroids now......
        //! Create new asteroids
       for(int i=0;i<15;i++)
        {
            asteroid *a = new asteroid();
            a->settings(sRock, rand()%W, rand()%H, rand()%360, 25);
            entities.push_back(a);
        }

        //! Create player and forward its pointer to p
        player *plr = new player();
        plr->settings(sPlayer,200,200,0,20);
        entities.push_back(plr);
        p = plr;
    };

    //!///////////////////////////////////////////////////////////////////
    //!Here by the game run with a call of Start new game function
    StartNewGame();

    //!//////////main loop/////
    while ( app.isOpen() )
    {
        Event event;     //! Here starts our SFML event handling class,
                         //! This library is used to control the events within the game, it is used to give user input in the game.
        while ( app.pollEvent( event ) )
        {
            if ( event.type == Event::Closed )
                app.close();

            if ( event.type == Event::KeyPressed )  /// controls
            {
                if ( end_game )                //! checks whether the game is over or not????
                {

                    if ( GoToGameOver() )      //! If the game is over go to game over window
                    {
                        if ( event.key.code == Keyboard::Escape )  //! if user presses escape key the
                            app.close();                           //! SFML window will be terminated
                        else
                            StartNewGame();
                    }
                   // if (event.key.code == Keyboard::Space)
                }
                else if ( event.key.code == Keyboard::Space )   //! When user presses Spacebar, the spaceship shoots the bullet
                {
                    bullet *b = new bullet();      //! Here we have called our bullet class that we created earlier
                    sound.play();     //! Play gunshot sound
                    b->settings( sBullet, p->x, p->y, p->angle, 10 );  //! setting the angle for bullet i.e. the shoots in the direction where the
                                                                       //! Head of our spaceship points in the X-Y Plane.
                    entities.push_back( b );           //! its a vector array index element,  it enables the bullet to move in XY direction.
                }
            }
        }
        //! this is used to draw window when the game is over
        if ( end_game )
        {
            //!////draw game over screen//////

            app.draw( background );
            app.draw( text_game_over );

            app.draw( text_final_score );
            app.display();
            continue;           //!it is used to hold the game over window
        }

        if ( Keyboard::isKeyPressed( Keyboard::Right ) ) p->angle += 3;
        if ( Keyboard::isKeyPressed( Keyboard::Left ) )  p->angle -= 3;
        if ( Keyboard::isKeyPressed( Keyboard::Up ) ) p->thrust = true;
        else p->thrust = false;

        //!when ever there is collision between asteroid and bullet the loop will be operated...
        for ( auto a : entities )       //!a points to asteroid and b points to bullet ...
        {
            for ( auto b : entities )
            {
                if ( a->name == "asteroid" && b->name == "bullet" )     //!string names are compared ..
                    if ( isCollide( a, b ) )
                    {
                        a->life = false;        //!to clear asteroid from memory after collision
                        b->life = false;       //!to clear bullet from memory after collision

                       Entity *e = new Entity();
                        e->settings( sExplosion, a->x, a->y );
                        e->name = "explosion";
                        sound1.play();
                        entities.push_back( e );

                        for ( int i = 0; i < 2; i++ )    //! This loop breaks the asteroid in to smaller two asteroids
                        {
                            if ( a->R == 15 ) continue;
                            Entity *e = new asteroid();
                            e->settings( sRock_small, a->x, a->y, rand() % 360, 15 );
                            entities.push_back( e );
                        }

                        score_total += (a->R == 15 ? score_rock_small : score_rock_big);
                        UpdateScoreText();
                    }

                if ( a->name == "player" && b->name == "asteroid" )
                    if ( isCollide( a, b ) )
                    {
                        b->life = false;

                        Entity *e = new Entity();
                        e->settings( sExplosion_ship, a->x, a->y );
                        e->name = "explosion";
                        sound1.play();
                        entities.push_back( e );

                        p->settings( sPlayer, W / 2, H / 2, 0, 20 ); //! 0 is angle and 20 radius
                        p->dx = 0; p->dy = 0;

                        if ( --lives <= 0 )
                        {

                            clock.restart();
                            end_game = true;
                            SetFinalScore();

                        }

                        UpdateLivesText();
                    }
            }
        }

        if ( p->thrust )  p->anim = sPlayer_go;  //! To run the animation for spaceship movement
        else   p->anim = sPlayer;                //! No animation for the spaceship as it is stationary

        for ( auto e : entities )
            if ( e->name == "explosion" )
                if ( e->anim.isEnd() ) e->life = 0;  //! to clear explosion from the memory

        if ( rand() %100== 0 )       //! Its the amount of rocks that generates at an instant
        {
            asteroid *a = new asteroid();
            a->settings( sRock, 0, rand() % H, rand() % 360, 25 );
            entities.push_back( a );
        }

        for ( auto i = entities.begin(); i != entities.end();)  //! Delete entity from the memory when the flow of that entity ends
                                                                //!It is for all the entities, without this our entities will pause
        {
            Entity *e = *i;

            e->update();
            e->anim.update();

            if ( e->life == false ) { i = entities.erase( i ); delete e; }
            else i++;
        }

        //////draw//////
        app.draw( background );        //! For Drawing our window, every entity and text in our window.
        for ( auto i : entities )
            i->draw( app );

        app.draw( text_score );
        app.draw( text_lives );
        app.display();
    }
        }

    return 0;
}
}
