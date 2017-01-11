/**
 * @JDLunarLander.java
 * @version     1.3, 01 Jan 1999
 * @author: John Donohue
 *
 *  This is the classic Lunar Lander Game.
 *
 * Example of APPLET code:
 * <APPLET CODE="JDLunarLander.class" WIDTH=600 HEIGHT=250>
 * </APPLET>
 *
 */


import java.awt.*;
import java.applet.*;
import java.util.*;
import java.net.*;

/**
 * The Applet.
 */
public class JDLunarLander extends Applet implements Runnable
{

 private Image bufferImage;
 private PlayingFieldCanvas playFieldCanvas;
 private Graphics bufferGraphics;
 private Thread animate;
 private boolean runAnimation;
 private int frameRate;
 private int rightScrollEdge;
 private int leftScrollEdge;
 private int topScrollEdge;
 private int bottomScrollEdge;
 private int leftScreenEdge;
 private int rightScreenEdge;
 private int topScreenEdge;
 private int bottomScreenEdge;
 private Panel playFieldPanel;
 private Panel controlPanel;
 private VertProgressBar fuelGuage;
 private Panel gravitySettingPanel;
 private CheckboxGroup gravitySettingRadio;
 private Checkbox moonGravityRadio, earthGravityRadio, jupiterGravityRadio;
 private Label lScore;
 long saveTimeWeLandedOrCrashed;
 Line l;
 LandingPadLine lp;
 Label l3, l4, l5;
 String pointsMessage;
 double scale;
 double xDisplacement;
 double yDisplacement;
 Lander theLander;
 LandingPads theLandingPads;
 Landscape theLandscape;
 UserMessageHandler theUserMessageHandler;
 int screenWidth;
 int screenHeight;
 int score;
 boolean zoomed;
 Vector messageVector;
 Vector thingsToDraw; 

 static AudioClip attitudeThrusterSound;
 static AudioClip crashSound;
 static AudioClip landingSound;
 static AudioClip mainThrusterSound;

 private Panel p1;

/**
 * The Applet.
 */
 public void init()
  {
    playFieldCanvas = new PlayingFieldCanvas(this, Color.black, bounds().width - 50, bounds().height);
    
    Font messagesFont  = new Font ( "Helvetica", Font.PLAIN, 20);
    FontMetrics messagesFontMetrics = playFieldCanvas.bufferGraphics.getFontMetrics(messagesFont);

    Font f1 = new Font ( "Helvetica", Font.PLAIN, 12);
    Font f2 = new Font ( "Helvetica", Font.BOLD, 10);
    Font f3 = new Font ( "Helvetica", Font.PLAIN, 9);
    Font f4 = new Font ( "Helvetica", Font.BOLD, 16);
    Font f5 = new Font ( "Helvetica", Font.BOLD, 12);
    Font f6 = new Font ( "Helvetica", Font.BOLD, 14);

    lScore = new Label("0");
    lScore.setForeground(Color.red);
    lScore.setFont(f4);
    lScore.reshape(7,55,48,15);

    screenWidth  = playFieldCanvas.bufferImage.getWidth(null);
    screenHeight = playFieldCanvas.bufferImage.getHeight(null);
    leftScreenEdge =0;
    rightScreenEdge = screenWidth;
    topScreenEdge = 0;
    bottomScreenEdge = screenHeight;
    rightScrollEdge = screenWidth -100;
    leftScrollEdge = 100;
    topScrollEdge = 50;
    bottomScrollEdge = screenHeight -50;

    setLayout( new BorderLayout() );

    controlPanel = new Panel();
    controlPanel.setBackground(Color.white);
    controlPanel.setLayout( null);
    controlPanel.resize(50,screenHeight);

    Label l1 = new Label("Lunar ", Label.LEFT);
    l1.setForeground(Color.black);
    l1.setFont(f5);
    l1.reshape(7,8,48,12);
    controlPanel.add(l1);

    Label l2 = new Label("Lander", Label.CENTER);
    controlPanel.add(l2);
    l2.setForeground(Color.black);
    l2.setFont(f5);
    l2.reshape(0,23,48,12);

    Label l6 = new Label("Score");
    l6.setFont(f6);
    l6.reshape(7,40,48,15);
    controlPanel.add(l6);
    controlPanel.add(lScore);

    Label lFuel = new Label("Fuel");
    lFuel.setFont(f4);
    lFuel.reshape(10,79,48,15);
    controlPanel.add(lFuel);
    fuelGuage = new  VertProgressBar(20, 50, Color.gray, Color.green, Color.black);
    fuelGuage.reshape(15,95,20,50);
    controlPanel.add(fuelGuage);

    Label l7 = new Label("2Gravity");
    l7.setFont(f5);
    l7.reshape(5,161,48,12);
    controlPanel.add(l7);

    gravitySettingPanel = new Panel();
    gravitySettingPanel.setLayout( null);
    gravitySettingPanel.setBackground(Color.white);
    gravitySettingPanel.setForeground(Color.black);
    gravitySettingPanel.resize(50,70);
    Label gravityLabel = new Label("1Gravity");
    gravityLabel.setFont(f1);
    gravityLabel.reshape(4,0,46,12);
    gravityLabel.setForeground(Color.darkGray);
    gravitySettingRadio = new CheckboxGroup();
    moonGravityRadio    = new Checkbox("Moon", gravitySettingRadio, true);
    earthGravityRadio   = new Checkbox("Earth", gravitySettingRadio, false);
    jupiterGravityRadio = new Checkbox("Jupiter", gravitySettingRadio, false);
    moonGravityRadio.setFont(f3);
    earthGravityRadio.setFont(f3);
    jupiterGravityRadio.setFont(f3);
    gravitySettingPanel.add(moonGravityRadio);
    gravitySettingPanel.add(earthGravityRadio);
    gravitySettingPanel.add(jupiterGravityRadio);
    moonGravityRadio.reshape(2,0,48,12);
    earthGravityRadio.reshape(2,12,48,12);
    jupiterGravityRadio.reshape(2,24,48,12);
    controlPanel.add(gravitySettingPanel);
    gravitySettingPanel.reshape(0,175,50,70);

    l3 = new Label("John");
    l3.setForeground(Color.blue);
    l3.setFont(f3);
    l3.reshape(16,218,50,9);
    controlPanel.add(l3);
    l4 = new Label("Donohue's");
    l4.setFont(f3);
    l4.reshape(4,228,50,9);
    l4.setForeground(Color.blue);
    controlPanel.add(l4);
    l5 = new Label("Web Page");
    l5.setFont(f3);
    l5.reshape(4,238,50,9);
    l5.setForeground(Color.blue);
    controlPanel.add(l5);

    add("West", controlPanel);
    add("Center",playFieldCanvas);

    super.init();

    mainThrusterSound = null;
    attitudeThrusterSound = null;
    landingSound = null;
    crashSound = null;
    crashSound = getAudioClip( getDocumentBase(), "crash.au");
    crashSound.play();
    crashSound.stop();
    mainThrusterSound = getAudioClip( getDocumentBase(), "mainThruster.au");
    mainThrusterSound.play();
    mainThrusterSound.stop();
    attitudeThrusterSound = getAudioClip( getDocumentBase(), "attitudeThruster.au");
    attitudeThrusterSound.play();
    attitudeThrusterSound.stop();
    landingSound = getAudioClip( getDocumentBase(), "landed.au");
    landingSound.play();
    landingSound.stop();

    runAnimation = false;

    messageVector = new Vector();
    theLandscape   = new Landscape(this);
    theLandingPads = new LandingPads(playFieldCanvas.bufferGraphics, this);
    theUserMessageHandler = new UserMessageHandler(this, messagesFont, messagesFontMetrics);

    thingsToDraw = new Vector();
    newGame();

  }




 public void start()
  {
   playFieldCanvas.requestFocus();
   xDisplacement = 0d;
   yDisplacement = 0d;
   scale =1d;
   zoomed = false;

   initAnimation();
   if (animate == null)
    {
     animate = new Thread (this);
     animate.setPriority(Thread.currentThread().getPriority() - 1);
     animate.start();
    }

  }

  public void initThingsToDraw()
  {
    thingsToDraw.removeAllElements();
    thingsToDraw.addElement(theLandscape);
    thingsToDraw.addElement(theLandingPads);
    thingsToDraw.addElement(theLander);
    thingsToDraw.addElement(theUserMessageHandler);
  }

  public void initAnimation()
  {
    repaint();
  }



/**
 * Run
 */
  public void run()
  {

   for (;;)
    {
     fuelGuage.updateBar(theLander.fuel);

     if ( theLander.screenPosX > rightScrollEdge )
        xDisplacement = theLander.landscapePosX - ( rightScrollEdge / scale);
     if (theLander.screenPosX < leftScrollEdge  )
        xDisplacement = theLander.landscapePosX - (leftScrollEdge / scale);
     if ( theLander.screenPosY < topScrollEdge )
        yDisplacement = theLander.landscapePosY - (topScrollEdge / scale);
     if (theLander.screenPosY > bottomScrollEdge  )
        yDisplacement = theLander.landscapePosY - (bottomScrollEdge / scale);
 
     theLander.tick();  //Update the lander

     if (zoomed && scale == 1d) changeScale(2);
     if (!zoomed && scale == 2d) changeScale(1);

     playFieldCanvas.repaint();
     try { animate.sleep(50); } catch (InterruptedException e) {};

    }

  }


/**
 * stop
 */
 public void stop()
  {
    if (animate != null)
     {
       animate.stop();
       animate=null;
     }
  }

/**
 * update
 */
  public void update(Graphics g)
   {
    paint(g);
   }

  public void changeScale(double scaleIn)
   {
    scale = scaleIn;
    xDisplacement = theLander.landscapePosX - ( (screenWidth/2) / scale);
    yDisplacement = theLander.landscapePosY - ( (screenHeight/2) / scale);
   }

  //Pass all key events to the Lander to handle
  public boolean keyDown(Event evt, int key)
   {
    theLander.keyDown(evt, key);
    return true;
   }




/**
 *
 */
  public boolean mouseDown(Event evt, int x, int y)
   {
    //System.out.println("X=" + Integer.toString(x) + " Y=" + Integer.toString(y) );
    if ( y > 218 && x < 50 )
     {
      try
        {
         URL urlToGoto = new URL( getCodeBase(), "http://www.panix.com/~wizjd" );
         getAppletContext().showDocument(urlToGoto);
        }
       catch (MalformedURLException e)
        {
         e.printStackTrace();
        }
      }
    return false;
   }


/**
 *
 */
  public boolean action(Event evt, Object o)
   {
    if (evt.target instanceof Checkbox)
     {
      playFieldCanvas.requestFocus();
      newGame();
      start();
     }

    return false;
   }


 public void newGame()
  {
    score =0;
    displayScore();
    messageVector.removeAllElements();
    theLander = new FlyingLander(this, 1f, 90, 75, .1d, -.1d);
    theLandingPads.resetPoints();
    setLanderGravity();
    initThingsToDraw();
   }


 public void setLanderGravity()
  {
   theLander.gravityAcceleration = ((double).003); //Default
   if ( moonGravityRadio.getState() )
    {
     theLander.gravityAcceleration = ((double).003);
     theLandingPads.scaleLandingPadPoints(1);
    }
   if ( earthGravityRadio.getState() )
    {
     theLander.gravityAcceleration = ((double).006);
     theLandingPads.scaleLandingPadPoints(2);
    }
   if ( jupiterGravityRadio.getState())
    {
     theLander.gravityAcceleration = ((double).009);
     theLandingPads.scaleLandingPadPoints(3);
    }
  }


 public void displayScore()
  {
   String tmpStr;
   tmpStr = "0000" + Integer.toString(score);
   lScore.setText(tmpStr.substring(tmpStr.length() - 4, tmpStr.length()) );
  }

 public void changeLander( Lander lIn)
  {
   theLander = lIn;
   initThingsToDraw();
  }


}
