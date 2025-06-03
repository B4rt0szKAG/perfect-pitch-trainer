#include <iostream>
#include <filesystem>
#include <sstream>
#include "player.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <random>

constexpr int DEFAULT_ROUND_NUMBER_VALUE = 10;
constexpr std::string DEFAULT_SCALE = "C_major";

std::string read_input() {
    char* input = readline(">>> ");
    if (!input) return "";

    if (*input) add_history(input);
    std::string result(input);
    free(input);
    return result;
}

void help1() {
    std::cout << "Available commands:\n"
              << "  settings  - change default settings\n"
              << "  start     - start the game\n"
              << "  quit      - exit the program\n"
              << "  credits   - show program credits\n" << std::endl;
}

void help2() {
    std::cout << "Available commands:\n"
              << "  load <scale>     - load a scale\n"
              << "  list_scales      - list all available scales\n"
              << "  list_notes       - list all available notes\n"
              << "  list_cur_notes   - list all current notes\n"
              << "  add <note>       - add a single note\n"
              << "  del <note>       - delete a single note\n"
              << "  num <number>     - change the number of rounds\n"
              << "  quit             - exit to menu\n";
}

void help3() {
    std::cout << "Available commands:\n"
              << "  list_cur_notes   - list all current notes\n"
              << "  <note>     - input your answer (e.g., C4, D5...)\n"
              << "  again      - hear the note again\n"
              << "  skip       - go to the next round immediately\n"
              << "  quit       - exit to the main menu\n"
              << std::endl;
}


void list_scales() {
    std::cout << "Currently available scales:\n" << "C_major" << std::endl;
}

void list_notes() {
    std::cout << "All available notes:\n";
    for (const auto &entry : std::filesystem::directory_iterator(Player::notes_path)) {
        if (entry.path().extension() == ".wav") {
            std::cout << "  - " << entry.path().stem().string() << '\n';
        }
    }
}

void add_note(Player &player, const std::string &note) {
    if (note.size() != 2 || !std::isupper(note[0]) || !std::isdigit(note[1]) || note[1] > '5' || note[1] < '4') {
        std::cout << "Invalid note format. Use format like 'A4', 'C5'...\n";
        return;
    }
    player.add_note(note);
    std::cout << "Note added: " << note << '\n';
}

void del_note(Player &player, const std::string &note) {
    if (note.size() != 2 || !std::isupper(note[0]) || !std::isdigit(note[1]) || note[1] > '5' || note[1] < '4') {
        std::cout << "Invalid note format. Use format like 'A4', 'C5'...\n";
        return;
    }
    player.delete_note(note);
    std::cout << "Note deleted: " << note << '\n';
}

void set_number_of_rounds(Player &player, const std::string &arg) {
    try {
        const size_t rounds = std::stoul(arg);
        player.set_number_of_rounds(rounds);
        std::cout << "Number of rounds set to: " << rounds << '\n';
    } catch (const std::exception &e) {
        std::cout << "Invalid number. Please enter a positive integer.\n";
    }
}

void settings(Player &player) {
    std::string line, cmd, arg;

    std::cout << "You are currently in settings.\n"
              << "Type \"help\" for available commands.\n";

    while (true) {
        line = read_input();
        std::istringstream iss(line);
        iss >> cmd >> arg;

        if (cmd == "help") {
            help2();
        } else if (cmd == "quit") {
            return;
        } else if (cmd == "load") {
            player.set_type(arg);
            player.load_notes();
            std::cout << "Loaded scale: " << arg << '\n';
        } else if (cmd == "list_scales") {
            list_scales();
        } else if (cmd == "list_notes") {
            list_notes();
        } else if (cmd == "list_cur_notes") {
            std::cout << "Current notes: " << player.get_notes() << '\n';
        } else if (cmd == "add") {
            add_note(player, arg);
        } else if (cmd == "del") {
            del_note(player, arg);
        } else if (cmd == "num") {
            set_number_of_rounds(player, arg);
        } else {
            std::cout << "Unknown command. Type \"help\" for options.\n";
        }
    }
}

void play(const Player &player) {
    std::size_t round = 0;
    std::size_t draw_size = player.get_notes().size() / 2;
    std::string curr_note;
    std::string line;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, draw_size - 1);

    bool flag = true;
    int curr = dist(gen);

    std::cout << "You are currently playing the game.\n"
              << "Type \"help\" for available commands.\n"
              << " Round: 0\n";

    while (round < player.get_number_of_rounds()) {
        if (flag) {
            curr = dist(gen);
            curr_note = player.get_notes().substr(2 * curr, 2);
            Player::play_note(curr_note);
            flag = false;
        }

        line = read_input();

        if (line == "help") {
            help3();
        } else if (line == "quit") {
            return;
        } else if (line == "again") {
            Player::play_note(curr_note);
        } else if (line == "skip") {
            round++;
            flag = true;
            continue;
        } else if (line == "list_cur_notes") {
            std::cout << "Current notes: " << player.get_notes() << '\n';
        } else if (line.size() == 2 && std::isupper(line[0]) && std::isdigit(line[1])) {
            if (line == curr_note) {
                round++;
                std::cout << "Congratulations, correct note!\n Round:" << round << "\n" ;
                flag = true;
            } else {
                std::cout << "Wrong answer, try again.\n";
                Player::play_note(line);
            }
        } else {
            std::cout << "Invalid command or note format. Type \"help\" for help.\n";
        }
    }

    std::cout << "Game over! You finished " << round << " rounds.\n";
}

int main() {
    std::cout << "Welcome in perfect pitch trainer menu" << std::endl;
    std::cout << "If you want more information input \"help\" " << std::endl;

    Player player(DEFAULT_ROUND_NUMBER_VALUE, DEFAULT_SCALE);

    while (true) {
        std::string input = read_input();

        if (input == "help") {
            help1();
        } else if (input == "quit") {
            break;
        } else if (input == "settings") {
            settings(player);
        } else if (input == "start") {
            play(player);
        } else {
            std::cout << "Invalid input!" << std::endl;
        }
    }
    return 0;
}
