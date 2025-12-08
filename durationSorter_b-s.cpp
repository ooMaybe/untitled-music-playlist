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

void durationSort(vector<SongResult>& playlist) {

        sort(playlist.begin(), playlist.end(), [](const SongResult& a, const SongResult& b) {
            return stoi(a.duration) > stoi(b.duration); //biggest to shortest
        });

}

void displayPlaylist(const vector<SongResult>& playlist) {
    cout << "\n Sorted by duration:\n";
    for (const auto& song : playlist) {
        cout << song.title << "-" << song.duration << " seconds" << endl;
    }
}
