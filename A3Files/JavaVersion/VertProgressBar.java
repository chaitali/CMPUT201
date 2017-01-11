import java.awt.*;

public class VertProgressBar extends Canvas{

    private int progressWidth;              // width of the progress bar
    private int progressHeight;             // height of the progress bar
    private float percentage;               // the percentage property used to updated the foreground
    private Image offscreenImg;             // the offscreen image
    private Graphics offscreenG;            // the offscreen images graphic context
    private Color progressColor = Color.red;        // the default foreground color
    private Color progressBackground = Color.white; // the default background color


/**
* Creates a progress bar using the passed width, height, canvas color,
* progress bar foreground color, and progress bar background color.
*/
    public VertProgressBar(int progressWidth, int progressHeight, Color canvasColor,
                        Color progressColor, Color progressBackground) {
        this.progressWidth = progressWidth;
        this.progressHeight = progressHeight;
        this.progressColor = progressColor;
        this.progressBackground = progressBackground;
        resize(progressWidth,progressHeight);
        setBackground(canvasColor);
    }


/**
* This method is called when another component wants to update the progress bar. A percentage in the form
* of a float between 0 and 1 is accepted, then the progress bars repaint method is called to paint the
* progress bar on its Canvas.
*/
    public void updateBar(float percentage) {

        setProgressColor(Color.green);
        if (percentage < .25f ) setProgressColor(Color.yellow);
        if (percentage < .15f ) setProgressColor(Color.red);


        this.percentage = percentage;
        repaint();

    }

/**
* Sets the background color of the canvas the progress bar is drawn on.
*/
  public void setCanvasColor(Color color) {

        setBackground(color);
    }

/**
* Sets the foreground color of the progress bar.
*/
  public void setProgressColor(Color progressColor) {

        this.progressColor = progressColor;
  }


/**
* Sets the background color of the progress bar.
*/
   public void setBackGroundColor(Color progressBackground) {

       this.progressBackground = progressBackground;
  }



  public void paint(Graphics g) {

   int width = 0;
   int height = 0;
   int inset = 4;

        offscreenImg = createImage(progressWidth-inset, progressHeight-inset);
        offscreenG = offscreenImg.getGraphics();

        width =  offscreenImg.getWidth(this);
        height = offscreenImg.getHeight(this);

        offscreenG.setColor(progressBackground);
        offscreenG.fillRect(0, 0, width, height);

        offscreenG.setColor(progressColor);
        offscreenG.fillRect(0,height-((int)(height*percentage)), width, (int)(height*percentage));

        g.setColor(progressBackground);
        g.draw3DRect(size().width/2-progressWidth/2, 0, progressWidth-1,progressHeight-1, false);

        g.drawImage(offscreenImg, size().width/2-progressWidth/2+inset/2,  inset/2, this);

   }

    public void update(Graphics g) {
        paint(g);
    }

} //end of vertprogressBar class

