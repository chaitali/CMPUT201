import java.awt.*;
import java.util.*;

public class LandingPadLine extends Line
{
  int basePointsScored;       //How many Points landing on this Landing Pad is worth
  int pointsScored;           //How many Points landing on this Landing Pad is worth
  int xIndentPointsScored;    //The X offset required to center the label under the Pad
  boolean landedHereAlready;

  public LandingPadLine (double startXIn, double startYIn, double endXIn, double endYIn,
                         double pointsScoredIn, Graphics graphicsIn)
   {
    super(startXIn, startYIn, endXIn, endYIn);
    basePointsScored = (int)pointsScoredIn;
    pointsScored = (int)pointsScoredIn;
    FontMetrics fm = graphicsIn.getFontMetrics( new Font("Helvetica", Font.PLAIN, 16)  );
    xIndentPointsScored = fm.stringWidth(Integer.toString(pointsScored))/2;
    landedHereAlready = false;
   }


 public void drawLine(Graphics graphicsIn)
   {
    if (visible)
     {
      graphicsIn.setColor(Color.cyan);
      graphicsIn.drawLine(screenStartX, screenStartY, screenEndX, screenEndY);
      graphicsIn.setColor(Color.yellow);
      graphicsIn.drawString(Integer.toString(pointsScored), screenStartX +
                            ((screenEndX - screenStartX)/2) - xIndentPointsScored,
                            screenStartY + 20);
     }
   }


  public void scalePoints(int multiplyPointsBy)
   {
    pointsScored = basePointsScored * multiplyPointsBy;
   }


}






