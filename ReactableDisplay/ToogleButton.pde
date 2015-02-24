class ToogleButton {
  
  PImage imgUp, imgDown;
  String text = "";
  boolean isDown;
  PVector position;
  PVector size;
  int lastEventMillis;
  int timeBetweenEvents = 1000;
  boolean wasPressed = false;
  
  public ToogleButton(String p_text, float xPos, float yPos, PImage up, PImage down) {
    isDown = false;
    
    text = p_text;
    
    position = new PVector();
    position.x = xPos;
    position.y = yPos;
    
    size = new PVector();
    size.x = 120;
    size.y = 120;
    
    imgUp = up;
    imgDown = down;
      
    if(imgUp != null && imgDown != null) {      
      if(imgUp.width > imgDown.width) size.x = imgUp.width;
      else size.x = imgDown.width;
      if(imgUp.height > imgDown.height) size.x = imgUp.height;
      else size.x = imgDown.height;
      
      imgUp.resize((int)size.x, (int)size.y);
      imgDown.resize((int)size.x, (int)size.y);
    }     
  }
  
  public void setSize(float newWidth, float newHeight) {
    size.x = newWidth;
    size.y = newHeight;
    imgUp.resize((int)size.x, (int)size.y);
    imgDown.resize((int)size.x, (int)size.y);
  }
  
  public void setPosition(float x, float y) {
    position.x = x;
    position.y = y;
  }
  
  public void setState(boolean newState) {
    isDown = newState;
  }
  
  public boolean getState() {
    return isDown;
  }
  
  public boolean isPressed(float cursorX, float cursorY) {
    boolean pressed = false;
    if (cursorX < position.x+size.x/2 &&
          cursorX > position.x-size.x/2 &&
          cursorY < position.y+size.y/2 &&
          cursorY > position.y-size.y/2 &&
          millis()- lastEventMillis > timeBetweenEvents &&
          !wasPressed ) {
      pressed = true;
      wasPressed = true;
      lastEventMillis = millis();
    }
    else
      wasPressed = false;
    return pressed;
  }
  
  public void drawOnScreen(PGraphics screen) {
    screen.pushMatrix();
    screen.translate(position.x, position.y);
    screen.imageMode(CENTER);
    screen.rectMode(CORNER);
    if(imgUp != null && imgDown != null) {
      if(isDown) {
        screen.image(imgDown, 0, 0);
      }
      else {
        screen.image(imgUp, 0, 0);
      }
    }
    else {
      if(isDown) {
        screen.fill(196);
      }
      else {
        screen.fill(60);
      }
      screen.stroke(255);
      screen.rect(-size.x/2, -size.y/2, size.x, size.y);
    }
    screen.textAlign(CENTER);
    screen.fill(255);
    screen.text(text, 0, 0);
    
    screen.popMatrix();
  }
  
}
