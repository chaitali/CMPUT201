import java.util.*;
import java.awt.*;

class PlayingFieldCanvas extends Canvas
{

  private JDLunarLander game;
  Image bufferImage;
  Graphics bufferGraphics;
  Color blankingColor;

  PlayingFieldCanvas(JDLunarLander gameIn, Color blankingColorIn, int widthIn, int heightIn )
   {
    game = gameIn;
    bufferImage = game.createImage( widthIn, heightIn );
    bufferGraphics = bufferImage.getGraphics();
    blankingColor = blankingColorIn; 

    int bufferWidth  = bufferImage.getWidth(null);
    int bufferHeight = bufferImage.getHeight(null);
    this.resize(bufferWidth, bufferHeight);
    bufferGraphics = bufferImage.getGraphics();

   }


  //Overide the default update method to avoid it blanking the screen and causing flickering
  public void update(Graphics g)
   {
    paint(g);
   }

  public void paint(Graphics g)
   {
     bufferGraphics.setColor(blankingColor);
     bufferGraphics.fillRect(0, 0, bounds().width, bounds().height);

     for ( int i =0; i < game.thingsToDraw.size(); i++ )
       {
        ((Drawable)game.thingsToDraw.elementAt(i)).draw(bufferGraphics);
      }  

    g.drawImage(bufferImage, 0, 0, this);
   }

}
