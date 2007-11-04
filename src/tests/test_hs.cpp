#include<iostream>

using namespace std;

#include "highscore.h"
int main() {

    Highscore hs;
    hs.addScore(15, "Nikhil");
    hs.addScore(20, "NSM");

    hs.write("scores.txt");

    //hs.read("scores.txt");

    vector<Score> v = hs.getScores();
    for(int i = 0; i < v.size(); ++i)
        cout<<v[i];
    return 0;
}

