import java.awt.*;
import java.util.*;

public class FlyingLander extends Lander
{
  private long saveTimeMainRocketOn;
  private long saveTimeMainThrusterSoundStarted;

  FlyingLander () 
   {
    game.messageVector.removeAllElements();
    spinLanderLeft();
    spinLanderRight();
   }

  public FlyingLander (JDLunarLander gameIn, float fuelIn, int startXPosIn, int startYPosIn, double startDeltaXIn,
                       double startDeltaYIn)
   {
    super( gameIn, fuelIn, startXPosIn, startYPosIn, startDeltaXIn, startDeltaYIn);
    mainRocketOff();
    spinLanderLeft();
    spinLanderRight();
   }

 public void tick()
   {
    move();
    overLandingPadDetect();
    landingDetect();
    collisionDetect();
    handleMainRocket();
    handleMainThrusterSound();
   }

 public void move()
   {
    Line currentElement;

    deltaY += gravityAcceleration;
    if (mainRocketOperating)
     {  
      deltaX -= xDirection * rocketAcceleration;
      deltaY -= yDirection * rocketAcceleration;
      fuel-=.001;
      if ( fuel < 0)
        {
         mainRocketOperating = false;
         mainRocketOff();
        }
     }

    landscapePosX += deltaX;
    landscapePosY += deltaY;

    for (int i =0; i < landerLines.size(); i++ )
     {
      ((Line)landerLines.elementAt(i)).deltaX = deltaX;
      ((Line)landerLines.elementAt(i)).deltaY = deltaY;
      ((Line)landerLines.elementAt(i)).move();
     }
 }



 public void draw(Graphics graphicsIn)
   {
    Line currentElement;
    screenPosX  = (int) ((landscapePosX - game.xDisplacement) * game.scale);
    screenPosY =  (int) ((landscapePosY - game.yDisplacement) * game.scale);

    if ( landerSpun  || game.scale != lastScale)
     {
      landerSpun = false;
      for (int ktr =0; ktr < landerLines.size(); ktr++ )
       {
        currentElement = (Line)landerLines.elementAt(ktr);
        currentElement.calculateScreenCoordinates( game.xDisplacement, game.yDisplacement, game.scale);
        currentElement.drawLine(graphicsIn);
        lastScreenDelta[ktr][0] = currentElement.screenStartX - screenPosX;
        lastScreenDelta[ktr][1] = currentElement.screenStartY - screenPosY;
        lastScreenDelta[ktr][2] = currentElement.screenEndX   - screenPosX;
        lastScreenDelta[ktr][3] = currentElement.screenEndY   - screenPosY;
       }
     }
    else
     {
      for (int ktr =0; ktr < landerLines.size(); ktr++ )
       {
        currentElement = (Line)landerLines.elementAt(ktr);
        currentElement.screenStartX = lastScreenDelta[ktr][0] + screenPosX;
        currentElement.screenStartY = lastScreenDelta[ktr][1] + screenPosY;
        currentElement.screenEndX   = lastScreenDelta[ktr][2] + screenPosX;
        currentElement.screenEndY   = lastScreenDelta[ktr][3] + screenPosY;
        currentElement.drawLine(graphicsIn);
       }
     }
    lastScale = game.scale;
   }





 public void mainRocketOff()
   {
    mainRocketOperating = false;
    //First two lines are flame from rocket make them invisible
    ( (Line)(landerLines.elementAt(0)) ).visible = false;
    ( (Line)(landerLines.elementAt(1)) ).visible = false;
   }



 public void mainRocketOn()
   {
    if ( fuel > 0 )
     {
      mainRocketOperating = true;
      //First two lines are flame from rocket, make them visible
      ( (Line)(landerLines.elementAt(0)) ).visible = true;
      ( (Line)(landerLines.elementAt(1)) ).visible = true;
     }
   }


 public void spinLanderLeft()
   {
    attitude += 1;
    if (attitude > 23) attitude =0;
    rotate();
    landerSpun = true;
   }

 public void spinLanderRight()
   {
    attitude -= 1;
    if (attitude < 0) attitude = 23;
    rotate();
    landerSpun = true;
   }


 public void rotate()
   {
    double radian;
    double totalDirection;
    Line l;
    double cos;
    double sin;
    double negativeSin;

    radian = attitude * 0.2617993877991d;  //15 degrees

    cos = Math.cos(radian);
    sin = Math.sin(radian);
    negativeSin = sin * -1;

    totalDirection = Math.abs(sin) + Math.abs(cos);
    yDirection =  cos / totalDirection;
    xDirection =  sin / totalDirection;

    for (int i =0; i < landerLines.size(); i++ )
     {

      l = (Line)landerLines.elementAt(i);

      l.startX = landscapePosX + ( (cos * landerArray[i][0]) + (sin * landerArray[i][1]) );
      l.startY = landscapePosY + ( (negativeSin * landerArray[i][0]) + (cos * landerArray[i][1]) );
      l.endX = landscapePosX   + ( (cos * landerArray[i][2]) + (sin * landerArray[i][3]) );
      l.endY = landscapePosY   + ( (negativeSin * landerArray[i][2]) + (cos * landerArray[i][3]) );

     }


   }


/**
 * Collision Detect
 */
 public void collisionDetect()
  {
   Line l;
   for (int ktr =0; ktr < landerLines.size(); ktr++ )
    {
     for (int ktr2 =0; ktr2 < game.theLandscape.slopes.size(); ktr2++ )
      {
        l = (Line)game.theLandscape.slopes.elementAt(ktr2);
        if (l.collisionDetect( (Line)landerLines.elementAt(ktr) ) )
         {
          changeStateTo("ExplodingLander");
          return;
         }
      }
    }
  }

/**
 * Landing Detect
 */
 public void landingDetect()
  {
   LandingPadLine lp;
   //Landing Detect
   if ( attitude != 0) return;       //Cant land unless were perfectly level
   //Cant land if were going too fast vertically or horizontally
   if ( (deltaY > maxSafeLandingSpeed ) || ( Math.abs(deltaX) > maxSafeLandingSpeed) ) return;

   //If all the above conditions are OK, then an intersection of the Landers landing gear feet with a
   //Landing Pad line means a good Landing
   for (int ktr =0; ktr < game.theLandingPads.pads.size(); ktr++ )
    {
     lp = (LandingPadLine)game.theLandingPads.pads.elementAt(ktr);
     if (  lp.collisionDetect((Line)landerLines.elementAt(20)) &&
           lp.collisionDetect((Line)landerLines.elementAt(14))    )
      {
       game.saveTimeWeLandedOrCrashed = System.currentTimeMillis();
       if ( lp.landedHereAlready )
         game.pointsMessage = "Landed here already, No Points";
       else
        {
         fuel = fuel + .5f;
         if (fuel > 1f) fuel = 1f;
         game.landingSound.play();
         lp.landedHereAlready = true;
         game.score = game.score + lp.pointsScored;
         game.displayScore();
         game.pointsMessage = "Scored " +Integer.toString(lp.pointsScored) + " points";
         changeStateTo("LandedLander");
        }
       break;
      }
    }

  }
 
/**
 * Over Landing Pad Detect
 */
 public void overLandingPadDetect()
  {
   Line l;

   game.zoomed = false;
   for (int ktr =0; ktr < game.theLandingPads.pads.size(); ktr++ )
    {
     l = (Line)game.theLandingPads.pads.elementAt(ktr);
     if ( (landscapePosX > (l.startX - 25)) &&
          (landscapePosX < (l.endX + 25)) &&
          (landscapePosY > (l.startY - 50)) &&
          (landscapePosY < l.endY)  )
       {
         game.zoomed = true;
         continue;
       }
    }
  }
   
  public void handleMainRocket()
   {
     if (System.currentTimeMillis() >  saveTimeMainRocketOn + 300 ) 
     mainRocketOff();
   }


  public  void handleMainThrusterSound()
   {
     if ( mainRocketOperating)
       {
        if ( System.currentTimeMillis() > saveTimeMainThrusterSoundStarted + 5000)
          {
           game.mainThrusterSound.play();
           saveTimeMainThrusterSoundStarted = System.currentTimeMillis();
          }
       }
     else
      game.mainThrusterSound.stop();
  }


/**
 *
 */
  public void keyDown(Event evt, int key)
   {

    if ( key == ' ')
     {
      throw   
      System.out.println("spc");
      saveTimeMainRocketOn = System.currentTimeMillis();
      mainRocketOn();
     }

    if ( key == Event.LEFT)
     {
      spinLanderLeft();
      game.attitudeThrusterSound.play();
     }

     if ( key == Event.RIGHT)
     {
      spinLanderRight();
      game.attitudeThrusterSound.play();
     }

   }


}
