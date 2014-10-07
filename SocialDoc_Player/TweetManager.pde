class TweetManager {
 private String[] tweets;
 private String tweetToDisp;
 private int maxLength;
 private int tSize;
 
 private int limitDisplayTime;
 private int startDisplayTime;
 
 // initialize tweets list from tweetFile (txt) 
  public TweetManager(String tweetFile) {
    tweets = loadStrings(tweetFile);
    
    tSize = width/52;;
    textSize(tSize);
    maxLength = 72;
    
    if(tweets == null) {
      tweets = new String[1];
      tweets[0] =  "";
    }
    
    limitDisplayTime = 2500; // display a tweet for 2.5s max
  }
  
  // chose a tweet to display at random
  public void nextTweet() {
    // change tweet after 2.5 seconds
    if(millis() - startDisplayTime > limitDisplayTime) {
      int rand = (int)random(tweets.length-1);
      tweetToDisp = tweets[rand];
      startDisplayTime = millis();
    }
  }
  
  
  // print the tweet at the middle of the main screen
  // if the string is too long, it is displayed on several lines
  public void printTweet() {
    textAlign(CENTER);
    int x =  width/2; int y = height/2;
    
    // cut the string into smaller pieces
    // and display them on several lines
    ArrayList<String> lines = cutTweet();
    for(int i=0; i<lines.size(); i++) {
      text(lines.get(i), x, y+i*tSize+4);
    }
    // if the tweet has been displayed longer than limitDisplayTime, go to the next tweet
    if((millis() - startDisplayTime) > limitDisplayTime) {
      nextTweet();
    }
  }
  
  // used to cut too long strings in several lines
  private ArrayList<String> cutTweet() {
    ArrayList<String> pieces = new ArrayList<String>();

    String toCut = new String(tweetToDisp);
    int indexToCut = maxLength;
    while(toCut.length() > maxLength && indexToCut > 0) {
      indexToCut = maxLength;      
      while(toCut.charAt(indexToCut) != ' ' && indexToCut > 0)
        indexToCut--;
      
      pieces.add(toCut.substring(0, indexToCut));
      toCut = toCut.substring(indexToCut);
    }
    pieces.add(toCut);
    
    return pieces;
  }
  
}
