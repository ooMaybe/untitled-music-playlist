#include <iostream>
#include <iomanip>
using namespace std;

string finding_song(){
    string songName;
    cout << "Enter song name: ";
    getline(cin, songName);
    return songName;

}

string finding_artist(){
    string artistName;
    cout << "Enter artist name: ";
    getline(cin, artistName);
    return artistName;

}

string finding_genre(){
    string genre;
    cout << "Enter genre: ";
    getline(cin, genre);
    return genre;

}

string finding_cover(){
    string cover;
    cout << "Enter cover as a png: ";
    getline(cin, cover);
    return cover;

}

int main() {

    string songName = finding_song();
    string artistname = finding_artist();
    string genre = finding_genre();
    string cover = finding_cover();

    cout << "\nSong: " << songName << endl;
    cout << "By: " << artistname << endl;
    cout << "Genre: " << genre << endl;
    cout << "\n" << cover << endl;

    return 0;
    
}