import java.awt.*;


public class Line
{
  double startX;
  double startY;
  double endX;
  double endY;
  int screenStartX;
  int screenStartY;
  int screenEndX;
  int screenEndY;
  boolean visible;
  double deltaX;
  double deltaY;

  public Line (double startXIn, double startYIn, double endXIn, double endYIn)
   {
    startX = startXIn;
    startY = startYIn;
    endX = endXIn;
    endY = endYIn;
    visible = true;

   }

 public void calculateScreenCoordinates(double XDisplacementIn, double YDisplacementIn,
                                                                         double ScaleIn)
   {
    screenStartX = (int)((startX - XDisplacementIn) * ScaleIn);
    screenStartY = (int)((startY - YDisplacementIn) * ScaleIn);
    screenEndX   = (int)((endX - XDisplacementIn) * ScaleIn);
    screenEndY   = (int)((endY - YDisplacementIn) * ScaleIn);
   }


 public void drawLine(Graphics graphicsIn)
   {
    graphicsIn.setColor(Color.white);
    if (visible)
      graphicsIn.drawLine(screenStartX, screenStartY, screenEndX, screenEndY);
   }


 public void move()
   {
    startX += deltaX;
    startY += deltaY;
    endX += deltaX;
    endY += deltaY;
   }


  /**
   *
   */
  public boolean collisionDetect(Line otherLine )
   {

    float r,s;
    float ax;
    float ay;
    float bx;
    float by;
    float cx;
    float cy;
    float dx;
    float dy;

    ax = (float)startX;
    ay = (float)startY;
    bx = (float)endX;
    by = (float)endY;
    cx = (float)otherLine.startX;
    cy = (float)otherLine.startY;
    dx = (float)otherLine.endX;
    dy = (float)otherLine.endY;

    //detect intersections between perfectly horizontal lines
    if ( otherLine.startX >= startX &&
         otherLine.endX  <= endX &&
         (Math.abs(startY - otherLine.startY) < 1)  &&
         (Math.abs(endY - otherLine.endY) < 1 ) )
      return true;
    //detect intersections between perfectly vertical lines
    if ( otherLine.startY >= startY &&
         otherLine.endY <= endY &&
         (Math.abs(startX - otherLine.startX) < 1 ) &&
         (Math.abs(endX - otherLine.endX) < 1 )    )
      return true;

    //In order to avoid a division by zero error we will slightly slant any
    //perfectly vertical or horizontal lines.  This may make the intersection
    //detection too eager when dealing with long close together parrallel lines.
    if (ax == bx) ax += .01;
    if (cx == dx) cx += .01;
    if (ay == by) ay += .01;
    if (cy == dy) cy += .01;


    r = ( ((ay - cy) * (dx - cx)) - ((ax - cx) * (dy - cy)) ) /
        ( ((bx - ax) * (dy - cy)) - ((by - ay) * (dx - cx)) );

    s = ( ((ay - cy) * (bx - ax)) - ((ax - cx) * (by - ay)) ) /
        ( ((bx - ax) * (dy - cy)) - ((by - ay) * (dx - cx)) );

    if ( r<0 || r>1 || s<0 || s>1 )
      return false;
    else
      return true;
   }





}
