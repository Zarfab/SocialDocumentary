public class KeywordImage {
  
  PImage imgActive, imgInactive;
  boolean isActive = false;
  FloatList imgX, imgY, imgA;
  float x, y, a;
  int filterSize = 5;
  int millisSinceLastDraw = 0; 
  int lastDrawTimeout = 1500;

  
  public KeywordImage(String imageActivePath, String imageInactivePath, float scale_factor) {
    imgActive = loadImage(imageActivePath);
    imgActive.resize((int)(imgActive.width*scale_factor), (int)(imgActive.height*scale_factor));
    imgInactive = loadImage(imageInactivePath);
    imgInactive.resize((int)(imgInactive.width*scale_factor), (int)(imgInactive.height*scale_factor));
    
    imgX = new FloatList();
    imgY = new FloatList();
    imgA = new FloatList();

  }
  
  public float getX() {
    return x;
  }
  public float getY() {
    return y;
  }
  public float getAngle() {
    return a;
  }
  
  public void clearFilters() {
    imgX.clear();
    imgY.clear();
    imgA.clear();
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
  
  public void pushXYA(float newX, float newY, float newA) {  
    imgX.append(newX);
    if(imgX.size() > filterSize)
      imgX.remove(0);
        
    imgY.append(newY);
    if(imgY.size() > filterSize)
      imgY.remove(0);
    
    imgA.append(newA);
    if(imgA.size() > filterSize)
      imgA.remove(0);
  }
  
  public void drawRow(PGraphics screen) {
    x = imgX.get(imgX.size()-1);
    y = imgY.get(imgY.size()-1);
    a = imgA.get(imgA.size()-1);
    screen.imageMode(CENTER);
    screen.pushMatrix();
       screen.translate(x, y);
       screen.rotate(a);
       screen.smooth(4);
       if(isActive)
         screen.image(imgActive, 0, 0);
       else
         screen.image(imgInactive, 0, 0);
    screen.popMatrix();
    millisSinceLastDraw = millis();
  }
  
  public void drawMeanFilter(PGraphics screen) {
    x = 0.0f;
    y = 0.0f;
    a = 0.0f;
    for(int i = 0; i < imgX.size() && i < imgY.size() && i < imgA.size(); i++) {
      x += imgX.get(i);
      y += imgY.get(i);
      a += imgA.get(i);
    }
    x /= imgX.size();
    y /= imgY.size();
    a /= imgA.size();
    
    screen.imageMode(CENTER);
    screen.pushMatrix();
       screen.translate(x, y);
       screen.rotate(a);
       screen.smooth(4);
       if(isActive)
         screen.image(imgActive, 0, 0);
       else
         screen.image(imgInactive, 0, 0);
    screen.popMatrix();
    millisSinceLastDraw = millis();
  }
  
  public void drawMedianFilter(PGraphics screen) {
    int medianIndex = 0;
    
    FloatList tempX = imgX.copy();
    tempX.sort();
    medianIndex = imgX.size() / 2;
    x = tempX.get(medianIndex);
    FloatList tempY = imgY.copy();
    tempY.sort();
    medianIndex = imgY.size() / 2;
    y = tempY.get(medianIndex);
    FloatList tempA = imgA.copy();
    tempA.sort();
    medianIndex = imgA.size() / 2;
    a = tempA.get(medianIndex);
    
    screen.imageMode(CENTER);
    screen.pushMatrix();
       screen.translate(x, y);
       screen.rotate(a);
       screen.smooth(4);
       if(isActive)
         screen.image(imgActive, 0, 0);
       else
         screen.image(imgInactive, 0, 0);
    screen.popMatrix();
    millisSinceLastDraw = millis();
  }
  
  public void drawWithTrace(PGraphics screen) {
    screen.imageMode(CENTER);
    for(int i = 0; i < imgX.size() && i < imgY.size() && i < imgA.size(); i++) {
      screen.pushMatrix();
      screen.translate(imgX.get(i), imgY.get(i));
      screen.tint(255, (int)(255.0f*(((float)(i)+1.0)/((float)(imgX.size())))));
      screen.rotate(imgA.get(i));
      if(isActive)
         screen.image(imgActive, 0, 0);
       else
         screen.image(imgInactive, 0, 0);
      screen.popMatrix();
      millisSinceLastDraw = millis();
    } 
  }
  
}
