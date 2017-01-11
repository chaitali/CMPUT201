import java.awt.*;
import java.util.*;

class Landscape implements Drawable
{
   JDLunarLander game;
   Vector slopes;
   double [][] landscapeArray = {
                              {-1000d,-1000d},
                              {-1000d,-100d},
                              {    0d,   0d},
                              {   42d, 188d},
                              {   65d, 153d},
                              {  126d, 153d},
                              {  136d, 167d},
                              {  186d, 224d},
                              {  251d, 144d},
                              {  304d, 144d},
                              {  307d,  88d},
                              {  335d,  70d},
                              {  370d, 234d},
                              {  417d, 234d},
                              {  467d, 146d},
                              {  476d, 164d},
                              {  521d, 164d},
                              {  536d, 144d},
                              {  593d, 185d},
                              {  644d, 129d},
                              {  723d, 129d},
                              {  772d, 239d},
                              {  828d, 239d},
                              {  863d, 106d},
                              {  897d, 106d},
                              {  936d, 210d},
                              {  988d, 210d},
                              { 1026d,  90d},
                              { 1090d,  90d},
                              { 1133d, 155d},
                              { 1143d, 184d},
                              { 1120d, 216d},
                              { 1126d, 234d},
                              { 1165d, 234d},
                              { 1191d, 184d},
                              { 1227d, 141d},
                              { 1278d, 127d},
                              { 1316d, 127d},
                              { 1340d, 161d},
                              { 1435d, 126d},
                              { 1468d, 126d},
                              { 1494d,   0d},
                              { 2500d,-100d},
                              { 2500d,-1000d}
                             };


  public Landscape(JDLunarLander gameIn)
    {
      game = gameIn;
      slopes = new Vector(100);

      for (int i = 0; i < landscapeArray.length - 1; i++)
        {
         slopes.addElement( new Line(
                                    landscapeArray[i][0],   landscapeArray[i][1],
                                    landscapeArray[i+1][0], landscapeArray[i+1][1]  )  );
        }
    }
   
  public void draw(Graphics graphicsIn)
    {
     for (int ktr =0; ktr < slopes.size(); ktr++ )
      {
       Line l = null;
       l = (Line)slopes.elementAt(ktr);
       l.calculateScreenCoordinates( game.xDisplacement, game.yDisplacement, game.scale);
       l.drawLine(graphicsIn);
      }
    }

}



