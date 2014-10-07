public class UserKISD {
  int faceTracked;
  int id;
  int screenWatched;
  float x, y;
  ArrayList<Float> X, Y;
  int filterSize;
  int interest;
  int r, g, b;
  
  // initialize user's variables
  UserKISD(int t, int p_id, int s, int i) {
    faceTracked = t;
    id = p_id;
    screenWatched = s;
    x = 0;
    y = 0;
    filterSize = 9;
    X = new ArrayList<Float>();
    Y = new ArrayList<Float>();
    
    interest = i;
    
   /* switch(id) {
      case 0:
        r = 96;
        g = 0;
        b = 0;
        break;
      case 1:
        r = 0;
        g = 96;
        b = 0;
        break;
      case 2:
        r = 0;
        g = 0;
        b = 96;
        break;
      case 3:
        r = 96;
        g = 96;
        b = 0;
        break;
      case 4:
        r = 0;
        g = 96;
        b = 96;
        break;
      case 5:
        r = 96;
        g = 0;
        b = 96;
        break;
      default:
        r = 0;
        g = 0;
        b = 0;
        break;
    }*/
  }
  
  String toString() {
    String str = "User " + id + "\n";
    str += ("faceTracked=" + faceTracked);
    str += (" - screenWatched=" + screenWatched);
    str += (" - interest=" + interest);
    str += "\n";
    return str; 
  }
  
  // used when new coordinates are got
  void pushCoordXY(float p_x, float p_y) {
    X.add(p_x);
    Y.add(p_y);
    if(X.size() > filterSize)
      X.remove(0);
    if(Y.size() > filterSize)
      Y.remove(0);
      
    x = 0; y = 0;
    for(int i=0; i<X.size(); i++)
      x += X.get(i);
    for(int i=0; i<Y.size(); i++)
      y += Y.get(i);

    x /= X.size();
    y /= Y.size();
  }
  
  // used when the user quits the installation
  void clearValues() {
    faceTracked = 0;
    screenWatched = 0;
    x = 0;
    y = 0;
    X.clear();
    Y.clear(); 
    interest = 0;
  }
}
