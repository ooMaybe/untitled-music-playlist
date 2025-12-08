
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
using namespace std;

struct SongResult {
    string id;
    string title;
    string uploader;
    string duration;
    string thumbnail;
    string url;

};


void displayPlaylist(const vector<SongResult>& playlist) {
    cout << "\n Regular Playlist order:\n";
    for (const auto& song : playlist) {
        cout << song.title << "-" << song.uploader << endl;
    }
}

void randomize(vector<SongResult>& playlist) {
    random_device rd;
    mt19937 g(rd());
    shuffle(playlist.begin(), playlist.end(), g);

}
