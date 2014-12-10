// and declare a TuioProcessing client variable
import TUIO.*;
import java.util.Vector;
import oscP5.*;
import netP5.*;

TuioProcessing tuioClient;
OscP5 oscP5;

// these are some helper variables which are used
// to create scalable graphical feedback
float cursor_size = 120;
float object_size = 60;
float table_size = 920; // TODO: 46.5cm x 66.5cm
float scale_factor;
PFont font;

// Sketch area
// since we are losing some of the projection, sketch should be inside following area 
// table projector resolution assumption: 1024x768
int sketchTopLeftX = 0;
int sketchTopLeftY = 12;
//int sketchWidth = displayWidth - 59;
//int sketchHeight = displayHeight - 129;


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
  if(sketchFullScreen())
    size(displayWidth,displayHeight);
  else
    size(800, 450);
  noStroke();
  fill(0);
  
  loop();
  frameRate(30);
  
  font = createFont("Arial", 18);
  scale_factor = height/table_size;
  
  // we create an instance of the TuioProcessing client
  // since we add "this" class as an argument the TuioProcessing class expects
  // an implementation of the TUIO callback methods (see below)
  tuioClient  = new TuioProcessing(this, 3334);
  
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
   
}


// within the draw method we retrieve a Vector (List) of TuioObject and TuioCursor (polling)
// from the TuioProcessing client and then loop over both lists to draw the graphical feedback.
void draw()
{

  background(0);
  
  //display logo
  imageMode(CORNERS);
  image(img_logo, sketchTopLeftX, sketchTopLeftY);
  image(ltv_logo, width-ltv_logo.width, sketchTopLeftY);
  
  // display a circle for interaction zone
  switch(videoRelevance) {
    case 1: stroke(156, 48, 48);
      break;
    case 2: stroke(176, 156, 0);
      break;
    case 3: stroke(48, 156, 48);
      break;
    default: 
      stroke(127);
      break;
  }
  noFill();
  strokeWeight(videoRelevance*3+1);
  ellipse(width*0.5, height*0.6, width*0.6, height*0.6);
  
  imageMode(CENTER); 
  noStroke();
   
  Vector tuioObjectList = tuioClient.getTuioObjects();
  
  for (int i=0;i<tuioObjectList.size();i++) {
     TuioObject tobj = (TuioObject)tuioObjectList.elementAt(i);
     int id = tobj.getSymbolID();     
     
     float x_translate = tobj.getScreenX(width);// + (width-tobj.getScreenX(width))*0.1;
     float y_translate = tobj.getScreenY(height);
     pushMatrix();
       translate(x_translate, y_translate);
       rotate(tobj.getAngle());
       smooth(4);
       
       
       if(id >= 0 && id < 6) // people
       {
         if(activeTags.hasValue(id))
           image(img_people[id], 0, 0);
         else
           image(img_people[id+6], 0, 0);
       }
       if(id >= 6 && id < 12) //action
       {
         if(activeTags.hasValue(id))
           image(img_action[id%6], 0, 0);
         else
           image(img_action[id%6+6], 0, 0);
       }
       if(id >= 12 && id < 18) // emotion
       {
         if(activeTags.hasValue(id))
           image(img_emotion[id%6], 0, 0);
         else
           image(img_emotion[id%6+6], 0, 0);
       }  
     popMatrix();
     

     enrichments.selectImages(tuioObjectList, forceImageUpdate);
     enrichments.displaySelection();
     if(forceImageUpdate)
       forceImageUpdate = false;
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

