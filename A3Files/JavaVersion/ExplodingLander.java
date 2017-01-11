import java.awt.*;
import java.util.*;

public class ExplodingLander extends Lander
{

   ExplodingLander ()
    {
     mainRocketOperating = false;
     game.saveTimeWeLandedOrCrashed = System.currentTimeMillis();
     game.messageVector.addElement("Press any key to play again");
     setExplosionDeltas();
     game.crashSound.play();
     game.saveTimeWeLandedOrCrashed = System.currentTimeMillis(); 
    }  

 public void tick()
   {
    move();
   }

 public void move()
   {
    Line currentElement;

    deltaY += gravityAcceleration;

    for (int i =0; i < landerLines.size(); i++ )
     {
      ((Line)landerLines.elementAt(i)).deltaY += gravityAcceleration;
      ((Line)landerLines.elementAt(i)).move();
     }
   }


 public void draw(Graphics graphicsIn)
   { Line currentElement;                                                              
    screenPosX  = (int) ((landscapePosX - game.xDisplacement) * game.scale);                     
    screenPosY =  (int) ((landscapePosY - game.yDisplacement) * game.scale);

    for (int ktr =0; ktr < landerLines.size(); ktr++ )
     {
      currentElement = (Line)landerLines.elementAt(ktr);
      currentElement.calculateScreenCoordinates( game.xDisplacement, game.yDisplacement, game.scale);
      currentElement.drawLine(graphicsIn);
     }
    lastScale = game.scale;
   }


 public void setExplosionDeltas()
   {
    Random generator = new Random();
    double r;
    //Increase gravity to make debris curve down more dramtically
    gravityAcceleration *= 10d;

    generator.setSeed(System.currentTimeMillis());
    for (int ktr =0; ktr < landerLines.size(); ktr++ )
     {
      r = generator.nextDouble();
      r -= .5d;
      r *= 3d;
      ((Line)landerLines.elementAt(ktr)).deltaX += r;
      ((Line)landerLines.elementAt(ktr)).deltaX *= -1d;
      r = generator.nextDouble();
      r -= .5d;
      r *= 3d;
      ((Line)landerLines.elementAt(ktr)).deltaY += r;
     ((Line)landerLines.elementAt(ktr)).deltaY *= -1d;
     }
    deltaX =0d;
    deltaY =0d;
   }

  public void keyDown(Event evt, int key)
   {
    if ( System.currentTimeMillis() > (game.saveTimeWeLandedOrCrashed + 3000) )
       game.newGame();
   }



}
