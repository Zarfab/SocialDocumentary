import TUIO.*;                //TUIO library
import java.util.Vector;      //
import oscP5.*;               //OSC library
import netP5.*;               //OSC
import deadpixel.keystone.*;  //projection mapping library

// calibration and projection mapping
Keystone ks;
CornerPinSurface surface;
PGraphics offscreen;
PImage pattern;
boolean calibrationMode = false;

TuioProcessing tuioClient;
OscP5 oscP5;

// these are some helper variables which are used
// to create scalable graphical feedback
boolean fingerTracking = false;
float cursor_size = 32;
float object_size = 60;
float table_size = 920; // TODO: 46.5cm x 66.5cm
float scale_factor;
float cur_size;
PFont font;


PImage[] img_people;
PImage[] img_emotion;
PImage[] img_action;
IntList activeTags;

PImage img_logo, ltv_logo;

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
  
  //loop();
  //frameRate(30);
  
  font = createFont("Arial", 18);
  scale_factor = offscreen.height/table_size;
  
  // we create an instance of the TuioProcessing client
  // since we add "this" class as an argument the TuioProcessing class expects
  // an implementation of the TUIO callback methods (see below)
  tuioClient  = new TuioProcessing(this, 3334);
  cur_size = cursor_size*scale_factor; 
  
  // communication with the main player
  oscP5 = new OscP5(this,11999);

  // people = cyan
  // emotion = yellow 
  // action = pink
  
  // Tangible filters represent 3 categories. 
  // Corresponding image file naming:
  // people_0.png
  // emotion_0.png
  // action_0.png
  img_people = new PImage[12];
  img_emotion = new PImage[12];
  img_action = new PImage[12];
  
  
  for (int i = 0; i < 6; i++ ){
    img_people[i] = loadImage("people_"+i+"b.png");
    img_people[i].resize((int)(img_people[i].width*scale_factor), (int)(img_people[i].height*scale_factor));
    img_emotion[i] = loadImage("emotion_"+i+"b.png");
    img_emotion[i].resize((int)(img_emotion[i].width*scale_factor), (int)(img_emotion[i].height*scale_factor));
    img_action[i] = loadImage("action_"+i+"b.png");
    img_action[i].resize((int)(img_action[i].width*scale_factor), (int)(img_action[i].height*scale_factor));
    
    img_people[i+6] = loadImage("people_"+i+"n.png");
    img_people[i+6].resize((int)(img_people[i+6].width*scale_factor), (int)(img_people[i+6].height*scale_factor));
    img_emotion[i+6] = loadImage("emotion_"+i+"n.png");
    img_emotion[i+6].resize((int)(img_emotion[i+6].width*scale_factor), (int)(img_emotion[i+6].height*scale_factor));
    img_action[i+6] = loadImage("action_"+i+"n.png");
    img_action[i+6].resize((int)(img_action[i+6].width*scale_factor), (int)(img_action[i+6].height*scale_factor));
  }
  activeTags = new IntList();
  
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
  
    for (int i=0;i<tuioObjectList.size();i++) {
       TuioObject tobj = (TuioObject)tuioObjectList.elementAt(i);
       int id = tobj.getSymbolID();     
     
       float x_translate = tobj.getScreenX(offscreen.width);// + (width-tobj.getScreenX(width))*0.1;
       float y_translate = tobj.getScreenY(offscreen.height);
       offscreen.pushMatrix();
       offscreen.translate(x_translate, y_translate);
         offscreen.rotate(tobj.getAngle());
         offscreen.smooth(4);
              
         if(id >= 0 && id < 6) { // people
           if(activeTags.hasValue(id))
             offscreen.image(img_people[id], 0, 0);
           else
             offscreen.image(img_people[id+6], 0, 0);
         }
         if(id >= 6 && id < 12) { //action
           if(activeTags.hasValue(id))
             offscreen.image(img_action[id%6], 0, 0);
           else
             offscreen.image(img_action[id%6+6], 0, 0);
         }
         if(id >= 12 && id < 18) { // emotion
           if(activeTags.hasValue(id))
             offscreen.image(img_emotion[id%6], 0, 0);
           else
             offscreen.image(img_emotion[id%6+6], 0, 0);
         }
       offscreen.popMatrix();
    }
    
    if(fingerTracking) {
      Vector tuioCursorList = tuioClient.getTuioCursors();
      for (int i=0;i<tuioCursorList.size();i++) {
        TuioCursor tcur = (TuioCursor)tuioCursorList.elementAt(i);
        Vector pointList = tcur.getPath();
      
        if (pointList.size()>0) {
          stroke(127);
          TuioPoint start_point = (TuioPoint)pointList.firstElement();;
          for (int j=0;j<pointList.size();j++) {
            TuioPoint end_point = (TuioPoint)pointList.elementAt(j);
            line(start_point.getScreenX(offscreen.width),start_point.getScreenY(offscreen.height),
              end_point.getScreenX(offscreen.width),end_point.getScreenY(offscreen.height));
            start_point = end_point;
          }
        
          stroke(192,192,192);
          fill(192,192,192);
          ellipse( tcur.getScreenX(offscreen.width), tcur.getScreenY(offscreen.height),cur_size,cur_size);
        }
      }
    }
    

     enrichments.selectImages(tuioObjectList, forceImageUpdate);
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
    activeTags.clear();
    for(int i=0; i<videoRelevance; i++) {
      activeTags.append(mes.get(i+1).intValue());
    }
    forceImageUpdate = true;
  }
}




// these callback methods are called whenever a TUIO event occurs

// called when an object is added to the scene
void addTuioObject(TuioObject tobj) {
  println("add object "+tobj.getSymbolID()+" ("+tobj.getSessionID()+") "+tobj.getX()+" "+tobj.getY()+" "+tobj.getAngle());
  
}

// called when an object is removed from the scene
void removeTuioObject(TuioObject tobj) {
  println("remove object "+tobj.getSymbolID()+" ("+tobj.getSessionID()+")");
}

// called when an object is moved
void updateTuioObject (TuioObject tobj) {
  println("update object "+tobj.getSymbolID()+" ("+tobj.getSessionID()+") "+tobj.getX()+" "+tobj.getY()+" "+tobj.getAngle()
          +" "+tobj.getMotionSpeed()+" "+tobj.getRotationSpeed()+" "+tobj.getMotionAccel()+" "+tobj.getRotationAccel());
}

// called when a cursor is added to the scene
void addTuioCursor(TuioCursor tcur) {
  println("add cursor "+tcur.getCursorID()+" ("+tcur.getSessionID()+ ") " +tcur.getX()+" "+tcur.getY());
}

// called when a cursor is moved
void updateTuioCursor (TuioCursor tcur) {
  println("update cursor "+tcur.getCursorID()+" ("+tcur.getSessionID()+ ") " +tcur.getX()+" "+tcur.getY()
          +" "+tcur.getMotionSpeed()+" "+tcur.getMotionAccel());
}

// called when a cursor is removed from the scene
void removeTuioCursor(TuioCursor tcur) {
  println("remove cursor "+tcur.getCursorID()+" ("+tcur.getSessionID()+")");
}

// called after each message bundle
// representing the end of an image frame
void refresh(TuioTime bundleTime) { 
  redraw();
}

