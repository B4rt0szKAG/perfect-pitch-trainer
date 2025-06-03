#include "player.h"

#include <cstring>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>

const std::filesystem::path Player::types_path = "./resources/types.txt";
const std::filesystem::path Player::notes_path = "./resources/notes";


void Player::load_notes() {
    std::ifstream file(types_path);
    std::string line;
    const std::string type = get_type();

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + types_path.string());
    }

    while (std::getline(file, line)) {
        if (!line.empty() && line[0] == '#') continue;
        if (line.rfind(type + ":", 0) == 0) {
            set_notes(line.substr(type.size() + 1));
            return;
        }
    }
    throw std::runtime_error("Failed to load notes of type: " + type);
}

void Player::play_note(const std::string &note) {
    const std::string cmd = "aplay \"" + notes_path.string() + "/" + note + ".wav\" > /dev/null 2>&1";
    system(cmd.c_str());
}

void Player::delete_note(const std::string &note) {
    if (note.size() != 2 || !isupper(note[0]) || !isdigit(note[1])) {
        throw std::runtime_error("Not a valid note: " + note);
    }

    std::size_t pos = notes.find(note);
    if (pos != std::string::npos) {
        notes.erase(pos, note.size());
    } else {
        std::cout << "note is not on a list" << std::endl;
    }
}

void Player::add_note(const std::string &note) {
    if (note.size() != 2 || !isupper(note[0]) || !isdigit(note[1])) {
        throw std::runtime_error("Not a valid note: " + note);
    }

    if (notes.find(note) != std::string::npos) {
        std::cout << "Note already exists in the list.\n";
        return;
    }
    notes += note;
}