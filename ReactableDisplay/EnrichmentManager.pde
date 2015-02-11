import TUIO.*;
import java.util.Vector;

public class EnrichmentManager {    
  
  private ArrayList<EnrichmentImage> artists;
  private ArrayList<EnrichmentImage> places;
  private ArrayList<EnrichmentImage> police;
  private ArrayList<EnrichmentImage> politics;
  private ArrayList<EnrichmentImage> protestors;
  private ArrayList<EnrichmentImage> selection;
  private IntList placesOnScreen;
  private int enrichmentTimeoutMillis;
  private int enrichmentTimeCount = 0;
  
  public EnrichmentManager(String imgagesPath) {
    enrichmentTimeoutMillis = 0;
    enrichmentTimeCount = millis();
    
    artists = new ArrayList<EnrichmentImage>();
    places = new ArrayList<EnrichmentImage>();
    police = new ArrayList<EnrichmentImage>();
    politics = new ArrayList<EnrichmentImage>();
    protestors = new ArrayList<EnrichmentImage>();
    selection = new ArrayList<EnrichmentImage>();
    
    placesOnScreen = new IntList();
    for(int i=0; i<4; i++) {
      placesOnScreen.append(i);
    }
    
    String[] directories = {"artist", "place", "police", "politic", "protestors"};
    for(int i=0; i<directories.length; i++) {  
      list_directory list = new list_directory(imgagesPath+"/"+directories[i], "png");
      
      for(int j=0; j<list.nb_items; j++) {        
        String imgPath = list.fichiers[j];
        String txtPath = imgPath.replaceFirst(".png", ".txt");
        
        switch(i) {
          case 0: artists.add(new EnrichmentImage(imgPath, txtPath, directories[i]));
            break;
          case 1: places.add(new EnrichmentImage(imgPath, txtPath, directories[i]));
            break;
          case 2: police.add(new EnrichmentImage(imgPath, txtPath, directories[i]));
            break;
          case 3: politics.add(new EnrichmentImage(imgPath, txtPath, directories[i]));
            break;
          case 4: protestors.add(new EnrichmentImage(imgPath, txtPath, directories[i]));
            break;
          default:
            break;
        }
      }
    }
  }
  
  public void selectImages(StringList identifiedKeywords, boolean force) {  
    if((millis() - enrichmentTimeCount) > enrichmentTimeoutMillis || force) {
      enrichmentTimeCount = millis();
      
      for(int i=0; i<selection.size(); i++) {
        selection.get(i).freeImage();
      }
      selection.clear();
      
      for (int i=0;i<identifiedKeywords.size();i++) {
       String keyword = identifiedKeywords.get(i);
       if(keyword.equals("police")) {
           selection.add(police.get((int)(random(0, police.size()-1))));
           selection.add(police.get((int)(random(0, police.size()-1))));
       } else if(keyword.equals("chapullers")) {
           selection.add(protestors.get((int)(random(0, protestors.size()-1))));
           selection.add(protestors.get((int)(random(0, protestors.size()-1))));
       } else if(keyword.equals("politics")) {
           selection.add(politics.get((int)(random(0, politics.size()-1))));
           selection.add(politics.get((int)(random(0, politics.size()-1))));
       } else if(keyword.equals("artists")) {
           selection.add(artists.get((int)(random(0, artists.size()-1))));
           selection.add(artists.get((int)(random(0, artists.size()-1))));
       } else if(keyword.equals("strike")) {
           selection.add(police.get((int)(random(0, police.size()-1))));
       } else if(keyword.equals("run")) {
           selection.add(protestors.get((int)(random(0, protestors.size()-1))));
       } else if(keyword.equals("choke")) {
           selection.add(protestors.get((int)(random(0, protestors.size()-1))));
       } else if(keyword.equals("fear")) {
           selection.add(police.get((int)(random(0, police.size()-1))));
       } else if(keyword.equals("calmness")) {
           selection.add(politics.get((int)(random(0, politics.size()-1))));
       } else if(keyword.equals("pride")) {
           selection.add(protestors.get((int)(random(0, protestors.size()-1))));
       } else {
           selection.add(places.get((int)(random(0, places.size()-1))));
       }
      }
      /*if(selection.size() < 2)
        selection.add(places.get((int)(random(0, places.size()-1))));  */
      for(int i=0; i<selection.size(); i++) {
        selection.get(i).loadImageFromFile();
      }
      
      placesOnScreen.shuffle();
      enrichmentTimeoutMillis = selection.size()*2500;
     }
  }
  
  public void displaySelection(PGraphics screen) {
    int sizeW = screen.width/7;
    for(int i=0; i<selection.size() && i<4; i++) {
      int x, y;
      if(placesOnScreen.get(i) == 0) {
        x = screen.width/9;
        y = screen.height/3;
        selection.get(i).drawAsPolaroid(screen, x, y, sizeW);
      }
      if(placesOnScreen.get(i) == 1) {
        x = 8*screen.width/9;
        y = 2*screen.height/3;
        selection.get(i).drawAsPolaroid(screen, x, y, sizeW);
      }
      if(placesOnScreen.get(i) == 2) {
        x = (int)(7.5*screen.width/9);
        y = (int)(screen.height/3.2);
        selection.get(i).drawAsPolaroid(screen, x, y, sizeW);
      }
      if(placesOnScreen.get(i) == 3) {
        x = screen.width/9;
        y = (int)(2.2*screen.height/3);
        selection.get(i).drawAsPolaroid(screen, x, y, sizeW);
      }
    }
  }
  
}
