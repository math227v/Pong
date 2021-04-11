#define xPin 6
#define yPin 5
#define potPin A0

#define playerWidth 5
#define playerHeight 50

#define player1Pin A1
#define player2Pin A2

#define ballSize 10

#define screenWidth 254
#define minHeight  -127
#define maxHeight 	127
#define minWidth   -127
#define maxWidth  	127

#define maxPlayerSpeed 10

// Initialising with a default
int delayTime = 1000;

class Player {
	public:
		int posX;
		int posY;

		Player(int x) {
			posY = 0;
			posX = x;
		}

		// Basic movement algorithm
		void update(int pos) {
			posY = map(pos, 0, 1024, -127, 127);

			// Limit player position to within screen
			if (outOfBounds()) {
				posY = constrain(posY, minHeight + (playerHeight / 2) - 1, maxHeight - (playerHeight / 2) + 1);
			}
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

			velX = 2;
			velY = 3;
		}

		void update(Player p1, Player p2) {
			posX += velX;
			posY += velY;

			// Collision Detection
			if (posX - (ballSize / 2) <= -127) {
				int distanceAbs = abs(p1.posY - posY);
				int distance = p1.posY - posY;
				if (distanceAbs < playerHeight / 2) {
					// Hit
					velX *= -1;
					velY = -distance / 4;
				} else {
					reset();
				}
			}

			if (posX + (ballSize / 2) >=  127) {
				int distanceAbs = abs(p2.posY - posY);
				int distance = p2.posY - posY;
				if (distanceAbs < playerHeight / 2) {
					// Hit
					velX *= -1;
					velY = -distance / 4;
				} else {
					reset();
				}
			}

			if (abs(posY) + (ballSize / 2) >= 127) { velY *= -1; }
		}

	private:
		void reset() {
			posX = 0;
			posY = 0;
			velX *= -1;
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




	player1.update(analogRead(player1Pin));
	player2.update(analogRead(player2Pin));

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
