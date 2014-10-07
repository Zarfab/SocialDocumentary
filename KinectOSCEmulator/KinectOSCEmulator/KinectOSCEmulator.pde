/**
 * oscP5sendreceive by andreas schlegel
 * example shows how to send and receive osc messages.
 * oscP5 website at http://www.sojamo.de/oscP5
 */
 
import oscP5.*;
import netP5.*;
  
OscP5 oscP5;
NetAddress playerApp;

EmulatorUserManager manager;

void setup() {
  size(640,480);
  frameRate(25);
  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this,12001);
  
  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application. usage see below. for testing purposes the listening port
   * and the port of the remote location address are the same, hence you will
   * send messages back to this sketch.
   */
  playerApp = new NetAddress("127.0.0.1",12000);
  
  manager = new EmulatorUserManager();
}


void draw() {
  background(0);
  stroke(255);
  noFill();
  rect(160, 120, 320, 240);
  
  manager.update();
  if(manager.hasNewMessages()) {
    ArrayList<OscMessage> mesArray = manager.getMessages();
    for(int i=0; i<mesArray.size(); i++) {
      oscP5.send(mesArray.get(i), playerApp);
      print("send message: "+mesArray.get(i).addrPattern());
      for(int j=0; j<mesArray.get(i).typetag().toString().length() ;j++)
        print(" "+mesArray.get(i).get(j).floatValue());
        
      print("\n");
    }
  }
}


/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}
