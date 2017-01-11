import java.awt.*;
import java.util.*;

public class LandingPads implements Drawable
 {
  Vector pads;
  JDLunarLander game;
  static double [][] landingPadArray = {
                              {   65d, 152d,  126d,  152d, 10d },
                              {  251d, 143d,  304d,  143d, 20d },
                              {  370d, 233d,  417d,  233d, 20d },
                              {  476d, 163d,  521d,  163d, 25d },
                              {  644d, 128d,  723d,  128d, 30d },
                              {  772d, 238d,  828d,  238d, 35d },
                              {  863d, 105d,  897d,  105d, 40d },
                              {  936d, 209,  988d,   209d, 45d },
                              { 1026d,  89d, 1090d,   89d, 50d },
                              { 1126d, 233d, 1165d,  233d, 100d},
                              { 1278d, 126d, 1316d,  126d, 60d },
                              { 1435d, 125d, 1468d,  125d, 65d }
                             };

  public LandingPads(Graphics graphicsIn, JDLunarLander gameIn)
    {
     game = gameIn;
     pads = new Vector();

     for (int i = 0; i < landingPadArray.length - 1; i++)
      {
       pads.addElement( new LandingPadLine(
                                  landingPadArray[i][0], landingPadArray[i][1],
                                  landingPadArray[i][2], landingPadArray[i][3],
                                  landingPadArray[i][4], graphicsIn )  );
      }

     //Test
     Enumeration enum = pads.elements();
    }

  public void draw(Graphics graphicsIn)
    {
     for (int ktr =0; ktr < pads.size(); ktr++ )
      {
        Line l = null;
        l = (Line)pads.elementAt(ktr);
        l.calculateScreenCoordinates(game.xDisplacement, game.yDisplacement, game.scale);
        l.drawLine(graphicsIn);
      }
    }

  public void scaleLandingPadPoints(int multiplyPointsBy)
   {
    if (pads == null) return;  
     for (int ktr =0; ktr < pads.size(); ktr++ )
      {
         LandingPadLine lp = (LandingPadLine)pads.elementAt(ktr);
         lp.scalePoints(multiplyPointsBy);
      }
   }

  public void resetPoints()
   {
    if (pads == null) return;  
     for (int ktr =0; ktr < pads.size(); ktr++ )
      {
         LandingPadLine lp = (LandingPadLine)pads.elementAt(ktr);
         lp.landedHereAlready = false;
      }
   }


}
