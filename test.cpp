
class MusicPlaylist {
private:
    vector<string> songs;
public:
    void addSong(const string& song) {
    #include <random>
    #include <chrono>
    #include <sstream>
    #include <iomanip>

    // RandomTestNummy: fills a MusicPlaylist with pseudo-random numeric song names.
    // Place this after your MusicPlaylist class definition.
    class RandomTestNummy {
    public:
        // Generate 'count' songs named like "Track 023" and add them to playlist.
        static void fill(MusicPlaylist &playlist, size_t count) {
            auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            std::mt19937 rng(static_cast<unsigned>(seed));
            std::uniform_int_distribution<int> dist(0, 999);

            for (size_t i = 0; i < count; ++i) {
                std::ostringstream ss;
                ss << "Track " << std::setw(3) << std::setfill('0') << dist(rng);
                playlist.addSong(ss.str());
            }
        }
    };

    // Example usage (add to your test.cpp main or test harness):
    /*
    int main() {
        MusicPlaylist pl;
        pl.displayMessage();
        RandomTestNummy::fill(pl, 10);
        pl.displaySongs();
        return 0;
    }
    */
    }

    void displaySongs() const {
        cout << "Songs in the playlist:" << endl;
        for (const auto& song : songs) {
            cout << "- " << song << endl;
        }
    }       
};