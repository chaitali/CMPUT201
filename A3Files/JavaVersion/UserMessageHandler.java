import java.awt.*;

class UserMessageHandler implements Drawable
 {
   JDLunarLander game;
   Font f;
   FontMetrics fm;
  
   public  UserMessageHandler(JDLunarLander gameIn, Font fontIn, FontMetrics fontMetricsIn)
     {
      game = gameIn;
      f = fontIn;
      fm = fontMetricsIn;
     }

   public void draw(Graphics graphicsIn)   
    {
     if ( !game.messageVector.isEmpty() )
       {
        if (  System.currentTimeMillis() > (game.saveTimeWeLandedOrCrashed + 3000) ) 
          {
           graphicsIn.setFont(f);
           for ( int i = 0; i < game.messageVector.size(); i++ )
             {
              graphicsIn.drawString( ((String)game.messageVector.elementAt(i)),
              (game.screenWidth - (fm.stringWidth( ((String)game.messageVector.elementAt(i)) ))) /2 ,
              100 + (i*30)  );
             }
          }
       }     
    }
 
 }
