#define xPin 6
#define yPin 5
#define potPin A0

#define playerWidth 5
#define playerHeight 20

#define ballSize 10
#define ballRootSize 5

#define screenWidth 254
#define maxHeight 	127
#define minHeight 	127
#define minWidth  	127
#define maxWidth  	127

#define maxPlayerSpeed 10

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
			acc = 0;
		}

		void update() {
			vel += acc;
			posY += vel;

			if (posY + (playerHeight / 2) < minHeight || posY - (playerHeight / 2) > maxHeight) {
				constrain(posY, minHeight, maxHeight);
			}

			acc = 0;
		}

		void move(int y) {
			acc += y;
			acc = constrain(acc, -maxPlayerSpeed, maxPlayerSpeed);
		}
};

Player player1( minWidth + playerWidth);
Player player2( maxWidth - playerWidth);

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
	float i;
	float j;
	{
		vertex(-127, -127);
		vertex( 127, -127);
		vertex( 127,  127);
		vertex(-127,  127);
	}
}

void vertex(int x, int y) {
	delayTime = map(analogRead(potPin), 0, 1024, 0, 16000);	
	analogWrite(xPin, constrain(x, -127, 127) + 127);
	analogWrite(yPin, constrain(y, -127, 127) + 127);
	delayMicroseconds(delayTime);
}
