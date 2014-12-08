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
  
  public void selectImages(Vector tuioObjectList, boolean force) {  
    if((millis() - enrichmentTimeCount) > enrichmentTimeoutMillis || force) {
      enrichmentTimeCount = millis();
      selection.clear();
      for (int i=0;i<tuioObjectList.size();i++) {
       TuioObject tobj = (TuioObject)tuioObjectList.elementAt(i);
       int id = tobj.getSymbolID();
       switch(id)
       {
         case 0: // police
           selection.add(police.get((int)(random(0, police.size()-1))));
           selection.add(police.get((int)(random(0, police.size()-1))));
           break;
         case 1: // protestors
           selection.add(protestors.get((int)(random(0, protestors.size()-1))));
           selection.add(protestors.get((int)(random(0, protestors.size()-1))));
           break;
         case 2: // politics
           selection.add(politics.get((int)(random(0, politics.size()-1))));
           selection.add(politics.get((int)(random(0, politics.size()-1))));
           break;
         case 4: // artists
           selection.add(artists.get((int)(random(0, artists.size()-1))));
           selection.add(artists.get((int)(random(0, artists.size()-1))));
           break;
         case 8: // strike
           selection.add(police.get((int)(random(0, police.size()-1))));
           break;
         case 10: // run
           selection.add(protestors.get((int)(random(0, protestors.size()-1))));
           break;
         case 11: // choke
           selection.add(protestors.get((int)(random(0, protestors.size()-1))));
           break;
         case 12: // fear
           selection.add(police.get((int)(random(0, police.size()-1))));
           break;
         case 14: // calmness
           selection.add(politics.get((int)(random(0, politics.size()-1))));
           break;
         case 16: // pride
           selection.add(protestors.get((int)(random(0, protestors.size()-1))));
           break;
         default:
           selection.add(places.get((int)(random(0, places.size()-1))));
           break;
       }
      }
      /*if(selection.size() < 2)
        selection.add(places.get((int)(random(0, places.size()-1))));  */  
      placesOnScreen.shuffle();
      enrichmentTimeoutMillis = selection.size()*3000;
     }
  }
  
  public void displaySelection() {
    int sizeW = width/7;
    for(int i=0; i<selection.size() && i<4; i++) {
      int x, y;
      if(placesOnScreen.get(i) == 0) {
        x = width/9;
        y = height/3;
        selection.get(i).drawAsPolaroid(x, y, sizeW);
      }
      if(placesOnScreen.get(i) == 1) {
        x = 8*width/9;
        y = 2*height/3;
        selection.get(i).drawAsPolaroid(x, y, sizeW);
      }
      if(placesOnScreen.get(i) == 2) {
        x = (int)(7.5*width/9);
        y = (int)(height/3.2);
        selection.get(i).drawAsPolaroid(x, y, sizeW);
      }
      if(placesOnScreen.get(i) == 3) {
        x = width/9;
        y = (int)(2.2*height/3);
        selection.get(i).drawAsPolaroid(x, y, sizeW);
      }
    }
  }
  
}