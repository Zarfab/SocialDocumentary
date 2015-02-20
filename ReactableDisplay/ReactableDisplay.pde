import TUIO.*;                //TUIO library
import java.util.Vector;      //
import oscP5.*;               //OSC library
import netP5.*;               //OSC
import deadpixel.keystone.*;  //projection mapping library
import controlP5.*;           //button interface

// calibration and projection mapping
Keystone ks;
CornerPinSurface surface;
PGraphics offscreen;
PImage pattern;
boolean calibrationMode = false;

TuioProcessing tuioClient;
OscP5 oscP5;
NetAddress playerAddress;

// these are some helper variables which are used
// to create scalable graphical feedback
boolean fingerTracking = false;
float cursor_size = 32;
float object_size = 60;
float table_size = 920; // TODO: 46.5cm x 66.5cm
float scale_factor;
float cur_size;
PFont font;


KeywordVisualManager kvm;
boolean applyCoordFilter = true;

PImage img_logo, ltv_logo;
ToogleButton nextButton, prevButton;
float buttonSize = 120;

EnrichmentManager enrichments;
int videoRelevance = 0;
boolean forceImageUpdate = false;

boolean sketchFullScreen(){
  return true;
  //return false;
}

void setup()
{
  if(sketchFullScreen()) {
    size(displayWidth, displayHeight, P3D);
  }
  else {
    size(640,480,P3D);
  }
  
  // projection mapping
  ks = new Keystone(this);
  float offscreenRatio = 0.6;
  surface = ks.createCornerPinSurface((int)(width*offscreenRatio), (int)(height*offscreenRatio), 20);
  offscreen = createGraphics((int)(width*offscreenRatio), (int)(height*offscreenRatio), P2D);
  pattern = loadImage("pattern.png");
  pattern.resize(offscreen.width, offscreen.height);
  
  font = createFont("Arial", 18);
  scale_factor = offscreen.height/table_size;
  
  buttonSize = offscreen.height/10.0;
  PImage nextU = loadImage("next_up.png");
  PImage nextD = loadImage("next_down.png");
  nextButton = new ToogleButton("", offscreen.width/2 + buttonSize, offscreen.height-buttonSize, nextU, nextD);
  nextButton.setSize(buttonSize, buttonSize);
  PImage prevU = loadImage("prev_up.png");
  PImage prevD = loadImage("prev_down.png");
  prevButton = new ToogleButton("", offscreen.width/2 - buttonSize, offscreen.height-buttonSize, prevU, prevD);
  prevButton.setSize(buttonSize, buttonSize);
  
  XML configElement = loadXML("config.xml");
  
  XML tuioElement = configElement.getChild("tuiolistener");
  tuioClient  = new TuioProcessing(this, tuioElement.getInt("port"));
  println("listening TUIO messages from port "+tuioElement.getInt("port"));
  cur_size = cursor_size*scale_factor; 
  
  // dual communication with the main player
  XML oscListenerElement = configElement.getChild("osclistener");
  oscP5 = new OscP5(this, oscListenerElement.getInt("port"));
  XML oscSenderElement = configElement.getChild("oscsender");
  playerAddress = new NetAddress(oscSenderElement.getString("address"), oscSenderElement.getInt("port"));
  println("sending OSC to : "+playerAddress);
  
  kvm = new KeywordVisualManager(this, "keywords.xml", scale_factor);
  
  img_logo = loadImage("SocialDocumentary.png");
  ltv_logo = loadImage("LinkedTV_WholeLogo.png");
  ltv_logo.resize(0, img_logo.height);
  
  
  enrichments = new EnrichmentManager(dataPath("MediaEnrichment"));
  
  ks.load();
   
}


// within the draw method we retrieve a Vector (List) of TuioObject and TuioCursor (polling)
// from the TuioProcessing client and then loop over both lists to draw the graphical feedback.
void draw()
{
  // Convert the mouse coordinate into surface coordinates
  // this will allow you to use mouse events inside the 
  // surface from your screen. 
  PVector surfaceMouse = surface.getTransformedMouse();
  background(0);
  
  // Draw the scene, offscreen
  offscreen.beginDraw();
  offscreen.background(0);
  
  if(calibrationMode) {
    offscreen.fill(0, 255, 0);
    offscreen.ellipse(surfaceMouse.x, surfaceMouse.y, 75, 75);
    offscreen.image(pattern, offscreen.width/2, offscreen.height/2);
  }
  else {  
    //display logo
    offscreen.imageMode(CORNERS);
    offscreen.image(img_logo, 0, 0);
    offscreen.image(ltv_logo, offscreen.width-ltv_logo.width, 0);
  
    // display a circle for interaction zone
    offscreen.stroke(127);
    offscreen.strokeWeight(2);
    offscreen.noFill();
    offscreen.ellipse(offscreen.width*0.5, offscreen.height*0.6, offscreen.width*0.6, offscreen.height*0.6);
  
    offscreen.imageMode(CENTER); 
    offscreen.noStroke();
   
    Vector tuioObjectList = tuioClient.getTuioObjects();
    kvm.updateTUIOList(tuioObjectList);
    kvm.drawKeywords(offscreen, applyCoordFilter);
    
    if(fingerTracking) {
      Vector tuioCursorList = tuioClient.getTuioCursors();
      for (int i=0;i<tuioCursorList.size();i++) {
        TuioCursor tcur = (TuioCursor)tuioCursorList.elementAt(i);
        boolean nextPressed = nextButton.isPressed(tcur.getScreenX(offscreen.width), tcur.getScreenY(offscreen.height));
        boolean prevPressed = prevButton.isPressed(tcur.getScreenX(offscreen.width), tcur.getScreenY(offscreen.height));
        if(nextPressed) {
          nextButton.setState(true);
          //send next message over OSC
          OscMessage nextMessage = new OscMessage("/reactable/next");
          oscP5.send(nextMessage, playerAddress);
        }
        else if(prevPressed) {
          prevButton.setState(true);
          //send prev message over OSC
          OscMessage prevMessage = new OscMessage("/reactable/prev");
          oscP5.send(prevMessage, playerAddress);
        }
        
        Vector pointList = tcur.getPath();
        if (pointList.size()>0) {
          offscreen.stroke(127);
          TuioPoint start_point = (TuioPoint)pointList.firstElement();;
          for (int j=0;j<pointList.size();j++) {
            TuioPoint end_point = (TuioPoint)pointList.elementAt(j);
            offscreen.line(start_point.getScreenX(offscreen.width),
              start_point.getScreenY(offscreen.height),
              end_point.getScreenX(offscreen.width),
              end_point.getScreenY(offscreen.height));
            start_point = end_point;
          }
          offscreen.stroke(192,192,192);
          offscreen.fill(192,192,192);
          offscreen.ellipse( tcur.getScreenX(offscreen.width), tcur.getScreenY(offscreen.height),cur_size,cur_size);
        }
      }
      
      nextButton.drawOnScreen(offscreen);
      prevButton.drawOnScreen(offscreen);
    }
    
    StringList keywords = kvm.getIdentifiedKeywords();
    enrichments.selectImages(keywords, forceImageUpdate);
    enrichments.displaySelection(offscreen);
    if(forceImageUpdate)
      forceImageUpdate = false;
   }
   
   
   
   offscreen.endDraw();  
  // render the scene, transformed using the corner pin surface
  surface.render(offscreen);
}

void keyPressed() {
  switch(key) {
  case 'f':
    // toogle finger tracking
    fingerTracking = !fingerTracking;
    break;
  case 'c':
    // enter/leave calibration mode, where surfaces can be warped 
    // and moved
    ks.toggleCalibration();
    calibrationMode = !calibrationMode;
    break;

  case 'l':
    // loads the saved layout
    ks.load();
    break;

  case 's':
    // saves the layout
    ks.save();
    break;
  }
}

// OSC callback
void oscEvent(OscMessage mes) {
  /* print the address pattern and the typetag of the received OscMessage */
  //print("### received an osc message.");
  //print(" addrpattern: "+ mes.addrPattern());
  //print(" argType: "+ mes.typetag() + "\n");
  
  if(mes.checkAddrPattern("/video/relevant_tags")==true) {
    videoRelevance = mes.get(0).intValue(); // nb tags
    IntList activeTags = new IntList();
    for(int i=0; i<videoRelevance; i++) {
      activeTags.append(mes.get(i+1).intValue());
    }
    kvm.setActiveTags(activeTags);
    forceImageUpdate = true;
    return;
  }
  
  // next video playing
  if(mes.checkAddrPattern("/video/next")==true) {
    nextButton.setState(false);
    return;
  }
  // previous video playing
  if(mes.checkAddrPattern("/video/prev")==true) {
    prevButton.setState(false);
    return;
  }
}


// these callback methods are called whenever a TUIO event occurs

// called when an object is added to the scene
void addTuioObject(TuioObject tobj) {
 // println("add object "+tobj.getSymbolID()+" ("+tobj.getSessionID()+") "+tobj.getX()+" "+tobj.getY()+" "+tobj.getAngle());
  
}

// called when an object is removed from the scene
void removeTuioObject(TuioObject tobj) {
  //println("remove object "+tobj.getSymbolID()+" ("+tobj.getSessionID()+")");
}

// called when an object is moved
void updateTuioObject (TuioObject tobj) {
 // println("update object "+tobj.getSymbolID()+" ("+tobj.getSessionID()+") "+tobj.getX()+" "+tobj.getY()+" "+tobj.getAngle()
        //  +" "+tobj.getMotionSpeed()+" "+tobj.getRotationSpeed()+" "+tobj.getMotionAccel()+" "+tobj.getRotationAccel());
}

// called when a cursor is added to the scene
void addTuioCursor(TuioCursor tcur) {
 // println("add cursor "+tcur.getCursorID()+" ("+tcur.getSessionID()+ ") " +tcur.getX()+" "+tcur.getY());
}

// called when a cursor is moved
void updateTuioCursor (TuioCursor tcur) {
 // println("update cursor "+tcur.getCursorID()+" ("+tcur.getSessionID()+ ") " +tcur.getX()+" "+tcur.getY()
          //+" "+tcur.getMotionSpeed()+" "+tcur.getMotionAccel());
}

// called when a cursor is removed from the scene
void removeTuioCursor(TuioCursor tcur) {
  //println("remove cursor "+tcur.getCursorID()+" ("+tcur.getSessionID()+")");
}

// called after each message bundle
// representing the end of an image frame
void refresh(TuioTime bundleTime) { 
  //redraw();
}

