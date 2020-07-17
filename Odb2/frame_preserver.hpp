// Sometimes you want something on screen that lasts 5 frames not 1
class FramePreserver{
public:
    FramePreserver(int updateEvery): updateEvery(updateEvery) { }

    int value;
    int updateEvery;
    int counter;

    bool incrFrame(int newValue) {
        // return true if new value accepted

        counter = (counter + 1) % updateEvery;
        if(counter == 0) {
            value = newValue;
            return true;
        }
    }
};