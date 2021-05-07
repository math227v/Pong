#define xPin 6
#define yPin 5
#define potPin A0

#define playerWidth 20
#define playerHeight 50

#define player1Pin A1
#define player2Pin A2

#define ballSize 10
#define ballSpeedMultiplier 6

#define screenWidth 254
#define minHeight  -127
#define maxHeight 	127
#define minWidth   -127
#define maxWidth  	127

#define drawBorder

#define pikHoved Tommy

// Initialising with a default
int delayTime = 1000;

class CustomVector {
	public:
		float x;
		float y;

		CustomVector() {
			x = 0;
			y = 0;
		}

		CustomVector(float _x, float _y) {
			x = _x;
			y = _y;
		}

		float mag() {
			return sqrt(pow(x, 2) + pow(y, 2));
		}

		void normalise() {
			float magnitude = mag();
			if (magnitude != 0) {
				x /= magnitude;
				y /= magnitude;
			}
		}

		void mult(float n) {
			x *= n;
			y *= n;
		}

		void setMag(float n) {
			normalise();
			mult(n);
		}

		void add(CustomVector v) {
			x += v.x;
			y += v.y;
		}
};

class Player {
	public:
		CustomVector pos;

		Player(int x) {
			pos = CustomVector(x, 0);
		}

		// Basic movement algorithm
		void update(int _pos) {
			pos.y = map(_pos, 0, 1024, -127, 127);

			// Limit player position to within screen
			if (outOfBounds()) {
				pos.y= constrain(pos.y, minHeight + (playerHeight / 2) - 1, maxHeight - (playerHeight / 2) + 1);
			}
		}

	private:
		boolean outOfBounds() {
			bool result = ((pos.y- (playerHeight / 2)) < minHeight || (pos.y+ (playerHeight / 2)) > maxHeight);
			return result;
		}
};

class Ball {
	public:
		CustomVector pos;
		CustomVector vel;

		Ball() {
			pos = CustomVector(0, 0);
			vel = CustomVector(1, 1);
			vel.setMag(ballSpeedMultiplier);
		}

		void update(Player p1, Player p2) {
			pos.add(vel);

			// Collision Detection
			// Player 1
			if ((pos.x - (ballSize / 2) <= -127 + playerWidth) && vel.x < 0) { handleHit(p1); }

			// Player 2
			if ((pos.x + (ballSize / 2) >=  127 - playerWidth) && vel.x > 0) { handleHit(p2); };

			if (abs(pos.y) + (ballSize / 2) >= 127) { vel.y *= -1; }
		}

		void handleHit(Player player) {
			int distanceAbs = abs(player.pos.y - pos.y) - (ballSize / 2);
			int distance = player.pos.y- pos.y;
			if (distanceAbs < playerHeight / 2) {
				// Hit player
				vel.x *= -1;
				vel.y = -distance / 3.5;
				vel.setMag(ballSpeedMultiplier);
			} else {
				// Hit wall
				reset();
			}
		}

	private:
		void reset() {
			pos.x = 0;
			vel.x *= -1;
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

void loop() {
	int p1PosY1 = player1.pos.y - (playerHeight / 2);
	int p1PosY2 = player1.pos.y + (playerHeight / 2);

	int p2PosY1 = player2.pos.y - (playerHeight / 2);
	int p2PosY2 = player2.pos.y + (playerHeight / 2);


	#ifdef drawBorder
		vertex(-127, -127);		// LEFT		BOTTOM

		// Render Player 1
		vertex(-127, p1PosY1);
	#endif

	vertex(player1.pos.x, p1PosY1);
	vertex(player1.pos.x, p1PosY2);
	vertex(-127, p1PosY2);

	#ifdef drawBorder
		vertex(-127,  127);		// LEFT		TOP
		vertex( 127,  127);		// RIGHT	TOP

		// Render Player 2
		vertex(127, p2PosY2);
	#endif
	vertex(player2.pos.x, p2PosY2);
	vertex(player2.pos.x, p2PosY1);
	#ifdef drawBorder
		vertex(127, p2PosY1);

		vertex( 127, -127);		// RIGHT	BOTTOM
	#endif

	// Render Ball
	int ballPosXL = ball.pos.x - (ballSize / 2);
	int ballPosXR = ball.pos.x + (ballSize / 2);
	int ballPosYB = ball.pos.y - (ballSize / 2);
	int ballPosYT = ball.pos.y + (ballSize / 2);

	#ifdef drawBorder
		vertex(ball.pos.x, -127);
		vertex(ball.pos.x, ballPosYB);
	#endif

	vertex(ballPosXL, ballPosYB);
	vertex(ballPosXL, ballPosYT);
	vertex(ballPosXR, ballPosYT);
	vertex(ballPosXR, ballPosYB);

	#ifdef drawBorder
		vertex(ball.pos.x, ballPosYB);
		vertex(ball.pos.x, -127);
	#endif



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
