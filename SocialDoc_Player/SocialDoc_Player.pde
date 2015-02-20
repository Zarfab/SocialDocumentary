// java imports
import java.util.*;

// Processing import
import TUIO.*;
import oscP5.*;
import netP5.*;
import processing.video.*;


/**************************************************************/
/***************** Global variables   *************************/
/**************************************************************/

SegmentManager segManager;
String srtFilePath;
KeywordManager kwManager;
String keywordFilePath;

// Communication variables
TuioProcessing tuioClient;
TuioObjectsManager tuioManager;
OscP5 oscP5;
NetAddress KISDapp;
NetAddress tableApp;

// users variables
int nbViewers;
UserKISD users[];
int jointAttention;

// video variables
Movie video;
String videoFilePath;
boolean videoPlaying;
boolean videoWasPlaying;
int welcomeTimer;

// Tweets
TweetManager tweetManager;
String tweetToDisp;
String tweetFilePath;

DisposeHandler dh;

/**************************************************************/
/***************** Processing methods *************************/
/**************************************************************/

// full screen mode
boolean sketchFullScreen() {
  return true;
  //return false;
}

// set full screen size, create instances of SegmentManager, TweetManager and Movie with their file paths
// open TUIO and OSC connections
void setup() { 
  dh = new DisposeHandler(this);
  
  if(sketchFullScreen())
    size(displayWidth, displayHeight);
  else
    size(800, 450);
  frameRate(25);
  
//  videoFilePath = "default"; 
//  selectInput("Select a video file to use:", "videoFileSelected");
//  while(videoFilePath.equals("default")) {
//    delay(50);
//  }
//  print("videoFilePath: "+videoFilePath+"\n");
//  if(videoFilePath.equals("default")) {
//    videoFilePath = dataPath("GeziParkDocumentary1.mp4");
//  }
  videoFilePath = dataPath("GeziParkDocumentary1.mp4");
  video = new Movie(this, videoFilePath);
  videoPlaying = false;
  videoWasPlaying = false;
  welcomeTimer = millis();
  //video.loop();
  
 /* srtFilePath = dataPath("SocialDoc_sample.srt");
  selectInput("Select a segment (*.srt) file to use:", "srtFileSelected");
  if(!srtFilePath.equals("")) {
    segManager = new SegmentManager(srtFilePath);
    segManager.calculateNewSegmentsList(new ArrayList<String>());
  }
  
  keywordFilePath = dataPath("keywords.xml");
  selectInput("Select a keywords file to use:", "keywordFileSelected");
  if(!keywordFilePath.equals("")) {
     kwManager = new KeywordManager(keywordFilePath);
  }
  
  tweetFilePath = dataPath("Tweets-OccupyGezi.txt");
  selectInput("Select a text (tweets) file to use:", "tweetFileSelected");
  if(!tweetFilePath.equals("")) {
    tweetManager = new TweetManager(tweetFilePath);
  }*/
  
  String prefixFilePath = videoFilePath.substring(0, videoFilePath.indexOf(".", videoFilePath.length()-5));
  
  srtFilePath = prefixFilePath + ".srt";
  segManager = new SegmentManager(srtFilePath);
  segManager.calculateNewSegmentsList(new ArrayList<String>(), new int[0]);
  
  keywordFilePath = prefixFilePath + "_keywords.xml";
  kwManager = new KeywordManager(keywordFilePath);
  
  tweetFilePath = prefixFilePath + "_tweets.txt";
  tweetManager = new TweetManager(tweetFilePath);
  
  
  // communication opening
  XML configElement = loadXML("config.xml");
  
  // receive from Kinect attention tracker and reactable
  XML oscListenerElement = configElement.getChild("osclistener");
  oscP5 = new OscP5(this, oscListenerElement.getInt("port"));
  
  XML[] oscSenderElements = configElement.getChildren("oscsender");
  for(int i=0; i<oscSenderElements.length; i++) {
    XML oscSenderElement = oscSenderElements[i];
    String dest = oscSenderElement.getString("dest");
    if(dest.equals("KISD")) {
      // send to Kinect attention tracker (reset timers)
      KISDapp = new NetAddress(oscSenderElement.getString("address"), oscSenderElement.getInt("port"));
      println("sendind messages to Kinect application at "+KISDapp);
    }
    else if(dest.equals("table")) {    
      // send to table display (relevance and enrichment update)
      tableApp = new NetAddress(oscSenderElement.getString("address"), oscSenderElement.getInt("port"));
      println("sendind messages to reactable application at "+tableApp);
    }
  }
  
  XML tuioElement = configElement.getChild("tuiolistener");
  tuioClient  = new TuioProcessing(this, tuioElement.getInt("port"));
  println("listening TUIO messages from port "+tuioElement.getInt("port"));
  tuioManager = new TuioObjectsManager();
  
  // ini users
  users = new UserKISD[6];
  for(int i=0; i<6; i++)
    users[i] = new UserKISD(0, i+1, 0, 0);
}

// display the image
void draw() {
  background(0);
  
  if(videoPlaying) {
    if(!videoWasPlaying){
      if(millis()-welcomeTimer < 3500) {
        welcomeMessage();
      }
      else {
        videoWasPlaying = true;
      }
    }
    
    else {
      image(video, 0, 0, width, height); 
    }
    
  }
  else {
    waitMessage();
  }
  
  
  if(jointAttention == 1) {
    tweetManager.printTweet();
  }
  
  if(video.time() > segManager.getCurrentSegment().getEndTime()) {
    segManager.goToNextSegment();
    updateSegment();
  }
  
  //printSelectedKeywords();
  //printSegmentAnnotations();
}

// used for debug
void mouseClicked()
{
  //for(int i=0; i<6; i++)
  //  print(users[i]);
 //segManager.incrementCurrent();
 //if(jointAttention == 1)
  //jointAttention = 0;
 //else {
  //jointAttention = 1;
  //tweetManager.nextTweet();
 //}
}

public class DisposeHandler {
   
  DisposeHandler(PApplet pa)
  {
    pa.registerMethod("dispose", this);
  }
   
  public void dispose()
  {      
    println("Closing sketch");
    segManager.saveStateInFile();
  }
}

/************************************************************/
/*************** Other methods         **********************/
/************************************************************/
void printSelectedKeywords() {
  ArrayList<String> kw = kwManager.getSelectedKeywords();
  String s = "";
  for(int i=0; i<kw.size(); i++) {
    s += " " + kw.get(i);
  }
  
  int txtsize = 32;
  int txtposY = 45;
  int rectH = 60;
  if(!sketchFullScreen()) {
    txtsize = 16;
    txtposY = 22;
    rectH = 30;
  }
  fill(0);
  rect(0, 0, width, rectH);
  textSize(txtsize);
  fill(220);
  text(s, width/2, txtposY);
}
void printSegmentAnnotations() {
  ArrayList<String> annot = segManager.getCurrentSegment().getTags();
  String s = "";
  for(int i=0; i<annot.size(); i++) {
    s += " " + annot.get(i);
  }
  
  int txtsize = 32;
  int txtposY = 45;
  int rectH = 60;
  if(!sketchFullScreen()) {
    txtsize = 16;
    txtposY = 22;
    rectH = 30;
  }
  fill(0);
  rect(0, rectH, width, rectH);

  textSize(txtsize);
  fill(220);
  text(s, width/2, rectH+txtposY);
}

// to be called when the number of markers changes
void updateKeywordList() {
  //Vector tuioObjectList = tuioClient.getTuioObjects();
  //int[] tagsID = new int[tuioObjectList.size()];
  //for (int i=0;i<tuioObjectList.size();i++) {
  //   TuioObject tobj = (TuioObject)tuioObjectList.elementAt(i);
  //   tagsID[i] = tobj.getSymbolID();
  // }
  kwManager.setSelectedKeywordsFromTagIDs(tuioManager.getTagsIDs());
  String s = "new keywords:";
  ArrayList<String> kw = kwManager.getSelectedKeywords();
  for(int i=0; i<kw.size(); i++) {
    s += " " + kw.get(i); }
  print(s+"\n");
  segManager.calculateNewSegmentsList(kw, tuioManager.getTagsIDs());
}

void updateSegment() {
  video.jump(segManager.getCurrentSegment().getStartTime());
  
  String s = "Jump to segment ";
  s += segManager.getCurrentSegment().getIndexInFile();
  s += " at " + segManager.getCurrentSegment().getStartTime();
  print(s + "\n");
  
  sendRelevanceMessage();
  sendNextMessage();
}

void sendNextMessage() {
  OscMessage resetMessage = new OscMessage("/player/next");
  oscP5.send(resetMessage, KISDapp);
}

void sendRelevanceMessage() {
  OscMessage relevanceMessage = new OscMessage("/video/relevant_tags");
  relevanceMessage.add(segManager.getCurrentSegment().getRelevance());
   ArrayList<Integer> relevantTags = segManager.getCurrentSegment().getRelevantTags();
  for(int i=0; i<relevantTags.size(); i++) {
    relevanceMessage.add(relevantTags.get(i));
  }
  println("send to table: "+relevanceMessage);
  oscP5.send(relevanceMessage, tableApp);
}


void welcomeMessage() {
  textAlign(CENTER);
  fill(127);
  textSize(36);
  text("Visitor detected", width/2, height/5);
  fill(200);
  textSize(48);
  text("Welcome to Istanbul!", width/2, height/2);
}

void waitMessage() {
  textSize(48);
  textAlign(CENTER);
  fill(200);
  text("Social Documentary", width/2, height/2);
}

/******************************************************************/
/*************** Callback methods *********************************/
/******************************************************************/

/* Video file selection */
void videoFileSelected(File selection) {
  if (selection == null) {
    print("Window was closed or the user hit cancel. \nTaking default value: "+videoFilePath +"\n");
  } else {
    videoFilePath = selection.getAbsolutePath();
    print("User selected " + videoFilePath +"\n");
  }
}

/* Srt file selection */
/*void srtFileSelected(File selection) {
  if (selection == null) {
    print("Window was closed or the user hit cancel.\nTaking default value: "+srtFilePath+"\n");
  } else {
    srtFilePath = selection.getAbsolutePath();
    print("User selected " + srtFilePath+"\n");
  }
}*/

/* keywords file selection */
/*void keywordFileSelected(File selection) {
  if (selection == null) {
    print("Window was closed or the user hit cancel.\nTaking default value: "+keywordFilePath+"\n");
  } else {
    keywordFilePath = selection.getAbsolutePath();
    print("User selected " + keywordFilePath+"\n");
  }
}*/

/* tweets file selection */
/*void tweetFileSelected(File selection) {
  if (selection == null) {
    print("Window was closed or the user hit cancel.\nTaking default value: "+tweetFilePath+"\n");
  } else {
    tweetFilePath = selection.getAbsolutePath();
    print("User selected " + tweetFilePath+"\n");
  }
}*/


/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage mes) {
  /* print the address pattern and the typetag of the received OscMessage */
  //print("### received an osc message.");
  //print(" addrpattern: "+ mes.addrPattern());
  //print(" argType: "+ mes.typetag() + "\n");
  
  // update nbViewers and joint attention if nbViewers = 0
  if(mes.checkAddrPattern("/context/nbusers")==true) {
      int temp = nbViewers;
      nbViewers = (int)mes.get(0).floatValue();
      if(nbViewers < temp) {
        for(int i=0; i<6; i++)
          users[i].clearValues();
      }
      if(nbViewers <= 1)
        jointAttention = 0;
      videoWasPlaying = videoPlaying;
      // stop the video when no one is present
      if(nbViewers == 0) {
        video.stop();
        videoPlaying = false;
      }
      else {
        welcomeTimer = millis();
        video.loop();
        videoPlaying = true;
      }
      return;
  }
  // update face tracked users
  if(mes.checkAddrPattern("/context/facetrackedusers")==true) {
      ArrayList<Integer> activeUsers = new ArrayList<Integer>();
      for(int i=0; i<mes.typetag().toString().length() ;i++)
        activeUsers.add((int)mes.get(i).floatValue());
      for(int i=0; i<6; i++) {
        if(activeUsers.contains(i+1))
          users[i].faceTracked = 1;
        else
          users[i].faceTracked = 0;
      }      
      return;
  }
  // update attention for one user
  if(mes.checkAddrPattern("/context/user/attention")==true) {
      int userID = (int)mes.get(0).floatValue();
      int sw = (int)mes.get(1).floatValue();
      int interest = (int)mes.get(2).floatValue();
      if(interest == 2)
        segManager.incrementCurrent();
      users[userID-1].screenWatched = sw;
      users[userID-1].interest = interest;
      return;
  }
  // update joint attention
  if(mes.checkAddrPattern("/context/jointattention")==true) {
      jointAttention = (int)mes.get(0).floatValue();
      if(jointAttention ==1)
       tweetManager.nextTweet();
      return;
  }
  // update coordinates for one user (where the user is currently looking at)
  if(mes.checkAddrPattern("/context/user/coordinates")==true) {
      int userID = (int)mes.get(0).floatValue();
      users[userID-1].screenWatched = (int)mes.get(1).floatValue();
      users[userID-1].pushCoordXY(/* x */mes.get(2).floatValue(), /* y */mes.get(3).floatValue() * -1);
      return;
  }
  
  // update reactable next / prev events
  if(mes.checkAddrPattern("/reactable/next")==true) {
    //go to next video
    segManager.goToNextSegment();
    updateSegment();
    //send feedback message
    OscMessage videoNextMessage = new OscMessage("/video/next");
    oscP5.send(videoNextMessage, tableApp);
    return;
  }
  if(mes.checkAddrPattern("/reactable/prev")==true) {
    //go to previous video
    segManager.goToPreviousSegment();
    updateSegment();
    //send feedback message
    OscMessage videoPrevMessage = new OscMessage("/video/prev");
    oscP5.send(videoPrevMessage, tableApp);
    return;
  }
}


// these callback methods are called whenever a TUIO event occurs
// called when an object is added to the scene
void addTuioObject(TuioObject tobj) {
  //println("add object "+tobj.getSymbolID()+" ("+tobj.getSessionID()+") "+tobj.getX()+" "+tobj.getY()+" "+tobj.getAngle());  
  tuioManager.pushObjects(tuioClient.getTuioObjects());
  if(tuioManager.isRealEvent()) {
    updateKeywordList();
    updateSegment();
  }
}

// called when an object is removed from the scene
void removeTuioObject(TuioObject tobj) {
  //println("remove object "+tobj.getSymbolID()+" ("+tobj.getSessionID()+")");
  tuioManager.pushObjects(tuioClient.getTuioObjects());
  if(tuioManager.isRealEvent()) {
    updateKeywordList();
    updateSegment();
  }
}

// called when an object is moved
void updateTuioObject (TuioObject tobj) {
  //println("update object "+tobj.getSymbolID()+" ("+tobj.getSessionID()+") "+tobj.getX()+" "+tobj.getY()+" "+tobj.getAngle()
  //        +" "+tobj.getMotionSpeed()+" "+tobj.getRotationSpeed()+" "+tobj.getMotionAccel()+" "+tobj.getRotationAccel());
}

// unused but necessary
void addTuioCursor(TuioCursor tcur) {}
void removeTuioCursor(TuioCursor tcur) {}
void updateTuioCursor(TuioCursor tcur) {}

// called after each message bundle
// representing the end of an image frame
void refresh(TuioTime bundleTime) { 
  redraw();
}

void movieEvent(Movie m) {
  m.read();
}

