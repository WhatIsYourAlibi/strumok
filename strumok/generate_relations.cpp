#include <filesystem>
#include <fstream>

enum WordSize {
    Size64 = 64,
    Size32 = 32
};

void GenerateRelations(const int rounds, const WordSize wordSize) {
    std::ofstream file("strumok_relations.txt");
    if (!file.is_open()) return;
    std::string content = "#Strumok " + std::to_string(rounds) + " Rounds\n";
    content += "connection relations\n";
    switch (wordSize) {
    // Relations if we split 64-bit word by two 32-bit words
    case 32:
        for (int i = 0; i <= rounds; ++i) {
            content += "S_" + std::to_string(i + 16) + "_0, S_" + std::to_string(i + 13)
                + "_0, S_" + std::to_string(i + 11) + "_0, S_" + std::to_string(i) + "_0\n";
            content += "S_" + std::to_string(i + 16) + "_1, S_" + std::to_string(i + 13)
                + "_1, S_" + std::to_string(i + 11) + "_1, S_" + std::to_string(i) + "_1\n";
            content += "Z_" + std::to_string(i) + "_0, S_" + std::to_string(i + 15) + "_0, R_"
                + std::to_string(i + 1) + "_0, R_" + std::to_string(i) + "_0, S_" + std::to_string(i) + "_0\n";
            content += "Z_" + std::to_string(i) + "_1, S_" + std::to_string(i + 15) + "_1, R_"
                + std::to_string(i + 1) + "_1, R_" + std::to_string(i) + "_1, S_" + std::to_string(i) + "_1\n";
            content += "R_" + std::to_string(i + 2) + "_0, S_" + std::to_string(i + 13)
                + "_0, R_" + std::to_string(i) + "_0\n";
            content += "R_" + std::to_string(i + 2) + "_1, S_" + std::to_string(i + 13)
                + "_1, R_" + std::to_string(i) + "_1\n";
        }
        break;
    // Basic relations for 64-bit word
    case 64:
        for (int i = 0; i < rounds; ++i) {
            content += "S_" + std::to_string(i + 16) + ", S_" + std::to_string(i + 13)
                + ", S_" + std::to_string(i + 11) + ", S_" + std::to_string(i) + "\n";
            content += "Z_" + std::to_string(i) + ", S_" + std::to_string(i + 15) + ", R_" + std::to_string(i + 1)
                + ", R_" + std::to_string(i) + ", S_" + std::to_string(i) + "\n";
            content += "R_" + std::to_string(i + 2) + ", S_" + std::to_string(i + 13)
                + ", R_" + std::to_string(i) + "\n";
        }
        break;
    default:
        file.close();
        return;
    }
    content += "known\n";
    for (int i = 0; i < rounds; ++i) {
        content += "Z_" + std::to_string(i) + "\n";
    }
    content += "end";
    file << content;
    file.close();
}

int main() {
    GenerateRelations(11, Size64);
    return 0;
}
