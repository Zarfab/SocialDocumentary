// and declare a TuioProcessing client variable
import TUIO.*;
import java.util.Vector;
TuioProcessing tuioClient;

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

PImage img_logo;

boolean sketchFullScreen(){
  return true;
}

void setup()
{
  size(displayWidth,displayHeight);
  noStroke();
  fill(0);
  
  loop();
  frameRate(30);
  
  font = createFont("Arial", 18);
  scale_factor = height/table_size;
  
  // we create an instance of the TuioProcessing client
  // since we add "this" class as an argument the TuioProcessing class expects
  // an implementation of the TUIO callback methods (see below)
  tuioClient  = new TuioProcessing(this);

  // people = cyan
  // emotion = yellow 
  // action = pink
  
  // Tangible filters represent 3 categories. 
  // Corresponding image file naming:
  // people_0.png
  // emotion_0.png
  // action_0.png
  img_people = new PImage[6];
  img_emotion = new PImage[6];
  img_action = new PImage[6];
  
  
  for (int i = 0; i < 6; i++ ){
    img_people[i] = loadImage("people_"+i+"b.png");
    img_people[i].resize((int)(img_people[i].width*scale_factor), (int)(img_people[i].height*scale_factor));
    img_emotion[i] = loadImage("emotion_"+i+"b.png");
    img_emotion[i].resize((int)(img_emotion[i].width*scale_factor), (int)(img_emotion[i].height*scale_factor));
    img_action[i] = loadImage("action_"+i+"b.png");
    img_action[i].resize((int)(img_action[i].width*scale_factor), (int)(img_action[i].height*scale_factor));
  }
  
  img_logo = loadImage("SocialDocumentary.png");
   
}


// within the draw method we retrieve a Vector (List) of TuioObject and TuioCursor (polling)
// from the TuioProcessing client and then loop over both lists to draw the graphical feedback.
void draw()
{

  background(0);
  
  //display logo
  imageMode(CORNERS);
  image(img_logo, sketchTopLeftX, sketchTopLeftY);
  
  imageMode(CENTER); 
   
  Vector tuioObjectList = tuioClient.getTuioObjects();
  
  for (int i=0;i<tuioObjectList.size();i++) {
     TuioObject tobj = (TuioObject)tuioObjectList.elementAt(i);
     int id = tobj.getSymbolID();     
       
     // Draw relation lines: 
     // only different categories can have a distance relation with
     // eachother. each object is belonging to a different 
     // category by definition
     /*if (tuioObjectList.size() > 1){
       int x = tobj.getScreenX(width);
       int y = tobj.getScreenY(height);
        for (int j = i+1; j < tuioObjectList.size(); j++) {
          TuioObject tobjNext = (TuioObject)tuioObjectList.elementAt(j);
          int nextX = tobjNext.getScreenX(width);
          int nextY = tobjNext.getScreenY(height);
        
          float d = dist(x, y, nextX, nextY);
          //println("Dist [" + i + "," + j + "] = " + d); 
          
         if (tobj.getTuioState() == TuioCursor.TUIO_STOPPED && 
              tobjNext.getTuioState() == TuioCursor.TUIO_STOPPED && 
              d < 600) {
                pushMatrix();
                   color c1 = #FBF9ED;
                   stroke(c1);
                   line(x, y, nextX, nextY);    
                popMatrix();            
          }
       }
     }*/
     
     //int y_offset = -23;
     int y_offset = 0;
     pushMatrix();
       translate(tobj.getScreenX(width),tobj.getScreenY(height) + y_offset);
       rotate(-tobj.getAngle());
       smooth(4);
       
       
       if(id >= 0 && id < 6) // people
       {
         image(img_people[id], 0, 0);
       }
       if(id >= 6 && id < 12) //action
       {
         image(img_action[id%6], 0, 0);
       }
       if(id >= 12 && id < 18) // emotion
       {
         image(img_emotion[id%6], 0, 0);
       }  
     popMatrix();
   
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

