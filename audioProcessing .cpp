/******************************************************************
 * Simon Fraser University
 * ENSC-151 Introduction to Software Development for Engineers
 * audioProcessing.cpp --
 *
 * Description:
 *   This module handles basic audio processing for the Music
 *   Playlist Manager project. It reads audio files (mp3, wav, etc.)
 *   as binary data and extracts simple metadata such as:
 *      - File size
 *      - Estimated duration
 *      - Average loudness (byte-level)
 *
 * Input:
 *   Audio file path (string)
 *
 * Output:
 *   Struct containing processed song information
 *
 * Author: Krishang Sarkar
 * Date: November 23, 2025
 *******************************************************************/
// libraries,preproccesser directive  and header files used in the program
#include <iostream> //for input and output
#include <fstream>  //file input and output
#include <vector>   // dynamic memory
#include <string>   //for strings in the program and processing
using namespace std;

// ------------------------------------------------------------
// SongInfo: A simple struct used to store processed audio data
// ------------------------------------------------------------
//
// This struct holds key metadata extracted from an audio file.
// It is used by the audio processing module and returned to
// other components (UI, playlist manager, etc.) for display or
// storing in the app's database.
//
// Fields:
//   - filename : Name/path of the audio file
//   - fileSize : Size of the file in bytes
//   - duration : Estimated duration (based on file size)
//   - loudness : Average byte value, used as a simple loudness metric
//
struct SongInfo
{
    string filename; // Name or path of the audio file
    long fileSize;   // Total file size in bytes
    double duration; // Estimated duration of the song (in seconds)
    double loudness; // Calculated average loudness (byte-level)
};

// function declaration for the program
SongInfo processSong(const string &filepath);
double computeLoudness(const vector<unsigned char> &data);
vector<unsigned char> readBinaryFile(const string &filepath);

// Demo main (remove when integrating with other features of the app)
int main()
{
    string file = "song.mp3";

    SongInfo s = processSong(file);

    cout << "----- Audio Processing Result -----" << endl;
    cout << "Filename: " << s.filename << endl;
    cout << "Size: " << s.fileSize << " bytes" << endl;
    cout << "Estimated Duration: " << s.duration << " sec" << endl;
    cout << "Loudness: " << s.loudness << endl;

    return 0;
}
// function implementation
//  Read raw bytes from file
vector<unsigned char> readBinaryFile(const string &filepath)
{
    //file input 
    ifstream file(filepath, ios::binary);

    vector<unsigned char> buffer;

    //to check sucsess of the file when opening     
    if (!file.is_open())
    {
        cout << "Error: Could not open file: " << filepath << endl;
        return buffer;
    }

    // Read all bytes of the fille 
    file.seekg(0, ios::end);
    long size = file.tellg();
    file.seekg(0, ios::beg);

    buffer.resize(size);
    file.read(reinterpret_cast<char *>(buffer.data()), size);

    return buffer;
}

/******************************************************************
 * computeLoudness-- Calculates "loudness" of the sound  = average byte value
 * Parameters:
 *  const vector<unsigned char> &data
 *  used where data is as refrence varameter
 * Modifies:
 * Returns: the loudness
 *******************************************************************/
double computeLoudness(const vector<unsigned char> &data)
{
    if (data.empty())
        return 0.0;

    long long sum = 0;

    for (unsigned char c : data)
    {
        sum += c;
    }

    return (double)sum / data.size();
}

// through calling the struct and refrencing the filepath
/******************************************************************
 * processSong -- Reads a file and extracts simple audio metadata
 *
 * Parameters:
 *   const string &filepath
 *
 * Returns:
 *   SongInfo struct containing filename, size, loudness, duration
 *******************************************************************/
SongInfo processSong(const string &filepath)
{
    vector<unsigned char> data = readBinaryFile(filepath);

    SongInfo info;
    info.filename = filepath;
    info.fileSize = data.size();
    info.duration = info.fileSize / 50000.0; // fake estimate
    info.loudness = computeLoudness(data);

    return info;
}
