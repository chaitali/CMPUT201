import java.awt.*;
import java.util.*;

/**
 *  @Lander.java
 *  @author John Donohue
 *  @version 1.3,  11 Jan 1999 
 *
 *    This abstract class will be subclassed for each of the three states the Player's lander can be in-
 *  Flying, Landed, and Exploding. This is an attempt to use the "State" design pattern to eliminate 
 *  the repeatative if/then's that would be needed to change the landers behavior according to the state
 *  it is in.  For example, the lander should only respond to the cursor keys to spin and the space bar 
 *  to fire the rocket if it is flying.   Likewise, the lander should only respod to the "T" key to 
 *  takeoff if it has landed. And pressing "any key" should start a new game only when the lander is 
 *  exploding following a crash.  If all the landers behavior was in one class we would need a series 
 *  of repeatative if/then's to check the landers state.  There would be three paths of execution through 
 *  the objects code based on one or two simple flags.
 *    Instead we will have seperate classes for each state.  Each class will contain the behavior the lander
 *  shows when in that state.  The FlyingLander class has code to check for collisons  with the landscape,
 *  and soft landings.  If it detects a crash it transmutes to a "ExplodingLander"  class, or if it sees
 *  a good landing it becomes a "LandedLander" class.  These classes have code to change back to a
 *  "FlyingLander" when certain keys are pressed.      
 *
 */


  public abstract class Lander implements Drawable {

  static protected double gravityAcceleration;
  static protected int attitude;
  static protected boolean landerSpun;
  static protected double lastScale;
  static protected double xDirection;
  static protected double yDirection;
  static protected boolean mainRocketOperating;
  static protected double landscapePosX;
  static protected double landscapePosY;
  static protected int screenPosX;
  static protected int screenPosY;
  static protected double deltaX;
  static protected double deltaY;
  static protected float fuel;
  static protected Vector landerLines;
  static protected JDLunarLander game;
  static protected long saveTimeAttitudeThrusterSoundStarted;
  static protected long saveTimeMainThrusterSoundStarted;
  static protected final double rocketAcceleration = (.015d);
  static protected final double maxSafeLandingSpeed = .25;


  double [][] landerArray = {
                           { -2d,   7d,  0d,  10d},
                           {  0d,  10d,  2d,   7d},
                           { -3d, -10d,  3d, -10d},
                           {  3d, -10d,  6d,  -7d},
                           {  6d,  -7d,  6d,  -4d},
                           {  6d,  -4d,  3d,  -1d},
                           { -3d,  -1d, -6d,  -4d},
                           { -6d,  -4d, -6d,  -7d},
                           { -6d,  -7d, -3d, -10d},
                           { -6d,  -1d,  6d,  -1d},
                           {  6d,  -1d,  6d,   4d},
                           {  6d,   1d,  8d,   7d},
                           {  4d,   4d,  8d,   7d},
                           {  8d,   7d,  8d,  10d},
                           {  6d,  10d, 10d,  10d},
                           {  6d,   4d, -6d,   4d},
                           { -6d,  -1d, -6d,   4d},
                           { -6d,   1d, -8d,   7d},
                           { -4d,   4d, -8d,   7d},
                           { -8d,   7d, -8d,  10d},
                           {-10d,  10d, -6d,  10d},
                           {  0d,   4d,  2d,   7d},
                           {  0d,   4d, -2d,   7d},
                           { -2d,   7d,  2d,   7d}
                          };

  int [][] lastScreenDelta = new int [landerArray.length][4];

  protected Lander () {}

  protected Lander (JDLunarLander gameIn, float fuelIn, int startXPosIn, int startYPosIn, double startDeltaXIn, 
                 double startDeltaYIn) 
  {
    saveTimeMainThrusterSoundStarted =0L;;
    fuel = fuelIn;
    game = gameIn;
    landerLines = new Vector(1);
    for (int i = 0; i < landerArray.length; i++)  {
      landerLines.addElement( new Line(
                                      landscapePosX + landerArray[i][0],
                                      landscapePosY + landerArray[i][1],
                                      landscapePosX + landerArray[i][2],
                                      landscapePosY + landerArray[i][3]  )  );
    }
    attitude =0;
    mainRocketOperating = false;
    landscapePosX  = startXPosIn;
    landscapePosY  = startYPosIn;
    deltaX = startDeltaXIn;
    deltaY = startDeltaYIn;
    lastScale = 0;
    mainRocketOperating = false;
  }


 /**
  * change state to another object
  */
 public void changeStateTo(String objectToChangeTo)   {
    if (objectToChangeTo.equals("ExplodingLander") )
      game.changeLander(new ExplodingLander());  
    if (objectToChangeTo.equals("LandedLander") )
      game.changeLander(new LandedLander());  
    if (objectToChangeTo.equals("FlyingLander") )
      game.changeLander(new FlyingLander());  
 }

  public void tick()  {}

  public void move()  {} 

  public void keyDown(Event evt, int key) {}
 

}

