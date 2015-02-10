import signal.library.*;

public class KeywordVisual {
  
  float imageSize = 240; //initial image size;
  color activeColor;
  color strokeColor; 
  String keyword;
  int tagId;
  boolean isActive = false;
  PFont YaHei_UI_Bold;
  float fontSize = 22.0f;
  
  PVector rawCoord;
  PVector filteredCoord;
  PApplet myParent;
  
  int millisSinceLastDraw = 0; 
  int lastDrawTimeout = 1500;
  
  // -----------------------------------------------------
  // Create the filter
   SignalFilter myFilter;
  // Parameters of the OneEuroFilter
  float freq      = 120.0;
  float minCutoff = 0.05; // decrease this to get rid of slow speed jitter
  float beta      = 6.0;  // increase this to get rid of high speed lag
  float dcutoff   = 1.0;
  // -----------------------------------------------------
  
  public KeywordVisual(PApplet parent, color theActiveColor, String theKeyword, int id, float scale_factor) {
    strokeColor = color(191, 191, 191);
    activeColor = theActiveColor;
    
    imageSize *= scale_factor;
    fontSize *= scale_factor;
    
    YaHei_UI_Bold = loadFont("MicrosoftYaHeiUI-Bold-48.vlw");
    keyword = theKeyword;
    tagId = id;
    
    myParent = parent;
    myFilter = new SignalFilter(myParent, 3);
    // Pass the parameters to the filter
    myFilter.setFrequency(freq);
    myFilter.setMinCutoff(minCutoff);
    myFilter.setBeta(beta);
    myFilter.setDerivateCutoff(dcutoff);
    
    rawCoord = new PVector(0.0, 0.0, 0.0);
    filteredCoord = new PVector(0.0, 0.0, 0.0);
  }
  
  public int getTagId() {
    return tagId;
  }
  
  public void clearFilters() {
    myFilter = new SignalFilter(myParent, 3);
    // Pass the parameters to the filter
    myFilter.setFrequency(freq);
    myFilter.setMinCutoff(minCutoff);
    myFilter.setBeta(beta);
    myFilter.setDerivateCutoff(dcutoff);
  }
  
  public void setActive(boolean state) {
    isActive = state;
  }
  public void toogleActive() {
    isActive = !isActive;
  }
  
  public void checkLastDraw() {
    if(millis()-millisSinceLastDraw > lastDrawTimeout) {
      clearFilters();
    }
  }
  
  public void pushXYA(float newX, float newY, float newA, PGraphics screen) {  
    rawCoord = new PVector(newX, newY, newA);
    filteredCoord = myFilter.filterCoord3D( rawCoord, screen.width, screen.height, TWO_PI );
    //println(rawCoord.z, filteredCoord.z);
  }
  
  public void drawRow(PGraphics screen) {
    float x = rawCoord.x;
    float y = rawCoord.y;
    float a = rawCoord.z;
    
    screen.ellipseMode(CENTER);
    screen.pushMatrix();
       screen.translate(x, y);
       screen.rotate(a);
       screen.smooth(4);
       drawOnPGraphics(screen);
    screen.popMatrix();
  }
  
  public void drawFiltered(PGraphics screen) {
    float x = filteredCoord.x;
    float y = filteredCoord.y;
    float a = filteredCoord.z;
   
    screen.ellipseMode(CENTER);
    screen.pushMatrix();
       screen.translate(x, y);
       screen.rotate(a);
       screen.smooth(4);
       drawOnPGraphics(screen);
    screen.popMatrix();
  }
  
  private void drawOnPGraphics(PGraphics screen) {
    float bigRadius = imageSize;
    float angleBeg = 0.5 + 0.02 * keyword.length();
    float angleEnd = 0.5 - 0.02 * keyword.length();

    screen.stroke(strokeColor);
    screen.strokeWeight(2.5);
    screen.noFill();
    if(isActive) {
       screen.stroke(activeColor);
       screen.strokeWeight(bigRadius*0.06);
       screen.strokeCap(SQUARE);
       screen.arc(0, 0, bigRadius*0.94, bigRadius*0.94, PI*(angleBeg), PI*(2+angleEnd), OPEN);
       screen.strokeWeight(2.5);
       screen.stroke(strokeColor);
     }
     screen.arc(0, 0, bigRadius, bigRadius, PI*angleBeg, PI*(2+angleEnd), OPEN);
     screen.arc(0, 0, bigRadius*0.88, bigRadius*0.88, PI*angleBeg, PI*(2+angleEnd), OPEN);
     screen.pushMatrix();
       screen.rotate(PI*angleBeg);
       screen.line(bigRadius*0.5, 0, bigRadius*0.44, 0);
     screen.popMatrix();
     screen.pushMatrix();
       screen.rotate(PI*angleEnd);
       screen.line(bigRadius*0.5, 0, bigRadius*0.44, 0);
     screen.popMatrix();
     screen.noStroke();
     if(isActive) {
       screen.fill(activeColor);
     }
     else {
       screen.fill(strokeColor);
     } 
     screen.ellipse(0, 0, bigRadius*0.275, bigRadius*0.275);
     screen.fill(255);
     screen.textFont(YaHei_UI_Bold);
     screen.textSize(fontSize);
     float keywordWidth = screen.textWidth(keyword);
     screen.textAlign(LEFT);           
     screen.rotate((angleEnd-angleBeg+0.02)/2*PI);
     for(int c = keyword.length()-1; c >= 0; c--) {
       char letter = keyword.charAt(c);
       float angle = PI * (angleBeg-angleEnd) * (screen.textWidth(letter)) / (keywordWidth + screen.textWidth('\''));
       screen.rotate(angle);
       screen.text(letter , 0, bigRadius*0.495);
     }  
  }
  
}
