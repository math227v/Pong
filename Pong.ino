#define xPin 6
#define yPin 5
#define potPin A0

#define playerWidth 5
#define playerHeight 50

#define ballSize 10
#define ballRootSize 5

#define screenWidth 254
#define minHeight  -127
#define maxHeight 	127
#define minWidth   -127
#define maxWidth  	127

#define maxPlayerSpeed 10

#define noCollision		0
#define playerBounce 	1
#define playerDead 		2

// Initialising with a default
int delayTime = 1000;

class Player {
	public:
		int posX;
		int posY;
		int vel;
		int acc;

		Player(int x) {
			posY = 0;
			posX = x;
			vel = 0;
			acc = 1;
		}

		// Basic movement algorithm
		void update() {
			vel += acc;
			posY += vel;

			// Limit player position to within screen
			if (outOfBounds()) {
				posY = constrain(posY, minHeight + (playerHeight / 2) - 1, maxHeight - (playerHeight / 2) + 1);
				if (outOfBounds()) {
					vel *= -1;
				}
			}

			acc = 0;
		}

		void move(int y) {
			acc += y;
			acc = constrain(acc, -maxPlayerSpeed, maxPlayerSpeed);
		}

	private:
		boolean outOfBounds() {
			bool result = ((posY - (playerHeight / 2)) < minHeight || (posY + (playerHeight / 2)) > maxHeight);
			return result;
		}
};

class Ball {
	public: 
		int posX;
		int posY;

		int velX;
		int velY;

		Ball() {
			posX = 0;
			posY = 0;

			velX = 1;
			velY = 1;
		}

		void update(Player p1, Player p2) {
			posX += velX;
			posY += velY;

			// Collision Detection

			// Y Axis
			if (abs(posY) + ballSize == 127) { velY *= -1; }

			switch (detectPlayerCollision(p1))
			{
			case noCollision:
				break;
			
			case playerBounce:
				// velX *= -1;
				break;

			case playerDead:
				velX *= -1;
				break;

			default:
				break;
			}

			switch (detectPlayerCollision(p2))
			{
			case noCollision:
				break;
			
			case playerBounce:
				velX *= -1;
				break;

			case playerDead:
				velX *= -1;
				break;

			default:
				break;
			}
		}

		void move() {

		}

	private:
		int detectPlayerCollision(Player player) {
			if (abs(player.posX) - (abs(posX) + (ballSize / 2)) <= 0 ) {
				return playerBounce;
			} else { 
				return noCollision; 
			};
		}
};

Player player1( minWidth + playerWidth);
Player player2( maxWidth - playerWidth);
Ball ball = Ball();

void setup() {
	pinMode(xPin, OUTPUT);
	pinMode(yPin, OUTPUT);
	pinMode(potPin, INPUT);

	#ifndef __INTELLISENSE__
		TCCR0A =  (TCCR0A & B10100011 + TCCR0A | B10100011);  //(1<<COM0A1 | 0<<COM0A0 | 1<<COM0B1 | 0<<COM0B0 | 1<<WGM01 | 1<<WGM00);
		TCCR0B =  (TCCR0B & B00000001 + TCCR0B | B00000001);  //(0<<FOC0A | 0<<FOC0B | 0<<WGM02 | 0<<CS02 | 0<<CS01 | 1<<CS00);
		TIMSK0 =  (TIMSK0 & B11111000);                       //(0<<OCIE0B | 0<<OCIE0A  | 0<<TOIE0);
	#endif
}

float offset = 0;

void loop() {
	int p1PosY1 = player1.posY - (playerHeight / 2);
	int p1PosY2 = player1.posY + (playerHeight / 2);

	int p2PosY1 = player2.posY - (playerHeight / 2);
	int p2PosY2 = player2.posY + (playerHeight / 2);

	vertex(-127, -127);		// LEFT		BOTTOM

	// Render Player 1
	vertex(-127, p1PosY1);
	vertex(player1.posX, p1PosY1);
	vertex(player1.posX, p1PosY2);
	vertex(-127, p1PosY2);

	vertex(-127,  127);		// LEFT		TOP
	vertex( 127,  127);		// RIGHT	TOP
	
	// Render Player 1
	vertex(127, p2PosY2);
	vertex(player2.posX, p2PosY2);
	vertex(player2.posX, p2PosY1);
	vertex(127, p2PosY1);
	
	vertex( 127, -127);		// RIGHT	BOTTOM

	// Render Ball
	int ballPosXL = ball.posX - (ballSize / 2);
	int ballPosXR = ball.posX + (ballSize / 2);
	int ballPosYB = ball.posY - (ballSize / 2);
	int ballPosYT = ball.posY + (ballSize / 2);

	vertex(ball.posX, -127);
	vertex(ball.posX, ballPosYB);

	vertex(ballPosXL, ballPosYB);
	vertex(ballPosXL, ballPosYT);
	vertex(ballPosXR, ballPosYT);
	vertex(ballPosXR, ballPosYB);

	vertex(ball.posX, ballPosYB);
	vertex(ball.posX, -127);



	player1.move(0);
	player1.update();

	player2.move(0);
	player2.update();

	ball.move();
	ball.update(player1, player2);
}

int prevX = 0;
int prevY = 0;

void vertex(int x, int y) {
	// float distance = sqrt(pow(prevX - x, 2) + pow(prevY - y, 2)) + 10;
	delayTime = map(analogRead(potPin), 0, 1024, 0, 2000);	
	analogWrite(xPin, constrain(x, -127, 127) + 127);
	analogWrite(yPin, constrain(y, -127, 127) + 127);

	// prevX = x;
	// prevY = y;

	delayMicroseconds(delayTime);
}
