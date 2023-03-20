/***************************************/
/*   Group C  */
/*   3/10/2023   */
/*   Drive function    */
/***************************************/


#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHBuzzer.h>
#include <stdio.h>


// Motor Inputs
DigitalEncoder right_encoder(FEHIO::P0_0);
DigitalEncoder left_encoder(FEHIO::P0_2);
FEHMotor right_motor(FEHMotor::Motor0,9.0);
FEHMotor left_motor(FEHMotor::Motor1,9.0);


// Bumper Switch Inputs
DigitalInputPin frontSwitchLeft(FEHIO::P1_2);
DigitalInputPin frontSwitchRight(FEHIO::P1_4);


// CdS Cell Input
AnalogInputPin sensor(FEHIO::P1_0);


// Line Following Inputs
AnalogInputPin leftLineF(FEHIO::P2_0);
AnalogInputPin centerLineF(FEHIO::P2_3);
AnalogInputPin rightLineF(FEHIO::P2_7);


//Define cool constants
#define LEFT_TURN_PERCENT 40
#define RIGHT_TURN_PERCENT 40
#define LEFT_RAMPING_SPEED 50
#define RIGHT_RAMPING_SPEED 50


void drive(int FoB, int percent, int inches)
{
    // drive(0) will drive backwards
    // drive(1) will drive forwards
    // Added inches to counter converter, so
    // we will only need to enter the inches we need to travel
    // this can be extended to RPS units once we know the conversion




    // Declare counts
    float counts;


    // convert inches to tic counts
    counts = (inches*318)/11;


    // Set forward (1) or backword (0)
    if(FoB==1)
    {
        percent = percent;
    }
    if(FoB==0)
    {
        percent = -percent;
    }


    // Set counts to zero
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();


    //Set both motors to percent speed WILL NEED ADJUSTMENT FOR MOTORS
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);


    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2 < counts);


    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}


void turn90(int direction)
{
    // turn(0) turns to the left (counterclockwise)
    // turn(1) turns to the right (clockwise)
   
    // reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();




    // creates a variable that sets the maximum amount of counts (318 counts in a full rotation of a wheel)
    int maxCounts=190;


    if(direction==0)
    {
        // set right motor power full speed forward
        right_motor.SetPercent(RIGHT_TURN_PERCENT);
 
        // set right motor power full speed backward
        left_motor.SetPercent(-LEFT_TURN_PERCENT);
        maxCounts=190;
       
    }


    else
    {
        // set right motor power full speed forward
        right_motor.SetPercent(-RIGHT_TURN_PERCENT);
 
        // set right motor power full speed backward
        left_motor.SetPercent(LEFT_TURN_PERCENT);
    }
   
    while(((left_encoder.Counts() + right_encoder.Counts())/2)<maxCounts);




    // Turn off motors once maxCounts are reached
    right_motor.Stop();
    left_motor.Stop();


}


void turnDegree(int direction, int degree) //This function turns the robot a set degree using one of its wheels as a pivot point
{
    // turn(0) turns to the left (counterclockwise)
    // turn(1) turns to the right (clockwise)
   
    // reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();


    // creates a variable that sets the maximum amount of counts (318 counts in a full rotation of a wheel)
    int maxCounts;


    //Variable for the width of the robot, which acts as the turning radius
    float radius=7;


    //convert degree into radians
    float radian;
    radian=degree*3.14159265358979323846264338327/180;


    //Calculate maxCounts
    maxCounts=radius*radian*318/11;


    if(direction==0)
    {
        // set right motor power full speed forward
        right_motor.SetPercent(RIGHT_TURN_PERCENT);
        //left_motor.SetPercent(-LEFT_TURN_PERCENT);        
    }


    else
    {
        // set right motor power full speed backward
        left_motor.SetPercent(LEFT_TURN_PERCENT);
        //right_motor.SetPercent(-RIGHT_TURN_PERCENT);
    }
   
    //Turn through the maxCounts
    while((left_encoder.Counts() + right_encoder.Counts())<maxCounts);


    // Turn off motors once maxCounts are reached
    right_motor.Stop();
    left_motor.Stop();
}


void turnDegreeBackwards(int direction, int degree) //This function turns the robot a set degree backwards using one of its wheels as a pivot point
{
    // turn(0) turns to the left (clockwise)
    // turn(1) turns to the right (counterclockwise)
   
    // reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();


    // creates a variable that sets the maximum amount of counts (318 counts in a full rotation of a wheel)
    int maxCounts;


    //Variable for the width of the robot, which acts as the turning radius
    float radius=7;


    //convert degree into radians
    float radian;
    radian=degree*3.14159265358979323846264338327/180;


    //Calculate maxCounts
    maxCounts=radius*radian*318/11;


    if(direction==0)
    {
        // set right motor
        right_motor.SetPercent(-RIGHT_TURN_PERCENT);        
    }


    else
    {
        // set left motor power
        left_motor.SetPercent(-LEFT_TURN_PERCENT);
    }
   
    //Turn through the maxCounts
    while((left_encoder.Counts() + right_encoder.Counts())<maxCounts);


    // Turn off motors once maxCounts are reached
    right_motor.Stop();
    left_motor.Stop();
}


void goUpRamp()
{
    //Set both motors to percent speed WILL NEED ADJUSTMENT FOR MOTORS
    right_motor.SetPercent(RIGHT_RAMPING_SPEED);
    left_motor.SetPercent(LEFT_RAMPING_SPEED);


    while((left_encoder.Counts() + right_encoder.Counts()) / 2 < 100);


    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}


void testRightMotor(int percent)
{
    right_motor.SetPercent(percent);
}


void testLeftMotor(int percent)
{
    left_motor.SetPercent(percent);
}


void linefollowing()
{
    // to be used to follow a line for either time or distance
    float L, R, C;
    // declare L,R,M as left, right and middle line sensors
    //Variables that display values for analogue inputs
    L=leftLineF.Value();
    C=centerLineF.Value();
    R=rightLineF.Value();


    while(L<1 && C<1.3 && R<1.2)
    {
        right_motor.SetPercent(30);
        left_motor.SetPercent(30);
    }


    //Line following algorithm
    //Determine case
    if (L>1 && C<1.3 && R<1.2)
    {
        //Need to go left
        left_motor.SetPercent(0);
        right_motor.SetPercent(30);
        Sleep(0.5);
        left_motor.SetPercent(30);
        right_motor.SetPercent(30);
    }
    else if (L<1 && C<1.3 && R>2)
    {
        //Need to go Right
        left_motor.SetPercent(30);
        right_motor.SetPercent(0);
        Sleep(0.5);
        left_motor.SetPercent(30);
        right_motor.SetPercent(30);
    }
    else
    {
        //Need to continue straight
        left_motor.SetPercent(30);
        right_motor.SetPercent(30);
    }
}


void displaySensorValues() //This function records and displays our values from the Cds cells and line following sensors, for testing purposes
{
    //Declare variables
    float CdS, leftLine, centerLine, rightLine;
    char s[30];


    while(true)
    {
        //Set variables equal to their respective values
        CdS=sensor.Value();
        leftLine=leftLineF.Value();
        centerLine=centerLineF.Value();
        rightLine=rightLineF.Value();


        //Clear the screen and print the values
        LCD.Clear();
        sprintf(s, "%f %f %f %f", CdS,leftLine,centerLine,rightLine);
        LCD.WriteLine(s);


        //Sleep so you got time to read
        Sleep(0.1);
    }
}


void bumperTest() //This function tests the bumper plate switches by displaying their values to the screen
{
    //Declare variables and clear screen
    int leftSwitch, rightSwitch;
    char s[30];
    LCD.Clear();


    while(true) //Take values from the switches and then print them to the screen continuously
    {
        leftSwitch=frontSwitchLeft.Value();
        rightSwitch=frontSwitchRight.Value();


        sprintf(s, "Left = %i and Right = %i", leftSwitch, rightSwitch);
        LCD.WriteLine(s);
    }
}


int determineColor() //This function determines the color of the light for the boarding pass and then moves to the correct light and displays it
{
    //Read the value of the CdS sensor
    float CdS;
    int return_color;


    //Sleep to get good reading
    Sleep(1.0);


    //read the value
    CdS=sensor.Value();


    //For Red
    if(CdS < 0.40)
    {
        //Make screen red
        LCD.Clear(RED);
        return_color=0;
    }
    //For Blue
    else if(CdS > 0.55)
    {
        //Make screen blue
        LCD.Clear(BLUE);  
        return_color=1;
    }
    else //error
    {
        return_color=2;
    }
    return return_color;
}


void goToWall(int percent) //This function drives straight until we hit a wall
{
    LCD.Clear();
    while(frontSwitchLeft.Value() || frontSwitchRight.Value())
    {
        //Drive forward with percent power
        left_motor.SetPercent(percent+1);
        right_motor.SetPercent(percent);
        LCD.WriteLine("DRIVING to waLL");
    }


    //Stop Motors
    left_motor.Stop();
    right_motor.Stop();


    //Kowalski, status report
    LCD.Clear();
    LCD.WriteLine("Skipper, I appear to have hit a wall");


    //Sleep for a sec
    Sleep(1.0);
}


void searchForLight() //This function drives forward until the robot detects the light
{
    int CdS;


    while(CdS=sensor.Value()>2.5)
    {
        //Drive until you find the light
        left_motor.SetPercent(20);
        right_motor.SetPercent(20);
    }


    //Stop motors
    left_motor.Stop();
    right_motor.Stop();
}


int main(void)
{  
    //Declare CdS variable
    float CdS=sensor.Value();


    // code for start with light
    LCD.Clear();


    // read the value of sensor
    while(CdS>2)
    {
        CdS=sensor.Value();
        LCD.WriteLine(CdS);
        Sleep(2.0);
    }
    LCD.WriteLine("Starting!"); //Status report


    /*
    //Wait for touch to start
    float x,y;
    while(!LCD.Touch(&x, &y));
    while(LCD.Touch(&x, &y));
    */
   
   
    //Drive forward 6 inches
    drive(1,30,6);




    //Turn right 30 degrees
    turnDegree(1,30);


    //Drive 3 inches
    drive(1,30,4);


    //Turn Right 30 degrees
    turnDegree(0,30);


    //Drive to ramp bottom
    drive(1,30,3);


    //Drive Up Ramp
    drive(1,50,13);


    //Drive forawd 6 inches to clear the ramp
    drive(1,30,6);


    //Status update
    LCD.Clear();
    LCD.WriteLine("I made it up the ramp!");


    //Turn 90 degrees left
    turnDegree(0,90);
   
    //Drive until we hit the wall
    goToWall(30);


    //Drive backwards a few inches then turn 90 degrees backwards clockwise
    drive(0, 30, 6);
    turnDegreeBackwards(0, 95);


    //Drive until we hit the light
    searchForLight();


    //Determine the color of the light and then display it and go hit the correct button
    int color;
    color=determineColor();


    //Go backwards a bit then turn 90 degrees backwards
    drive(0,30,3);
    turnDegreeBackwards(0,95);


    //Hit the proper button
    if(color==0) //red
    {
        //status report
        LCD.Clear();
        LCD.WriteLine("going to red button");


        //Drive to button
        drive(1,30,10);
        turnDegree(0,90);
        goToWall(30);


        //status report
        LCD.Clear();
        LCD.WriteLine("ayo Mr. White I hit the red button");


    }
    else if(color==1) //blue
    {
        //status report
        LCD.Clear();
        LCD.WriteLine("going to blue button");


        //Drive to button
        drive(1,30,5);
        turnDegree(0,90);
        goToWall(30);


        //status report
        LCD.Clear();
        LCD.WriteLine("ayo Mr. White I hit the blue button");
    }
    else //error
    {
        //Status report
        LCD.Clear(BLACK);
        LCD.WriteLine("Error:invalid color");
    }


    // Drive back 3 inches then turn backwards
    drive(0,30,15);
    turnDegreeBackwards(0,90);


    //Drive forward then turn
    goToWall(30);
    drive(0,30,2);
    turnDegree(1,90);


    //Go down ramp
    drive(1,30, 17);
}  

