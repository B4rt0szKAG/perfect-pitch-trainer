#ifndef PLAYER_H
#define PLAYER_H
#include <filesystem>
#include <string>
#include <vector>

class Player {

    std::string type;
    std::string notes;
    int number_of_rounds;

    public:
    static const std::filesystem::path types_path;
    static const std::filesystem::path notes_path;
    Player(const int n, const std::string &t): type(t), number_of_rounds(n) {
        load_notes();
    }
    std::string get_type() { return type; }
    void set_type(const std::string &t) { type = t; }
    int get_number_of_rounds() const { return number_of_rounds; }
    std::string get_notes() const { return notes; }
    void set_number_of_rounds(const size_t n) { number_of_rounds = n; }
    void set_notes(const std::string &n) { notes = n; }
    void load_notes();
    void delete_note(const std::string &note);
    void add_note(const std::string &note);

    static void play_note(const std::string &note);

};

#endif
