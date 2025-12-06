/******************************************************************
 * Simon Fraser University
 * ENSC-151 Introduction to Software Development for Engineers
 * audioProcessing.h --header file
 *
 * Description:
 *   Header file for the audio processing module. Declares the
 *   SongInfo struct and function prototypes used to read audio 
 *   files, compute loudness, and extract basic metadata.
 *
 * Author: Krishang Sarkar
 * Date: November 23, 2025
 *******************************************************************/
#ifndef AUDIO_PROCESSING_H
#define AUDIO_PROCESSING_H

#include <iostream>   // for input and output
#include <fstream>    // for file input and output
#include <vector>     // for dynamic memory handling
#include <string>     // for strings

using namespace std;

// Struct for storing the processed audio information
struct SongInfo {
    string filename;   // name of the song
    long fileSize;     // size of file in bytes
    double duration;   // estimated duration
    double loudness;   // average byte value for loudness
};

// Function declarations
vector<unsigned char> readBinaryFile(const string &filepath);
double computeLoudness(const vector<unsigned char> &data);
SongInfo processSong(const string &filepath);

#endif
