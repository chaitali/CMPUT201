import java.awt.*;
import java.util.*;

public class LandedLander extends Lander
{

  LandedLander ()
   {
    mainRocketOperating = false;
    game.messageVector.addElement("Landed Successfully !");
    game.messageVector.addElement("Press T to Take Off Again");
    game.saveTimeWeLandedOrCrashed = System.currentTimeMillis(); 
   }


 public void draw(Graphics graphicsIn )
   {
    Line currentElement;
    screenPosX  = (int) ((landscapePosX - game.xDisplacement) * game.scale);
    screenPosY =  (int) ((landscapePosY - game.yDisplacement) * game.scale);

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
    lastScale = game.scale;
   }


 public void launchLander()
  {
   deltaY = -.15;
   changeStateTo("FlyingLander");
  }


  public void keyDown(Event evt, int key)
   {

    if (key == 'T' || key == 't') 
     {
       if ( System.currentTimeMillis() > (game.saveTimeWeLandedOrCrashed + 3000) )
       launchLander();
     }

   }

}
