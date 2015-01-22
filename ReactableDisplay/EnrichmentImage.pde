public class EnrichmentImage {
    private PImage img;
    private String txt;
    private String path;
    private ArrayList<String> tags;
    private int borderSize = 15;
    PFont typewriter;
    float rotation;
    
    public EnrichmentImage(String imagePath, String textPath, String monotag) {
      tags = new ArrayList<String>();
      tags.add(monotag);
      
      path = imagePath;
      
      
      txt = "";
      String[] textMultiLines = loadStrings(textPath);
      for(int i=0; i<textMultiLines.length; i++) {
        txt += textMultiLines[i] + " ";
      }
      
      typewriter = loadFont("CourierNewPS-BoldMT-48.vlw");
      rotation = PI*random(-0.10, 0.10);
    }
    
    public void loadImageFromFile() {
      img = loadImage(path);
    }
    
    public void freeImage() {
      img = null;
    }
    
    public void drawAsPolaroid(PGraphics screen, int cx, int cy, int w) {
      screen.pushMatrix();
      screen.translate(cx, cy);
      screen.rotate(rotation);
      
      screen.fill(180, 180, 160);
      screen.noStroke();
      
      // cut the string into smaller pieces
      // and display them on several lines
      ArrayList<String> lines = cutString(txt, 20);
      int linesNb = lines.size();
      int maxSize = 10;
      if(linesNb > maxSize) {
        linesNb = 10;
      }
      screen.textAlign(CENTER);
      float tSize = w/10;
      
      // draw a clear rectangle for "polaroid" effect
      screen.rect(-w/2 - borderSize, -w/2 - borderSize, w+2*borderSize, w+2*borderSize+linesNb*tSize);
      
      //draw the image
      screen.imageMode(CENTER);
      screen.image(img, 0, 0, w, w);
            
      // print the text
      if(tags.get(0).equals("artist"))
        screen.fill(106, 63, 6);
      else if(tags.get(0).equals("police"))
        screen.fill(17, 17, 179);
      else if(tags.get(0).equals("politic"))
        screen.fill(79, 33, 51);
      else if(tags.get(0).equals("protestors"))
        screen.fill(50, 100, 12);
      else
        screen.fill(0);

      screen.textFont(typewriter, tSize);
      for(int i=0; i<lines.size() && i<maxSize; i++) {
        screen.text(lines.get(i), 0, w/2+borderSize+(i+0.3)*tSize+4);
      }
      
      screen.popMatrix();
    }
    
    // used to cut too long strings in several lines
    private ArrayList<String> cutString(String text, int maxLength) {
      ArrayList<String> pieces = new ArrayList<String>();
      String toCut = new String(text);
      int indexToCut = maxLength;
      while(toCut.length() > maxLength && indexToCut > 0) {
        indexToCut = maxLength;      
        while(toCut.charAt(indexToCut) != ' ' && indexToCut > 0)
          indexToCut--;
      
        pieces.add(toCut.substring(0, indexToCut));
        toCut = toCut.substring(indexToCut);
      }
      pieces.add(toCut);    
      return pieces;
    }
}
