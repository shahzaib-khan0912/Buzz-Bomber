


/* Name : Muhammad Shahzaib Khan
   Section: CS-C
   Roll number: 24I-0741
*/


#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
using namespace sf;



const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;
const int gameColumns = resolutionY / boxPixelsY; 


int gameGrid[gameRows][gameColumns] = {0};

void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite);

void playerActions(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite, float& bullet_x, float& bullet_y,
		 	bool& bullet_exists, Clock& bulletClock, Sprite& bulletSprite, int array[][56],int cans, int bullets, int& currentCan,
		 	bool& changeCan, bool& gameOver, bool& visible, int& blinkCount, Clock& sprayCan, Text& loseText, int& remainingBullets, Sound& hitBullet);
		

void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);


void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite);


void moveSprite(Sprite& hummingBird, float& bird_x, float& bird_y, float& velocity_x, float& velocity_y, Clock& clock, RenderWindow& window, Clock& animationClock,
	int& currentFrame, int totalFrames, int width, int height, float honeyComb_x[], float honeyComb_y[], bool honeyComb[], int totalBees, int widthh, int heighth);


void manageBees(sf::RenderWindow& window, int resolutionX, int resolutionY, int boxPixelsY, sf::Sprite regularBees[], sf::IntRect frameRect,
		float bee_x[], float bee_y[], float beeSpeed[], bool beeDirection[], bool beeActive[], sf::Texture& regularBeeTexture,
		int totalBees, int* currentFrame, Clock beeClocks[], Sound& hitWall, RectangleShape groundRectangle, sf::Clock collision[], 
		int bullet_x, int bullet_y, bool& bullet_exists, Sprite honeyComb[], float& player_x, float& player_y, int& remainingBees,
		int& remainingBullets, float honeyComb_x[], float honeyComb_y[], bool aliveHoneyComb[], int & collidedBees, IntRect frameRect1,
		Sprite& flowers, int& groundReached, int& score, Text& scoreText, int &currentLevel, int beeSpeedRange);
		

void drawMenu(sf::RenderWindow& window, sf::Font& fonts, int& selectedIndex, Sprite& background, Music& bgMusic1);

void handleKillerBees(sf::RenderWindow& window,int remainingBees, int levelOneKbees, sf::Sprite killerBee[], float kBee_x[], float kBee_y[], float kBeeSpeed[],
		bool kBeeDirection[], bool kBeeActive[], sf::IntRect &frameRect, sf::Sound &hitWall, int resolutionX, int resolutionY,
		sf::RectangleShape &groundRectangle, int currentFrame[], int& remainingKbees, float& bullet_x, float& bullet_y, bool& bullet_exists,
		float& player_x, float& player_y, Sprite khoneyComb[], float khoneyComb_x[], float khoneyComb_y[], bool alivekHoneyComb[], int& score, Text& scoreText,
		int& remainingBullets, int& currentLevel, int& collidedkBees, Sprite bulletSprite);

void saveUserScore(int score, sf::RenderWindow& window, sf::Font& fonts);

void displayScoreboard(sf::RenderWindow& window, sf::Font& font);





int main()
{	

	int currentLevel = 1;
	
	

	bool gameOver = false; // a bool for basics logic that when to clear the screen.
	bool changeCan = false; // a bool for checking if a new can is needed or not
	
	// Setting the global font
	Font fonts;
	fonts.loadFromFile("buzz.ttf");
		
	//Text for when you are out of can and bullets as well
	Text loseText;
	loseText.setFont(fonts);
	loseText.setString("You are out of cans");
	sf::FloatRect textBounds = loseText.getLocalBounds();
	loseText.setOrigin(textBounds.left + textBounds.width / 2.0f, 
	textBounds.top + textBounds.height / 2.0f);
	loseText.setPosition(resolutionX / 2.0f, resolutionY / 2.0f);
	loseText.setFillColor(sf::Color::Red);
	
	
	srand(time(0));

	// Declaring RenderWindow.
	RenderWindow window(VideoMode(resolutionX, resolutionY), "Buzz Bombers by Shahzaib Khan", Style::Close | Style::Titlebar);

	window.setPosition(Vector2i(500, 200));

	// Initializing Background Music.
	Music bgMusic;
	bgMusic.openFromFile("Music/Title.ogg");
	bgMusic.play();
	bgMusic.setVolume(45);
	
	Music bgMusic1;
	bgMusic1.openFromFile("Music/Cave.ogg");
	bgMusic1.setVolume(45);
	
	Music bgMusic2;
	bgMusic2.openFromFile("Music/Forest.ogg");
	bgMusic2.setVolume(45);
	
	Music bgMusic3;
	bgMusic3.openFromFile("Music/lava.ogg");
	bgMusic3.setVolume(45);
	
	SoundBuffer hit;
	hit.loadFromFile("Effects/Obstacle.wav");
	Sound hitWall;
	hitWall.setBuffer(hit);
	hitWall.setVolume(50);
	
	SoundBuffer bullet;
	bullet.loadFromFile("Effects/fire.wav");
	Sound hitBullet;
	hitBullet.setBuffer(bullet);
	hitBullet.setVolume(50);
	

	
	// Initializing Player and Player Sprites.
	float player_x = (gameRows / 2) * boxPixelsX;
	float player_y = (gameColumns - 2) * boxPixelsY;
	
	const int bullets = 56;
	int remainingBullets = bullets;
	
	Texture playerTexture;
	Sprite playerSprite;
	playerTexture.loadFromFile("Textures/Spray/FullCan.png");
	playerSprite.setScale(48/32,48/32);
	playerSprite.setTexture(playerTexture);
	playerSprite.setOrigin(0, playerSprite.getGlobalBounds().height);
	playerSprite.setPosition(player_x, player_y);
	
	float bullet_x = player_x;
	float bullet_y = player_y;
	bool bullet_exists = false;

	Clock bulletClock;
	Texture bulletTexture;
	Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setScale(1.0f, 1.0f);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	// The ground on which player moves
	
	RectangleShape groundRectangle(Vector2f(960, 64));
	Texture groundRectangleTexture;
	if (!groundRectangleTexture.loadFromFile("Textures/grass.png")) {
    		std::cerr << "Error loading texture" << std::endl;
	}

	// Setting the texture to the rectangle
	groundRectangle.setTexture(&groundRectangleTexture);
	groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
	
	// Setting up can2
	Texture can2Texture;
	can2Texture.loadFromFile("Textures/Spray/ExtraCans.png");
	Sprite can2;
	float can2Y = resolutionY - groundRectangle.getSize().y - can2.getGlobalBounds().height;   
												  
												   

	if (can2Y < 0) {
		can2Y = 0; 
	}


	can2.setTexture(can2Texture);
	can2.setPosition(0, can2Y); 

	// Setting up can3
	Texture can3Texture;
	can3Texture.loadFromFile("Textures/Spray/ExtraCans.png");
	Sprite can3;
	can3.setTexture(can3Texture);
	can3.setPosition(32, can2Y); 


	
	const int cans = 3;
	
	
	
	
	//A 2d array that stores the current can and the number of bullets remaining	
	int array[cans][bullets] = {0};
		
		
	// Making all the bullets in 3 cans available to shoot:
		
	for(int i = 0; i < cans; i++){
		for(int j = 0; j < bullets; j++){
			array[i][j] = 1;
		}
	}
		
	int currentCan = 0;
	Clock sprayCan;
	bool visible = true;
	//float flashingTime = 200;
	static int blinkCount = 0;
	
	
////////////////////////////////////// For Bees ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int score = 0;
	
	sf::Text scoreText;
	scoreText.setFont(fonts);
	scoreText.setCharacterSize(15);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setString("Score: 0");
	scoreText.setPosition(800, 600); 
	
	
	int beeSpeedRange1 = 3;
	int beeSpeedRange2 = 5;
  	int beeSpeedRange3 = 7;
  	
  	
  	int currentLevelBees[] = {20,15,20};
  	int totalBees = 20;
  	
  	
	int remainingBees = totalBees;
	
	
	sf::Texture regularBeeTexture;
	regularBeeTexture.loadFromFile("Textures/normalBee.png");
        

	Sprite regularBees[remainingBees];
	sf::IntRect frameRect(0, 0, 48, 48);
	float bee_x[remainingBees] = {0};
	float bee_y[remainingBees] = {0};
	float beeSpeed[remainingBees] = {0};
	bool beeDirection[remainingBees] = {false};
	bool beeActive[remainingBees] = {false};
	int currentFrame[remainingBees] = {0};
	
	int collidedBees = 0;
	int collidedkBees = 0;
	for (int i = 0; i < totalBees; i++) {
		regularBees[i].setTexture(regularBeeTexture);
		regularBees[i].setTextureRect(frameRect);
	}
	
	sf::Clock beeClocks[remainingBees];
	sf::Clock collision[remainingBees];
	
	int groundReached = 0;
	int levelTwoKbees = 10;
	int levelOneKbees = 5;
	
	int remainingkBees = levelOneKbees;
	
	Texture kBeeTexture;
	kBeeTexture.loadFromFile("Textures/killerbee.png");
	Sprite killerBee[levelOneKbees];
	
	float kBee_x[remainingkBees] = {0};
	float kBee_y[remainingkBees] = {0};
	float kBeeSpeed[remainingkBees] = {0};
	bool kBeeDirection[remainingkBees] = {false};
	bool kBeeActive[remainingkBees] = {false};
	
	for(int i = 0; i < 2; i++){
	
		killerBee[i].setTexture(kBeeTexture);
		killerBee[i].setTextureRect(sf::IntRect(0, 0, 48, 48));
	}
	
	
	
	
///////////////////////////// honey comb logic ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	Texture honeyCombTexture;
	
	honeyCombTexture.loadFromFile("Textures/honeycomb.png");
	const int widthh = honeyCombTexture.getSize().x;
	const int heighth = honeyCombTexture.getSize().y;
	Sprite honeyComb[totalBees];
	IntRect frameRect1(0, 0, widthh, heighth);
	for(int i = 0; i< totalBees; i++){
		
		honeyComb[i].setTexture(honeyCombTexture);
		honeyComb[i].setTextureRect(frameRect1);
	}
		
	float honeyComb_x[remainingBees] = {0};
	float honeyComb_y[remainingBees] = {0};
	
	bool aliveHoneyComb[remainingBees] = {false};
	
	Texture khoneyCombTexture;
	
	khoneyCombTexture.loadFromFile("Textures/honeycomb_red.png");
	Sprite khoneyComb[levelOneKbees];
	
	for(int i = 0; i< levelOneKbees; i++){
		
		khoneyComb[i].setTexture(khoneyCombTexture);
		khoneyComb[i].setTextureRect(frameRect1);
	}
	
	
		
	float khoneyComb_x[remainingkBees] = {0};
	float khoneyComb_y[remainingkBees] = {0};
	
	bool alivekHoneyComb[remainingkBees] = {false};
	
	Texture beeHiveTexture;
	Sprite beeHive;
	beeHiveTexture.loadFromFile("Textures/hive.png");
	beeHive.setTexture(beeHiveTexture);
		
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////// Sprite for the flower //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	Sprite flowers;
	Texture flowerTexture;
	
	flowerTexture.loadFromFile("Textures/flowers.png");
	
	flowers.setTexture(flowerTexture);
	flowers.setScale(1.0f,1.0f);
	
/////////////////////// background pics ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	Sprite background;
	Texture bgTexture;
	bgTexture.loadFromFile("Textures/background1.png");
	background.setTexture(bgTexture);
	background.setScale(2.0f,2.0f);
	
	Sprite background2;
	Texture bgTexture2;
	bgTexture2.loadFromFile("Textures/cave.png");
	background2.setTexture(bgTexture2);
	background2.setScale(2.0f,2.0f);
	
	Sprite background3;
	Texture bgTexture3;
	bgTexture3.loadFromFile("Textures/forest.png");
	background3.setTexture(bgTexture3);
	background3.setScale(2.0f,2.0f);
	
	Sprite background4;
	Texture bgTexture4;
	bgTexture4.loadFromFile("Textures/sky.png");
	background4.setTexture(bgTexture4);
	background4.setScale(2.0f,2.0f);
	
////// HummingBird ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	Sprite hummyingBird;
	Texture birdTexture;
	birdTexture.loadFromFile("Textures/bird.png");
	hummyingBird.setTexture(birdTexture);
	const int width = birdTexture.getSize().x / 2;
	const int height = birdTexture.getSize().y;
	hummyingBird.setTextureRect(sf::IntRect(0, 0, width, height));
	hummyingBird.setScale(2.0f, 2.0f);
	
	int HtotalFrames = 2;
	int HcurrentFrame = 0;
	float bird_x = 200, bird_y = 200;
	const int sprite_width = 64;
	const int sprite_height = 64;
	hummyingBird.setPosition(bird_x, bird_y); 
	float velocity_x = 0.2f;
	float velocity_y = 0.2f;
	sf::Clock clock, animationClock;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	int selectedIndex = 0; 
	drawMenu(window, fonts, selectedIndex, background, bgMusic1);
	//bgMusic1.play();
	 if (selectedIndex == 0) { 
	 ;
	
	  while (window.isOpen()) {

		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				return 0;
			}
			if(e.type == Event::KeyPressed){
				if(e.key.code == Keyboard::Escape){
					drawMenu(window, fonts, selectedIndex, background, bgMusic1);
					
					if (selectedIndex == 1) { 
						
		
		 				displayScoreboard(window, fonts);
		 				window.display();
						 window.clear();
		  			}
					else if (selectedIndex == 2) {
						
						window.close();
					}
				}
			}
		}
				
	if(currentLevel <=3){	
	
		if(currentLevel == 1 ){
			window.draw(background2);
			}
		else if(currentLevel == 2){
			window.draw(background3);
			}	
		else if(currentLevel == 3){
			window.draw(background4);
			}
		if(gameOver){
			window.clear();
			window.draw(loseText);
			window.display();
			sleep(sf::seconds(2));
			window.clear();
			displayScoreboard(window, fonts);
			window.display();
			window.clear();
		}
		if(remainingBullets > 48){
			playerTexture.loadFromFile("Textures/Spray/FullCan.png");
		}
		else if (remainingBullets > 40 ){
			playerTexture.loadFromFile("Textures/Spray/SecondDown.png");
		}
		else if (remainingBullets > 32){
			playerTexture.loadFromFile("Textures/Spray/ThirdDown.png");
		}
		else if (remainingBullets > 24){
			playerTexture.loadFromFile("Textures/Spray/FourthDown.png");
		}
		else if (remainingBullets > 16){
			playerTexture.loadFromFile("Textures/Spray/FifthDown.png");
		}
		else if (remainingBullets > 8){
			playerTexture.loadFromFile("Textures/Spray/SixthDown.png");
		}
		else {
			playerTexture.loadFromFile("Textures/Spray/EmptyCan.png");
		}
		playerSprite.setTexture(playerTexture);
		
		

			 
		playerActions(window, player_x, player_y, playerSprite, bullet_x, bullet_y, bullet_exists, bulletClock, 
			bulletSprite, array, cans, bullets, currentCan, changeCan, gameOver, visible, blinkCount, sprayCan, loseText, remainingBullets, hitBullet);
		
		// Calling and moving the bullet.
		
		if(bullet_exists){
			moveBullet(bullet_y,bullet_exists, bulletClock);
			drawBullet(window, bullet_x, bullet_y, bulletSprite);
			}

		
		
		window.draw(groundRectangle);
		
		
		if(currentCan == 0){
			window.draw(can2);
			window.draw(can3);
			}
		else if(currentCan == 0 || currentCan == 1){
			window.draw(can2);
			}
			
		window.draw(scoreText);
		
	}
	
	
	
		switch(currentLevel){
			case 1: 
				//bgMusic1.play();
				
			moveSprite(hummyingBird, bird_x, bird_y, velocity_x, velocity_y, clock, window, animationClock, HcurrentFrame, HtotalFrames, width, height,
				honeyComb_x, honeyComb_y, aliveHoneyComb, totalBees, widthh, heighth);

			manageBees(window, resolutionX, resolutionY, boxPixelsY, regularBees, frameRect, bee_x, bee_y, beeSpeed, beeDirection,
			   beeActive, regularBeeTexture, totalBees, currentFrame, beeClocks, hitWall, groundRectangle, collision, bullet_x, bullet_y,
			    bullet_exists, honeyComb, player_x, player_y, remainingBees, remainingBullets,  honeyComb_x,  honeyComb_y, aliveHoneyComb,
			    collidedBees, frameRect1, flowers, groundReached, score, scoreText, currentLevel, beeSpeedRange1);
			    
			    for(int i = 0; i < totalBees; i++){
		if(!beeActive[i]) continue;
		
		if(bee_y[i] > resolutionY - groundRectangle.getSize().y){
			flowers.setPosition(bee_x[i], bee_y[i]);
			window.draw(flowers);
			}
		}
			    
			
			    
			
			break;
		case 2:
			//bgMusic2.play();
			
			moveSprite(hummyingBird, bird_x, bird_y, velocity_x, velocity_y, clock, window, animationClock, HcurrentFrame, HtotalFrames, width, height,
				honeyComb_x, honeyComb_y, aliveHoneyComb, totalBees, widthh, heighth);
		 	
			manageBees(window, resolutionX, resolutionY, boxPixelsY, regularBees, frameRect, bee_x, bee_y, beeSpeed, beeDirection,
			   beeActive, regularBeeTexture, totalBees, currentFrame, beeClocks, hitWall, groundRectangle, collision, bullet_x, bullet_y,
			    bullet_exists, honeyComb, player_x, player_y, remainingBees, remainingBullets,  honeyComb_x,  honeyComb_y, aliveHoneyComb,
			    collidedBees, frameRect1, flowers, groundReached, score, scoreText, currentLevel, beeSpeedRange2);
			    
			
			    handleKillerBees(window, remainingBees, levelOneKbees, killerBee, kBee_x, kBee_y, kBeeSpeed, kBeeDirection, kBeeActive, frameRect, hitWall,
				 resolutionX, resolutionY, groundRectangle, currentFrame, remainingkBees, bullet_x, bullet_y, bullet_exists, player_x, player_y,
				 khoneyComb,  khoneyComb_x, khoneyComb_y, alivekHoneyComb,  score, scoreText, remainingBullets, currentLevel, collidedkBees, bulletSprite); 
			    
			 
			    break;
		
		case 3:
			//bgMusic3.play();
			
			manageBees(window, resolutionX, resolutionY, boxPixelsY, regularBees, frameRect, bee_x, bee_y, beeSpeed, beeDirection,
			   beeActive, regularBeeTexture, totalBees, currentFrame, beeClocks, hitWall, groundRectangle, collision, bullet_x, bullet_y,
			    bullet_exists, honeyComb, player_x, player_y, remainingBees, remainingBullets,  honeyComb_x,  honeyComb_y, aliveHoneyComb,
			    collidedBees, frameRect1, flowers, groundReached, score, scoreText, currentLevel, beeSpeedRange3);
			    
			
			   handleKillerBees(window, remainingBees, levelOneKbees, killerBee, kBee_x, kBee_y, kBeeSpeed, kBeeDirection, kBeeActive, frameRect, hitWall,
				 resolutionX, resolutionY, groundRectangle, currentFrame, remainingkBees, bullet_x, bullet_y, bullet_exists, player_x, player_y,
				 khoneyComb,  khoneyComb_x, khoneyComb_y, alivekHoneyComb,  score, scoreText, remainingBullets, currentLevel, collidedkBees, bulletSprite); 
			       
			    break;
		}
		
		if (remainingBees == 0) {
			currentLevel++;
			if (currentLevel > 3) {
			 // Game completed
			 window.clear();
			 
			saveUserScore(score, window, fonts);
			displayScoreboard(window, fonts);
			
 			window.display();
			
			
		}

		
		remainingBees = totalBees;
		collidedBees = 0;
		remainingBullets = 56;
		//currentCan = 0;
        
		
        
		for(int i = 0; i < totalBees; i++){
			alivekHoneyComb[i] = false;
			honeyComb_x[i] = -100;
			honeyComb_y[i] = -100;
		}
		for(int i = 0; i< totalBees; i++){
	 		beeActive[i] = false;
	 		bee_x[i] = -100;
	 		bee_y[i] = -100;
		}
	 	
        
        
        	// Displaying level transition message
       		Text levelUpText;
       		levelUpText.setFont(fonts);
       		if(currentLevel <= 3)
       		levelUpText.setString("Level " + std::to_string(currentLevel));
       		levelUpText.setCharacterSize(50);
       		levelUpText.setFillColor(sf::Color::Yellow);
       		levelUpText.setPosition(resolutionX / 2 - 100, resolutionY / 2 - 50);
       		window.clear();
       		window.draw(levelUpText);
        	window.display();
        	sleep(sf::seconds(2)); 
    		}
		
		window.display();
		
		window.clear();
		
		}
	}
	
	else if (selectedIndex == 1) { 
		
		
		 displayScoreboard(window, fonts);
		 window.display();
		 window.clear();
		  }
	else if (selectedIndex == 2) {
	
	window.close();
	}
		
		
}	

void drawPlayer(RenderWindow& window, float& player_x,float& player_y , Sprite& playerSprite) {
	window.draw(playerSprite);
}

void playerActions(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite, 
		float& bullet_x, float& bullet_y, bool& bullet_exists, Clock& bulletClock, Sprite& bulletSprite,
		int array[][56],int cans, int bullets, int& currentCan, bool& changeCan, bool& gameOver, bool& visible,
		int& blinkCount, Clock& sprayCan, Text& loseText, int& remainingBullets, Sound& hitBullet)
	{
		
		float flashingTime = 200;
		const float speed = 0.27f;
		
///////////////////////////////////////// movement of the can /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			if (playerSprite.getPosition().x > 0) // Restriction of movement to left boundary
				playerSprite.move(-speed, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			if (playerSprite.getPosition().x + playerSprite.getGlobalBounds().width < resolutionX) // Restriction of movement to right boundary
			playerSprite.move(speed, 0);
		}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////// Bullet firing logic /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		if(Keyboard::isKeyPressed(Keyboard::Space) && !bullet_exists){
			hitBullet.play();
			for(int j = 0; j < bullets; j++){
				if(array[currentCan][j] == 1){
					array[currentCan][j] = 0;
					bullet_x = playerSprite.getPosition().x + playerSprite.getGlobalBounds().width / 2 - 16; 
					bullet_y = playerSprite.getPosition().y - 4 * 32 ; // Starting bullet from player's position
					bullet_exists = true;
					bulletClock.restart(); 
					
						remainingBullets--;
					
					
					return;
				}
			}
		
		currentCan++;
		changeCan = true;
		remainingBullets = 56;
		if(currentCan >= cans){
			gameOver = true;
			}
		}
		

		
		if(sprayCan.getElapsedTime().asMilliseconds() > flashingTime && changeCan){
			
			sprayCan.restart();
			visible = !visible;
			blinkCount++;
		}
		if(blinkCount >= 10){
			visible = true;
			changeCan = false;
			blinkCount = 0;
		}
		
		if(visible){
			window.draw(playerSprite);
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
/////////////////////////////// Function for moving of the bullet /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock) {
	if (bulletClock.getElapsedTime().asMilliseconds() < 20)
		return;

	bulletClock.restart();
	bullet_y -= 18;
	if (bullet_y < 0)
		bullet_exists = false;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawBullet(sf::RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite) {
	bulletSprite.setPosition(bullet_x, bullet_y + 32);
	window.draw(bulletSprite);
}


////// Function for all the managent of bees and honey Comb ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void manageBees(sf::RenderWindow& window, int resolutionX, int resolutionY, int boxPixelsY, sf::Sprite regularBees[], sf::IntRect frameRect,
 		float bee_x[], float bee_y[], float beeSpeed[], bool beeDirection[], bool beeActive[], sf::Texture& regularBeeTexture,
 		int totalBees, int* currentFrame, Clock beeClocks[], Sound& hitWall, RectangleShape groundRectangle, sf::Clock collision[],
 		int bullet_x , int bullet_y, bool& bullet_exists, Sprite honeyComb[], float& player_x, float& player_y, int& remainingBees,
 		int& remainingBullets, float honeyComb_x[], float honeyComb_y[], bool aliveHoneyComb[], int& collidedBees, IntRect frameRect1,
 		Sprite& flowers, int& groundReached, int& score, Text& scoreText, int& currentLevel, int beeSpeedRange){

///////////// Common variable and constants needed ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	static sf::Clock spawnClock, moveClock, animationClock;
	float spawnDelay = 2.0f;
	float moveDelay = 0.02f;
	float animationDelay = 0.2f;
	const int totalFrames = 4;
	const float collisionCoolDown = 0.5f;
	const float pushBack = 5.0f;
	
//////////// Animation of the bees ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	for(int i = collidedBees; i < totalBees; i++){
		if (animationClock.getElapsedTime().asSeconds() > animationDelay) {
			currentFrame[i] = (currentFrame[i] + 1) % totalFrames; 
			frameRect.left = currentFrame[i] * 48;      
			regularBees[i].setTextureRect(frameRect);
			beeClocks[i].restart(); 
        		}
        }
		
///////////////////// Generating new bees /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	if (spawnClock.getElapsedTime().asSeconds() > spawnDelay) {
		for (int i = collidedBees; i < totalBees; i++) {
		if (!beeActive[i]) {
			beeDirection[i] = rand() % 2;
			if(beeDirection[i]){
				regularBees[i].setScale(-1.0f , 1.0f);
				bee_x[i] =  0;
				
			}
			else{
				regularBees[i].setScale(1.0f, 1.0f);
				bee_x[i] = resolutionX - frameRect.width;
				
				} 
			
			bee_y[i] = 0;
			beeSpeed[i] = rand() % beeSpeedRange + 2;
			beeActive[i] = true;
			spawnClock.restart();
			break;
			}
		}
	}

////////// Moving the active bees /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	if (moveClock.getElapsedTime().asSeconds() > moveDelay) {
		for (int i = collidedBees; i < totalBees; i++) {
			if (!beeActive[i]) continue;
			bee_x[i] += beeDirection[i] ? beeSpeed[i] : -beeSpeed[i];
           	 // Bounce off walls
			if (bee_x[i] < 0 || bee_x[i] > resolutionX - frameRect.width) {
				beeDirection[i] = !beeDirection[i];
				bee_y[i] += 48;
				regularBees[i].setScale(beeDirection[i] ? -1.0f : 1.0f, 1.0f);
				
				hitWall.play();
				bee_x[i] += beeDirection[i] ? pushBack : -pushBack;
			}
		
	
		
	
			

//////////////////////// Checking Bees that are reaching ground ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
	
			if (bee_y[i] > resolutionY - groundRectangle.getSize().y - regularBees[i].getGlobalBounds().height) {
				beeActive[i] = false;
				
				
					flowers.setPosition(450 , resolutionY - groundRectangle.getSize().y);
					window.draw(flowers);
					
				
				remainingBees--;
				continue;
				}

			regularBees[i].setPosition(bee_x[i], bee_y[i]);
			}
			
			
					
		moveClock.restart();
	}
	


	
/////////////// Collision Dection of two bees /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	for(int i = 0; i < remainingBees; i++){
		if(!beeActive[i]) continue;
		
		for(int j = i + 1; j < remainingBees; j++){
			if(!beeActive[i]) continue;
			
			float leftA = bee_x[i];
			float rightA = bee_x[i] + frameRect.width;
			float topA = bee_y[i];
			float bottomA = bee_y[i] +  frameRect.height;
			
			float leftB = bee_x[j];
			float rightB = bee_x[j] + frameRect.width;
			float topB = bee_y[j];
			float bottomB = bee_y[j] + frameRect.height;
			
			if(leftA < rightB && rightA > leftB && topA < bottomB && bottomA > topB){
				if(collision[i].getElapsedTime().asSeconds() > collisionCoolDown && collision[j].getElapsedTime().asSeconds() > collisionCoolDown){
					beeDirection[i] = !beeDirection[i];
					beeDirection[j] = !beeDirection[j];
				
					regularBees[i].setScale(beeDirection[i] ? -1.0f : 1.0f, 1.0f); 
					regularBees[j].setScale(beeDirection[j] ? 1.0f : -1.0f, 1.0f);
					
					bee_x[i] += beeDirection[i] ? pushBack : -pushBack; 
					bee_x[j] += beeDirection[j] ? pushBack : -pushBack;
				
					collision[i].restart();
					collision[j].restart();
				}
				
				
			}
		}
	}
	
////////////////// Collision of bees with bullet //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	if(bullet_exists){
		for(int i = collidedBees; i < totalBees; i++){
			if(!beeActive[i]) continue;
		
			float leftA = bee_x[i];
			float rightA = bee_x[i] + frameRect.width;
			float topA = bee_y[i];
			float bottomA = bee_y[i] +  frameRect.height;
		
			float leftB = bullet_x;
			float rightB = bullet_x + frameRect.width;
			float topB = bullet_y;
			float bottomB = bullet_y + frameRect.height;
			
			if(leftA < rightB && rightA > leftB && topA < bottomB && bottomA > topB){
				
				
				score += 100;
				scoreText.setString("Score: " + std::to_string(score));
				
				beeActive[i] = false;
				bullet_exists = false;
				
				remainingBees--;
				remainingBullets--;
				collidedBees++;
				
				honeyComb_x[i] = bee_x[i];
				honeyComb_y[i] = bee_y[i];
				aliveHoneyComb[i] = true;
				
				
				bullet_x = player_x;
				bullet_y = player_y;
			}
		}
			
}

/////////////// Drawing bees //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	for (int i = collidedBees; i < totalBees; i++) {
		if (beeActive[i]) {
 			window.draw(regularBees[i]);
		}
	}
	
////////// Drawing HoneyCombs /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	for(int i = 0; i < totalBees; i++ ){
		if(aliveHoneyComb[i]){
			honeyComb[i].setPosition(honeyComb_x[i], honeyComb_y[i]);
			honeyComb[i].setScale(1.0f,1.0f);
			window.draw(honeyComb[i]);
			}
	}
	
//////////////////Collision of bees and HoneyCombs ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	for(int i = 0; i < totalBees; i++){
		
		if(!beeActive[i]) continue;
		

			float leftA = bee_x[i];
			float rightA = bee_x[i] + frameRect.width;
			float topA = bee_y[i];
			float bottomA = bee_y[i] +  frameRect.height;
			
			for(int j = 0 ; j < totalBees; j++ ){
				if (!aliveHoneyComb[j]) continue; 
				float leftB = honeyComb_x[j];
				float rightB = honeyComb_x[j] + honeyComb[j].getGlobalBounds().width;;
				float topB = honeyComb_y[j];
				float bottomB = honeyComb_y[j] + honeyComb[j].getGlobalBounds().height;
			
				if (leftA < rightB && rightA > leftB && topA < bottomB && bottomA > topB) {
					
					beeDirection[i] = !beeDirection[i];
					bee_y[i] += 48;
					regularBees[i].setScale(beeDirection[i] ? -1.0f : 1.0f, 1.0f);
				
					hitWall.play();
					bee_x[i] += beeDirection[i] ? pushBack : -pushBack;
					
				}
			}
	}
	
///////////// Colllsion of bullets with honeyComb /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	if(bullet_exists){
		for(int i = 0; i < totalBees; i++){
			if (!aliveHoneyComb[i]) continue; 
			
			float leftA = bullet_x;
			float rightA = bullet_x + frameRect.width;
			float topA = bullet_y;
			float bottomA = bullet_y + frameRect.height;
			
			float leftB = honeyComb_x[i];
			float rightB = honeyComb_x[i] + honeyComb[i].getGlobalBounds().width;
			float topB = honeyComb_y[i];
			float bottomB = honeyComb_y[i] + honeyComb[i].getGlobalBounds().height;
			
			if (leftA < rightB && rightA > leftB && topA < bottomB && bottomA > topB){
			
				//remainingBullets--;
				aliveHoneyComb[i] = false;
				bullet_exists = false;
			}
		}
	}
	
	
	
		    		
			
		
}	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////// Menu ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	


void drawMenu(sf::RenderWindow& window, sf::Font& fonts, int& selectedIndex, Sprite& background, Music& bgMusic1) {
   
	Text gameName("Buzz Bombers", fonts, 50);
	gameName.setPosition(480 - gameName.getLocalBounds().width / 2, 100);

	Text startText("Start Game", fonts, 25);
 	Text scoreBoard("ScoreBoard", fonts, 25);
	Text quitText("Quit Game", fonts, 25);

	startText.setPosition(480 - startText.getLocalBounds().width / 2, 200);
 	scoreBoard.setPosition(480 - scoreBoard.getLocalBounds().width / 2, 300);
	quitText.setPosition(480 - quitText.getLocalBounds().width / 2, 400);
	
	gameName.setFillColor(sf::Color::Yellow);

	startText.setFillColor(sf::Color::Cyan);
	scoreBoard.setFillColor(sf::Color::Green);
	quitText.setFillColor(sf::Color::Magenta); 

	Color normalColor = sf::Color::Blue;
	Color selectedColor = sf::Color::Red;

	float keyRepeatTime = 0.2f;  
	Clock keyClock;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
			window.close();
				}
			}

////////////////// MOving up and down the menu keyboard input 
	if (keyClock.getElapsedTime().asSeconds() > keyRepeatTime) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			selectedIndex = (selectedIndex - 1 + 3) % 3;  
			keyClock.restart();
		} 
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			selectedIndex = (selectedIndex + 1) % 3;  
			keyClock.restart();
		} 
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                	return;  
            	}
        }

        
	startText.setFillColor(selectedIndex == 0 ? selectedColor : normalColor);
	scoreBoard.setFillColor(selectedIndex == 1 ? selectedColor : normalColor);
	quitText.setFillColor(selectedIndex == 2 ? selectedColor : normalColor);

		
	window.clear();
	window.draw(background);
	window.draw(gameName);
	window.draw(startText);
	window.draw(scoreBoard);
	window.draw(quitText);
	window.display();
	
	}
}
///////////////// killer bees /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void handleKillerBees(sf::RenderWindow& window, int remainingKBees, int levelOneKbees, sf::Sprite killerBee[], float kBee_x[], float kBee_y[], float kBeeSpeed[],
		     bool kBeeDirection[],bool kBeeActive[], sf::IntRect &frameRect, sf::Sound &hitWall, int resolutionX, int resolutionY,
		     sf::RectangleShape &groundRectangle, int currentFrame[], int& remainingKbees, float& bullet_x, float& bullet_y, bool& bullet_exists,
		     float& player_x, float& player_y, Sprite khoneyComb[], float khoneyComb_x[], float khoneyComb_y[], bool alivekHoneyComb[], int& score, Text& scoreText
		     , int& remainingBullets, int& currentLevel, int& collidedkBees, Sprite bulletSprite) {
    
	static sf::Clock spawnClock, moveClock, animationClock;

    
	const float spawnDelay = 2.0f;
	const float moveDelay = 0.02f;
	const float animationDelay = 0.2f;
	const int totalFrames = 4;
	const float collisionCoolDown = 0.5f;
	const float pushBack = 5.0f;

    // Animation of the killer bees
	 
	for (int i = 0; i < levelOneKbees; i++) {
		if (animationClock.getElapsedTime().asSeconds() > animationDelay) {
			currentFrame[i] = (currentFrame[i] + 1) % totalFrames;
			frameRect.left = currentFrame[i] * 48;
			killerBee[i].setTextureRect(frameRect);
		}
	}

///////////// Generating new killer bees //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (spawnClock.getElapsedTime().asSeconds() > spawnDelay) {
        for (int i = 0; i < levelOneKbees; i++) {
            if (!kBeeActive[i]) {
                kBeeDirection[i] = rand() % 2;
                if (kBeeDirection[i]) {
                    killerBee[i].setScale(-1.0f, 1.0f);
                    kBee_x[i] = 0;
                } else {
                    killerBee[i].setScale(1.0f, 1.0f);
                    kBee_x[i] = resolutionX - frameRect.width;
                }

                kBee_y[i] = 0;
                kBeeSpeed[i] = rand() % 3 + 6; 
                kBeeActive[i] = true;
                spawnClock.restart();
                break;
            }
        }
    }

    
	if (moveClock.getElapsedTime().asSeconds() > moveDelay) {
		for (int i = 0; i < levelOneKbees; i++) {
		if (!kBeeActive[i]) continue;

			kBee_x[i] += kBeeDirection[i] ? kBeeSpeed[i] : -kBeeSpeed[i];

            // Bouncing off walls
			if (kBee_x[i] < 0 || kBee_x[i] > resolutionX - frameRect.width) {
				kBeeDirection[i] = !kBeeDirection[i];
				kBee_y[i] += 48;
				killerBee[i].setScale(kBeeDirection[i] ? -1.0f : 1.0f, 1.0f);
				hitWall.play();
				kBee_x[i] += kBeeDirection[i] ? pushBack : -pushBack;
			}

            // Checking if bees reach the ground
			if (kBee_y[i] > resolutionY - groundRectangle.getSize().y - killerBee[i].getGlobalBounds().height) {
				kBeeActive[i] = false;
				remainingKBees--;
				continue;
			}

			killerBee[i].setPosition(kBee_x[i], kBee_y[i]);
		}

	moveClock.restart();
	}
	
	
///////////////// Collision of bees with bullet ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	/*if(bullet_exists){
		for(int i = 0; i < levelOneKbees; i++){
			if(!kBeeActive[i]) continue;
			float beeWidth = boxPixelsX * killerBee[i].getScale().x;
        		float beeHeight = boxPixelsY * killerBee[i].getScale().y;

        		float bulletWidth = boxPixelsX * bulletSprite.getScale().x;
        		float bulletHeight = boxPixelsY * bulletSprite.getScale().y;
			
		
			float leftA = kBee_x[i];
			float rightA = kBee_x[i] + beeWidth;
			float topA = kBee_y[i];
			float bottomA = kBee_y[i] +  beeHeight;
		
			float leftB = bullet_x;
			float rightB = bullet_x + bulletWidth;
			float topB = bullet_y;
			float bottomB = bullet_y + bulletHeight;
			
			if(leftA < rightB && rightA > leftB && topA < bottomB && bottomA > topB){
				
				
				score += 100;
				scoreText.setString("Score: " + std::to_string(score));
				
				kBeeActive[i] = false;
				bullet_exists = false;
				
				remainingKBees--;
				remainingBullets--;
				collidedkBees++;
				
				khoneyComb_x[i] = kBee_x[i];
				khoneyComb_y[i] = kBee_y[i];
				alivekHoneyComb[i] = true;
				
				
				bullet_x = player_x;
				bullet_y = player_y;
			}
		}
			
}*/

if(bullet_exists){
		for(int i = collidedkBees; i < levelOneKbees; i++){
			if(!kBeeActive[i]) continue;
		
			float leftA = kBee_x[i];
			float rightA = kBee_x[i] + frameRect.width;
			float topA = kBee_y[i];
			float bottomA = kBee_y[i] +  frameRect.height;
		
			float leftB = bullet_x;
			float rightB = bullet_x + frameRect.width;
			float topB = bullet_y;
			float bottomB = bullet_y + frameRect.height;
			
			if(leftA < rightB && rightA > leftB && topA < bottomB && bottomA > topB){
				
				
				score += 100;
				scoreText.setString("Score: " + std::to_string(score));
				
				kBeeActive[i] = false;
				bullet_exists = false;
				
				remainingKBees--;
				remainingBullets--;
				collidedkBees++;
				
				khoneyComb_x[i] = kBee_x[i];
				khoneyComb_y[i] = kBee_y[i];
				alivekHoneyComb[i] = true;
				
				
				bullet_x = player_x;
				bullet_y = player_y;
			}
		}
			
}


//////////////////For Drawing kBees ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	for (int i = 0; i < levelOneKbees; i++) {
		if (kBeeActive[i]) {
		killerBee[i].setPosition(kBee_x[i], kBee_y[i]);
		window.draw(killerBee[i]);
	}
}	


	
////////// Drawing HoneyCombs /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	for(int i = 0; i < levelOneKbees; i++ ){
		if(alivekHoneyComb[i]){
			khoneyComb[i].setPosition(khoneyComb_x[i], khoneyComb_y[i]);
			khoneyComb[i].setScale(1.0f,1.0f);
			window.draw(khoneyComb[i]);
			}
	}
	
//////////////////// Collision of bullet wiht honeyCOmb //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

if(bullet_exists){
		for(int i = 0; i < levelOneKbees; i++){
			if (!alivekHoneyComb[i]) continue; 
			
			float leftA = bullet_x;
			float rightA = bullet_x + frameRect.width;
			float topA = bullet_y;
			float bottomA = bullet_y + frameRect.height;
			
			float leftB = khoneyComb_x[i];
			float rightB = khoneyComb_x[i] + khoneyComb[i].getGlobalBounds().width;
			float topB = khoneyComb_y[i];
			float bottomB = khoneyComb_y[i] + khoneyComb[i].getGlobalBounds().height;
			
			if (leftA < rightB && rightA > leftB && topA < bottomB && bottomA > topB){
			
				remainingBullets--;
				alivekHoneyComb[i] = false;
				bullet_exists = false;
			}
		}
	}

	



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	for(int i = 0; i < levelOneKbees; i++){
		
		if(!kBeeActive[i]) continue;
		

			float leftA = kBee_x[i];
			float rightA = kBee_x[i] + frameRect.width;
			float topA = kBee_y[i];
			float bottomA = kBee_y[i] +  frameRect.height;
			
			for(int j = 0 ; j < levelOneKbees; j++ ){
				if (!alivekHoneyComb[j]) continue; 
				float leftB = khoneyComb_x[j];
				float rightB = khoneyComb_x[j] + khoneyComb[j].getGlobalBounds().width;;
				float topB = khoneyComb_y[j];
				float bottomB = khoneyComb_y[j] + khoneyComb[j].getGlobalBounds().height;
			
				if (leftA < rightB && rightA > leftB && topA < bottomB && bottomA > topB) {
					
					kBeeDirection[i] = !kBeeDirection[i];
					kBee_y[i] += 48;
					killerBee[i].setScale(kBeeDirection[i] ? -1.0f : 1.0f, 1.0f);
				
					hitWall.play();
					kBee_x[i] += kBeeDirection[i] ? pushBack : -pushBack;
					
				}
			}
	}

}
///////////////// Function to save the score at the end of the game ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void saveUserScore(int score, sf::RenderWindow& window, sf::Font& fonts) {
 	
 	Text promptText("Enter your name: ", fonts, 25);
	promptText.setPosition(100, 200);
	Text nameText("", fonts, 18);
	nameText.setPosition(100, 240);
    
	string name;
	bool enterPressed = false;

	while (window.isOpen() && !enterPressed) {
		Event event;
		while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		} 
		if (event.type == sf::Event::TextEntered) {
			if (event.text.unicode == '\b' && !name.empty()) {
	
				name.pop_back();
                	}
                	else if (event.text.unicode == '\r') {
                    // If Enter key is pressed
                    		if (!name.empty()) {
					enterPressed = true;
				}
                	}
			else if (event.text.unicode < 128) {
                   
				if (name.size() < 20) { // Limit to 20 characters
					name += static_cast<char>(event.text.unicode);
				}
                	}
            }
            
            	
        }

		// Updating the  displayed text on the screen
		nameText.setString(name);

        
		window.clear();
		window.draw(promptText);
		window.draw(nameText);
		window.display();
    	}

    	// Saving name and score to file
	 ofstream scoreFile("scores.txt", std::ios::app);
	if (scoreFile.is_open()) {
		scoreFile << name << " " << score << "\n";
		scoreFile.close();
	}
	 
}

//////////////A Function to display the score board //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void displayScoreboard(sf::RenderWindow& window, sf::Font& font) {
    
	ifstream scoreFile("scores.txt");
	if (!scoreFile.is_open()) {
		return;
	}

    
	const int MAX_SCORES = 100;
	std::string scores[MAX_SCORES];
	int scoreCount = 0;
	std::string line;
	while (getline(scoreFile, line) && scoreCount < MAX_SCORES) {
		scores[scoreCount] = line;
		scoreCount++;
	       }
	       
	scoreFile.close();
	
	
    
	window.clear(); 
	sf::Text titleText("Scoreboard", font, 30);
	titleText.setPosition(50, 10);
	titleText.setFillColor(sf::Color::Yellow);
	window.draw(titleText);

	float y = 60; 
	for (int i = 0; i < scoreCount; i++) {
		sf::Text scoreText(scores[i], font, 18);
		scoreText.setPosition(55, y);
		scoreText.setFillColor(sf::Color::Yellow);
		window.draw(scoreText);
		y += 40; 
    	}
	window.display();

    
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || 
 				(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
				return;
           			 }
			}
 		}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void moveSprite(Sprite& hummingBird, float& bird_x, float& bird_y, float& velocity_x, float& velocity_y, Clock& clock, RenderWindow& window, Clock& animationClock,
	int& currentFrame, int totalFrames, int width, int height, float honeyComb_x[], float honeyComb_y[], bool aliveHoneyComb[], int totalBees, int widthh ,int heighth) {
    // Time-based movement for smooth animation
    Time deltaTime = clock.restart();
    bird_x += velocity_x * deltaTime.asSeconds() * 100;
    bird_y += velocity_y * deltaTime.asSeconds() * 100;

    // Randomly change direction every 2 seconds
    static Clock changeDirectionClock;
    if (changeDirectionClock.getElapsedTime().asSeconds() > 2) {
        velocity_x = (std::rand() % 200 - 100) / 100.0f;
        velocity_y = (std::rand() % 200 - 100) / 100.0f;
        changeDirectionClock.restart();
    }

    // Handle screen boundaries
    if (bird_x < 0) {
        bird_x = 0;
        velocity_x = -velocity_x;
    } else if (bird_x > window.getSize().x - 64) { 
       bird_x = window.getSize().x - 64;
        velocity_x = -velocity_x;
    }

    if (bird_y < 0) {
        bird_y = 0;
        velocity_y = -velocity_y;
    } else if (bird_y > window.getSize().y - 64) {  
        bird_y = window.getSize().y - 64;
        velocity_y = -velocity_y;
    }
    const float animationDelay = 0.1f;
    if (animationClock.getElapsedTime().asSeconds() > animationDelay) {
	currentFrame = (currentFrame + 1) % totalFrames;
	hummingBird.setTextureRect(sf::IntRect(currentFrame * width, 0, width, height));
	animationClock.restart(); 
	}
	
	for(int i = 0; i < totalBees; i++){
		if(!aliveHoneyComb[i]) continue;
		
		float leftA = bird_x;
		float rightA = bird_x + width;
		float topA = bird_y;
		float bottomA = bird_y + height;
			
		 
		float leftB = honeyComb_x[i];
		float rightB = honeyComb_x[i] + widthh;
		float topB = honeyComb_y[i];
		float bottomB = honeyComb_y[i] + heighth;
			
				if (leftA < rightB && rightA > leftB && topA < bottomB && bottomA > topB) {
				
					aliveHoneyComb[i] = false;
					velocity_x = 0;
					velocity_y = 0;
				}
		}
					
		
		
    
    hummingBird.setPosition(bird_x, bird_y);
    window.draw(hummingBird);
}


	



