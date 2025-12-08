
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct SongResult {
    string id;
    string title;
    string uploader;
    string duration;
    string thumbnail;
    string url;

};

void titleSort(vector<SongResult>& playlist) {

        sort(playlist.begin(), playlist.end(), [](const SongResult& a, const SongResult& b) {
            return a.title < b.title; //A-Z
        });

}

void displayPlaylist(const vector<SongResult>& playlist) {
    cout << "\n Sorted by title:\n";
    for (const auto& song : playlist) {
        cout << song.title << "-" << song.uploader << endl;
    }
}
