/*
 Name:		SonarCave.ino
 Created:	07-Feb-18 9:09:25 PM
 Author:	BLANK
*/

const int trigPinF = 29;   //Define front sonar pins
const int echoPinF = 27;

const int trigPinL = 20;   //Define left sonar pins
const int echoPinL = 21;

const int trigPinR = A15;   //Define right sonar pins
const int echoPinR = A14;

const int motor_inputs[4] = { 32, 34, 40, 38 };   //Motor input pins
const int leftMotor = 2;
const int rightMotor = 3;
int motor_driver_on = 36;

const int motorSpeed = 150;   //Initializes motor speed
int leftMotorSpeed = motorSpeed;
int rightMotorSpeed = motorSpeed;


void setup()
{
	for (int i = 0; i < 4; i++)
	{
		pinMode(motor_inputs[i], OUTPUT);
	}

	digitalWrite(motor_driver_on, HIGH);

	pinMode(trigPinF, OUTPUT);
	pinMode(echoPinF, INPUT);

	pinMode(trigPinL, OUTPUT);
	pinMode(echoPinL, INPUT);

	pinMode(trigPinR, OUTPUT);
	pinMode(echoPinR, INPUT);

	Serial.begin(9600);
}


void loop()
{
	long durationFront = 0, durationLeft = 0, durationRight = 0;   //Duration Variables
	long distanceFront = 0, distanceLeft = 0, distanceRight = 0;   //Distance Variables

	distanceFront = FindDistanceFront();
	distanceLeft = FindDistanceLeft();
	distanceRight = FindDistanceRight();

	Serial.print("Front distance is ");
	Serial.print(distanceFront);
	Serial.print('\t');
	Serial.print("Left distance is ");
	Serial.print(distanceLeft);
	Serial.print('\t');
	Serial.print("Right distance is ");
	Serial.print(distanceRight);
	Serial.println('\t');
	//delay(500);


	StopMotors();  //Stops the motors after entering the cave
				   //Serial.println("Entered the cave");

	if (distanceFront > 15) //If there is a free path ahead
	{
		if (distanceLeft > 12 && distanceLeft < 15) // If distance is between 12 to 15 the robot moves straight;
		{
			MoveForward(100, 100);
		}

		if (distanceLeft >= 15)  // If distance is greater than or equal to 15 cm the robot increases its left motor speed to approach the left wall;
		{
			MoveForward(60, 120);
		}


		if (distanceLeft <= 12)   //If distance is less than or equal to 12 cm the robot increases its right motor speed to move away from the left wall;
		{
			MoveForward(120, 60);
		}
	}

	if (distanceFront <= 15)
	{

		if (distanceLeft <= 20 && distanceRight > 20)
			Right90();

		if (distanceLeft > 20 && distanceRight > 20)
			Right90();

		if (distanceLeft > 20 && distanceRight <= 20)
			Left90();

		if (distanceLeft <= 20 && distanceRight <= 20)
			Back180();
	}
}

long FindDistanceFront()   //Calculates Front Distance
{
	long durationFront = 0;
	long distanceFront = 0;

	digitalWrite(trigPinF, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPinF, HIGH);
	delayMicroseconds(5);
	digitalWrite(trigPinF, LOW);
	durationFront = pulseIn(echoPinF, HIGH, 20000);
	distanceFront = durationFront / 29 / 2;   //Speed of sound 29 microseconds per centimeter which is divided by 2 as the sound goes back and forth

	return distanceFront;
}

long FindDistanceLeft()   //Calculates Left Distance
{
	long durationLeft = 0;
	long distanceLeft = 0;

	digitalWrite(trigPinL, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPinL, HIGH);
	delayMicroseconds(5);
	digitalWrite(trigPinL, LOW);
	durationLeft = pulseIn(echoPinL, HIGH, 5000);
	distanceLeft = durationLeft / 29 / 2;

	return distanceLeft;
}

long FindDistanceRight()   //Calculate Right Distance
{
	long durationRight = 0;
	long distanceRight = 0;

	digitalWrite(trigPinR, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPinR, HIGH);
	delayMicroseconds(5);
	digitalWrite(trigPinR, LOW);
	durationRight = pulseIn(echoPinR, HIGH, 20000);
	distanceRight = durationRight / 29 / 2;

	return distanceRight;
}

void Left90() //function to make the robot rotate 90 degree to the left if there is no front and right exit;
{
	analogWrite(leftMotor, 120);
	analogWrite(rightMotor, 120);

	digitalWrite(motor_inputs[0], LOW);
	digitalWrite(motor_inputs[1], HIGH);
	digitalWrite(motor_inputs[2], HIGH);
	digitalWrite(motor_inputs[3], LOW);

	delay(400);
	Serial.println("Rotates 90 degree to the left");
}

void Right90() //function to make the robot rotate 90 degree to the right if there is no forward or left exit;
{
	analogWrite(leftMotor, 120);
	analogWrite(rightMotor, 120);

	digitalWrite(motor_inputs[0], HIGH);
	digitalWrite(motor_inputs[1], LOW);
	digitalWrite(motor_inputs[2], LOW);
	digitalWrite(motor_inputs[3], HIGH);

	delay(400);
	Serial.println("Rotates 90 degree to the right");
}

void Back180() //function to make the robot rotate 180 degree if there is no forward, left and right exit;
{
	analogWrite(leftMotor, 120);
	analogWrite(rightMotor, 120);

	digitalWrite(motor_inputs[0], LOW);
	digitalWrite(motor_inputs[1], HIGH);
	digitalWrite(motor_inputs[2], LOW);
	digitalWrite(motor_inputs[3], HIGH);

	delay(1200);
	Serial.println("Rotates 180 degree");
}

void MoveForward(int leftSpeed, int RightSpeed)   //Moves forward at given speed
{
	analogWrite(leftMotor, leftSpeed);
	analogWrite(rightMotor, RightSpeed);

	digitalWrite(motor_inputs[0], HIGH);
	digitalWrite(motor_inputs[1], LOW);
	digitalWrite(motor_inputs[2], HIGH);
	digitalWrite(motor_inputs[3], LOW);
}

void StopMotors()   //Stop the motors
{
	analogWrite(leftMotor, 0);
	analogWrite(rightMotor, 0);
	digitalWrite(motor_inputs[0], LOW);
	digitalWrite(motor_inputs[1], LOW);
	digitalWrite(motor_inputs[2], LOW);
	digitalWrite(motor_inputs[3], LOW);
}
