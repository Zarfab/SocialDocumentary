public class EnrichmentImage {
    private PImage img;
    private String txt;
    private ArrayList<String> tags;
    private int borderSize = 15;
    PFont typewriter;
    float rotation;
    
    public EnrichmentImage(String imagePath, String textPath, String monotag) {
      tags = new ArrayList<String>();
      tags.add(monotag);
      
      img = loadImage(imagePath);
      
      txt = "";
      String[] textMultiLines = loadStrings(textPath);
      for(int i=0; i<textMultiLines.length; i++) {
        txt += textMultiLines[i] + " ";
      }
      
      typewriter = loadFont("CourierNewPS-BoldMT-48.vlw");
      rotation = PI*random(-0.10, 0.10);
    }
    
    public void drawAsPolaroid(int cx, int cy, int w) {
      pushMatrix();
      translate(cx, cy);
      rotate(rotation);
      
      fill(225, 225, 200);
      noStroke();
      
      // cut the string into smaller pieces
      // and display them on several lines
      ArrayList<String> lines = cutString(txt, 30);
      int linesNb = lines.size();
      int maxSize = 10;
      if(linesNb > maxSize) {
        linesNb = 10;
      }
      textAlign(CENTER);
      float tSize = w/16;
      
      // draw a clear rectangle for "polaroid" effect
      rect(-w/2 - borderSize, -w/2 - borderSize, w+2*borderSize, w+2*borderSize+linesNb*tSize);
      
      // draw the image
      imageMode(CENTER);
      image(img, 0, 0, w, w);
            
      // print the text
      if(tags.get(0).equals("artist"))
        fill(106, 63, 6);
      else if(tags.get(0).equals("police"))
        fill(17, 17, 179);
      else if(tags.get(0).equals("politic"))
        fill(79, 33, 51);
      else if(tags.get(0).equals("protestors"))
        fill(50, 100, 12);
      else
        fill(0);

      textFont(typewriter, tSize);
      for(int i=0; i<lines.size() && i<maxSize; i++) {
        text(lines.get(i), 0, w/2+borderSize+(i+0.3)*tSize+4);
      }
      
      popMatrix();
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
