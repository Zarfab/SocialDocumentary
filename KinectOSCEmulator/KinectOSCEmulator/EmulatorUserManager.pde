import oscP5.*;

class EmulatorUserManager {
  
  private class EmulatorUser {
    int id;
    boolean isInScene;
    int gazeX, gazeY;
    ArrayList<Integer> xArray, yArray;
    int filterSize;
    int interest;
    int screenWatched;
    color c;
    private int sceneChangeTime, screenWatchedTime;
    private int timeInScene; // ms passed in scene
    private int timeOutScene; // ms passed out scene
  
    EmulatorUser(int id, int tInScene, int tOutScene) { 
      this.id = id;
      if(id == 1)
        c = #FF0000;
      else
        c = #0000FF;
        
      this.timeInScene = tInScene * 1000;
      this.timeOutScene = tOutScene * 1000;
      sceneChangeTime = millis();
      isInScene = false;
      
      filterSize = 5;
      xArray = new ArrayList<Integer>();
      yArray = new ArrayList<Integer>();
      initCoord();

      interest = 0;
      screenWatched = 0;
      print("new user, eid: "+id+", time in: "+timeInScene+", time out: "+timeOutScene+"\n");
    }
    
    void update() {
      boolean inScenePrev = isInScene;
      if(millis() - sceneChangeTime > timeInScene && isInScene) {
        isInScene = false;
        sceneChangeTime = millis();
      }
      if(millis() - sceneChangeTime > timeOutScene && !isInScene) {
        isInScene = true;
        sceneChangeTime = millis();
      }   
      
      if(isInScene) {
        //update coord with the filters
        xArray.remove(0);
        xArray.add(constrain(gazeX + (int)random(-20, 20), 0, width));
        yArray.remove(0);
        yArray.add(constrain(gazeY + (int)random(-15, 15), 0, height));
        
        int x = 0, y = 0;
        for(int i=0; i<filterSize; i++) {
          x += xArray.get(i);
          y += yArray.get(i);
        }
        gazeX = x/filterSize;
        gazeY = y/filterSize;
           
        // draw a circle at the looking point
        noStroke();
        fill(c);
        ellipse(gazeX, gazeY, 10, 10); 
        
        
        // update screen watched
        int screenWatchedPrev = screenWatched;
        if(gazeX > 160 && gazeX < 480 && gazeY > 120 && gazeY < 360)
          screenWatched = 1;
        else
          screenWatched = 0;
             
        // update interest
        if(screenWatchedPrev != screenWatched) {
          screenWatchedTime = millis();
        }
        else {
          int timeSpent = millis() - screenWatchedTime;
          if(timeSpent < 1500)
            interest = 0;
          if(timeSpent >= 1500 && timeSpent < 5500)
            interest = 1;
          if(timeSpent >= 5500 && timeSpent < 15000)
            interest = 2;
          if(timeSpent >= 15000)
            interest = 3;  
        }   
      }
      else {
        initCoord();
        interest = 0;
        screenWatched = 0;
      }
    }
    
    void initCoord() {
      gazeX = width/2;
      gazeY = height/2;
      xArray.clear();
      yArray.clear();
      for(int i=0; i<filterSize; i++) {
        xArray.add(gazeX);
        yArray.add(gazeY);
      }   
    }
  }



  private ArrayList<EmulatorUser> users;
  private int nbUsers;
  private int jointAttention;
  private ArrayList<OscMessage> messages;
  
  EmulatorUserManager() {
    messages = new ArrayList<OscMessage>();
    users = new ArrayList<EmulatorUser>();
    for(int i=1; i<=2; i++) {
      users.add(new EmulatorUser(i, (int)random(14,20), (int)random(3, 7)));
    }
    nbUsers = 0;
  }
  
  void update() {
    int nbUsersPrev = nbUsers;
    nbUsers = 0;
    for(int i=0; i<users.size(); i++) {
      int interestPrev = users.get(i).interest;
      users.get(i).update();
      if(users.get(i).isInScene) {
        nbUsers++;
      }
      if(interestPrev != users.get(i).interest)
      {
        OscMessage mes = new OscMessage("/context/user/attention");
        mes.add((float)users.get(i).id);
        mes.add((float)users.get(i).screenWatched);
        mes.add((float)users.get(i).interest);
        messages.add(mes);
      }
    }
    if(nbUsersPrev != nbUsers) {
      OscMessage mes = new OscMessage("/context/nbusers");
      mes.add((float)nbUsers);
      messages.add(mes);
    }

    // joint attention
    int jaPrev = jointAttention;
    jointAttention = users.get(0).screenWatched * users.get(1).screenWatched;
    if(jaPrev != jointAttention) {
      OscMessage mes = new OscMessage("/context/jointattention");
      mes.add((float)jointAttention);
      messages.add(mes);
    }
  }
  
  boolean hasNewMessages() {
    return messages.size() > 0;
  }
  
  ArrayList<OscMessage> getMessages() {
    ArrayList<OscMessage> temp = new ArrayList<OscMessage>();
    for(int i=0; i<messages.size(); i++)
      temp.add(messages.get(i));
    messages.clear();
    return temp;
  }
}
