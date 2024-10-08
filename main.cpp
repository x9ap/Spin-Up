/*----------------------------------------------------------------------------*/
/*    Module:       main.cpp                                                  */
/*    Author:       karod327                                                  */
/*    Created:      Tuesday Sep 6 2022                                        */
/*    Description:  Odometry Test                                             */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Right                motor         18                          
// Left                 motor         20                         
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <math.h>
#include <iostream> 
using namespace vex;
vex::competition    Competition;
/*-------------------------------Variables-----------------------------------*/
#define Pi 3.14159265358979323846
#define fieldscale 1 //1.66548042705
#define SL 4.9375 //distance from tracking center to middle of left wheel
#define SR 4.9375 //distance from tracking center to middle of right wheel
#define WheelDiam 4.1875 //diameter of all the wheels being used for tracking
#define tpr 360  //Degrees per single encoder rotation
double DeltaL,DeltaR,DeltaB,currentL,currentR,PreviousL,PreviousR,DeltaTheta,X,Y,Theta,DeltaXSide,DeltaYSide,SideChord,OdomHeading;
/*---------------------------------------------------------------------------*/
/*                            Odometry Functions                             */
/*---------------------------------------------------------------------------*/
void TrackPOS() {
// 2 cases could be occuring in odometry
// 1: Going in a straight line
// 2: Going in an arc motion
// If the bot is on an angle and going straight the displacement would be linear at angle Theta, meaning a right triangle is formed (Trig ratios to calc movement)
// Since it is a linear motion, the Left and right will move the same amount so we can just pick a side and do our movement calculation
// Since this calculation is working based of very infinitely small arcs, the displacement of the robot will be a chord
// Below it Averages the Left and Right integrated motor encoders since we don't have encoders yet
  currentR = Right.position(degrees);
  currentL = Left.position(degrees);

  //Creates variables for change in each side info in inches (12.9590697 is circumference of wheel)
  DeltaL = ((currentL - PreviousL) * 12.9590697) / tpr;
  DeltaR = ((currentR - PreviousR) * 12.9590697) / tpr;
  //DeltaB = ((currentB - PreviousB) * 12.9590697) / tpr;

  //Determines the change in angle of the robot using the rotational change in each side
  DeltaTheta = (DeltaR - DeltaL) / (SL + SR);

  //Creates an if/else statement to prevent NaN values from appearing and causing issues with calculation
  if(DeltaTheta == 0) {  //If there is no change in angle
    X += DeltaL * sin (Theta);
    Y += DeltaL * cos (Theta);
    //X += DeltaB * cos (Theta + 1.57079633);
    //Y += DeltaB * sin (Theta + 1.57079633);

  //If there is a change in angle, it will calculate the changes in X,Y from chords of an arc/circle.
  } else {  //If the angle changes
      SideChord = 2 * ((DeltaL / DeltaTheta) + SL) * sin (DeltaTheta / 2);
      //BackChord = 2 * ((DeltaB / DeltaTheta) + SS) * sin (DeltaTheta / 2);
      DeltaYSide = SideChord * cos (Theta + (DeltaTheta / 2));
      DeltaXSide = SideChord * sin (Theta + (DeltaTheta / 2));
      //DeltaXBack = BackChord * sin (Theta + (DeltaTheta / 2));
      //DeltaYBack = -BackChord * cos (Theta + (DeltaTheta / 2));
      Theta += DeltaTheta;
      X += DeltaXSide;
      Y += DeltaYSide;
    }

    //Odom heading is converting the radian value of Theta into degrees
    OdomHeading = Theta * 57.295779513;

    //Converts values into newer values to allow for code to effectively work in next cycle
    PreviousL = currentL;
    PreviousR = currentR;
    DeltaTheta = 0;
  /*--------------------GRAPHICS--------------------*/
    //Coordinates for each section of text
    int textadjustvalue = 55;
    int rowadjust = 39;

    //Sets graphical things for our display 
    Brain.Screen.setPenWidth( 1 );
    vex::color redtile = vex::color( 210, 31, 60 );
    vex::color bluetile = vex::color( 14, 77, 146 );
    vex::color graytile = vex::color( 49, 51, 53 );
    Brain.Screen.setFillColor(vex::color( 0, 0, 0 ));
    Brain.Screen.setFont(vex::fontType::mono20);
    Brain.Screen.setPenColor( vex::color( 222, 49, 99 ) );

    //Displays all the field tiles, text of odom values, and a dot symbolizing the robot
    Brain.Screen.printAt(40,20 + textadjustvalue, "X-Pos:%f",-X);
    Brain.Screen.setPenColor( vex::color( 191, 10, 48 ) );
    Brain.Screen.printAt(40,50 + textadjustvalue, "Y-Pos:%f",Y);
    Brain.Screen.setPenColor( vex::color( 141, 2, 31 ) );
    Brain.Screen.printAt(40,80 + textadjustvalue, "Theta:%f",Theta);
    Brain.Screen.setPenColor( vex::color( 83, 2, 1 ) );
    Brain.Screen.printAt(40,110 + textadjustvalue, "Angle:%f",OdomHeading);
    Brain.Screen.setPenColor( vex::color( 255, 255, 255 ) );
    Brain.Screen.setFillColor( graytile );
    Brain.Screen.drawRectangle( 245, 2, 234, 234 );
    Brain.Screen.drawRectangle( 245, 2, 39, 39 );
    Brain.Screen.drawRectangle( 245, 80, 39, 39 );
    Brain.Screen.drawRectangle( 245, 119, 39, 39 );
    Brain.Screen.drawRectangle( 245, 197, 39, 39 );
    Brain.Screen.drawRectangle( 245+rowadjust, 2, 39, 39 );
    Brain.Screen.drawRectangle( 245+rowadjust, 41, 39, 39 );
    Brain.Screen.drawRectangle( 245+rowadjust, 80, 39, 39 );
    Brain.Screen.drawRectangle( 245+rowadjust, 119, 39, 39 );
    Brain.Screen.drawRectangle( 245+rowadjust, 158, 39, 39 );
    Brain.Screen.drawRectangle( 245+rowadjust, 197, 39, 39 );
    Brain.Screen.drawRectangle( 245+(2*rowadjust), 2, 39, 39 );
    Brain.Screen.drawRectangle( 245+(2*rowadjust), 41, 39, 39 );
    Brain.Screen.drawRectangle( 245+(2*rowadjust), 80, 39, 39 );
    Brain.Screen.drawRectangle( 245+(2*rowadjust), 119, 39, 39 );
    Brain.Screen.drawRectangle( 245+(2*rowadjust), 158, 39, 39 );
    Brain.Screen.drawRectangle( 245+(2*rowadjust), 197, 39, 39 );
    Brain.Screen.drawRectangle( 245+(3*rowadjust), 2, 39, 39 );
    Brain.Screen.drawRectangle( 245+(3*rowadjust), 41, 39, 39 );
    Brain.Screen.drawRectangle( 245+(3*rowadjust), 80, 39, 39 );
    Brain.Screen.drawRectangle( 245+(3*rowadjust), 119, 39, 39 );
    Brain.Screen.drawRectangle( 245+(3*rowadjust), 158, 39, 39 );
    Brain.Screen.drawRectangle( 245+(3*rowadjust), 197, 39, 39 );
    Brain.Screen.drawRectangle( 245+(4*rowadjust), 2, 39, 39 );
    Brain.Screen.drawRectangle( 245+(4*rowadjust), 41, 39, 39 );
    Brain.Screen.drawRectangle( 245+(4*rowadjust), 80, 39, 39 );
    Brain.Screen.drawRectangle( 245+(4*rowadjust), 119, 39, 39 );
    Brain.Screen.drawRectangle( 245+(4*rowadjust), 158, 39, 39 );
    Brain.Screen.drawRectangle( 245+(4*rowadjust), 197, 39, 39 );
    Brain.Screen.drawRectangle( 245+(5*rowadjust), 2, 39, 39 );
    Brain.Screen.drawRectangle( 245+(5*rowadjust), 80, 39, 39 );
    Brain.Screen.drawRectangle( 245+(5*rowadjust), 119, 39, 39 );
    Brain.Screen.drawRectangle( 245+(5*rowadjust), 197, 39, 39 );
    Brain.Screen.setFillColor( redtile );
    Brain.Screen.drawRectangle( 245, 158, 39, 39 );
    Brain.Screen.drawRectangle( 245, 41, 39, 39 );
    Brain.Screen.setFillColor( bluetile );
    Brain.Screen.drawRectangle( 245+(5*rowadjust), 41, 39, 39 );
    Brain.Screen.drawRectangle( 245+(5*rowadjust), 158, 39, 39 );
    Brain.Screen.setPenColor( vex::color( 255,255,255));
    Brain.Screen.setFillColor( vex::color(0,0,0) );
    
    //This draws the robot body for position and arm for angle
    double yfieldvalue = ((Y)*fieldscale)+245-10;
    double xfieldvalue = ((X)*fieldscale)+245;
    Brain.Screen.drawCircle(xfieldvalue, yfieldvalue, 10 );
    Brain.Screen.setPenWidth( 4 );
    //Line angle calculation:
    //x1 and y1 are the robot's coordinates, which in our case is xfieldvalue and yfieldvalue
    //angle is the angle the robot is facing, which in our case is Theta
    //(x1,y1, x1 + line_length*cos(angle),y1 + line_length*sin(angle)) = (x1,y1,x2,y2)
    Brain.Screen.drawLine(xfieldvalue, yfieldvalue, xfieldvalue+cos(-Theta-(Pi/2))*15, yfieldvalue+ sin(-Theta-(Pi/2)) *15);
  }
/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*---------------------------------------------------------------------------*/
void pre_auton( void ) {
    Brain.Screen.print("Starting Vex Service...",2);
    vexcodeInit();
    Brain.Screen.print("Vex Service Online.",3);
    Brain.resetTimer();
    Brain.Screen.print("Timer Reset.",4);
    Brain.Screen.print("Syncing Motors...",5);
    Right.resetRotation();
    Brain.Screen.print("Right Front Primed...",6);
    Left.resetRotation();
    Brain.Screen.print("Left Front Primed...",7);

    //SET VALUES FOR INITIAL ROBOT POSITION
    X = 0;
    Brain.Screen.print("X Synced.",9);
    Y = 0;
    Brain.Screen.print("Y Synced.",10);

}
/*---------------------------------------------------------------------------*/
/*                              Autonomous Task                              */
/*---------------------------------------------------------------------------*/
void turnto(double theta) {
  double turn_amount = theta - Theta;
  if (turn_amount > 0) {
    while (Theta != theta) { 
      Left.spin(vex::directionType::rev, .2, vex::velocityUnits::pct);
      Right.spin(vex::directionType::fwd, .2, vex::velocityUnits::pct);
    }
  }
  if (turn_amount < 0) {
    while (Theta != theta) { 
      Left.spin(vex::directionType::fwd, .2, vex::velocityUnits::pct);
      Right.spin(vex::directionType::rev, .2, vex::velocityUnits::pct);
    }
  }
}

void go(double x, double y) {
  double x_rel = x - X;
  double y_rel = y - Y;
  turnto(atan2(y_rel, x_rel));
  while ((X != x) && (Y != y)) {
    Left.spin(vex::directionType::rev, 1, vex::velocityUnits::pct);
    Right.spin(vex::directionType::rev, 1, vex::velocityUnits::pct);
  }
}

void autonomous( void ) {
  go(4,3);
  turnto(0);
}
/*----------------------------------------------------------------------------*/
/*                              User Control Task                             */
/*----------------------------------------------------------------------------*/
void usercontrol( void ) {
  Brain.Screen.print("Initializing Input Service.",3);
  while (1){
    Brain.Screen.clearScreen();

    //provides power to the motors to allow for movement of robot for testing using controller
    Left.spin(vex::directionType::rev, ((Controller1.Axis3.value() + (Controller1.Axis1.value())*0.1)), vex::velocityUnits::pct);
    Right.spin(vex::directionType::rev, ((Controller1.Axis3.value() - (Controller1.Axis1.value())*0.1)), vex::velocityUnits::pct);
    
    if (Controller1.ButtonA.pressing()) {
      autonomous();
    }

    //Calls the TrackPosition function
    TrackPOS();
    Brain.Screen.render(); //push data to the LCD all at once to prevent image flickering
    vex::task::sleep(10); //Slight delay so the Brain doesn't overprocess
  }
}
int main() {
    Brain.Screen.print("Initializing...",1);
    pre_auton();
    Brain.Screen.print("Initialized...",12);
    wait(1, seconds);
    Brain.Screen.clearScreen();
    //Competition.autonomous( autonomous ); //Calls the autonomous function
    Brain.Screen.print("Autonomous Service Started.",1);
    Competition.drivercontrol( usercontrol ); //Calls the driver control function
    Brain.Screen.print("Driver Control Service Started.",2);
    while(1) {
      vex::task::sleep(5); //Slight delay so the Brain doesn't overprocess
    }
}

