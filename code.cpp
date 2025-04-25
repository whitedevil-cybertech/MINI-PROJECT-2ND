#include <iostream>
#include <fstream>
#include <string>
#include <direct.h> // For Windows directory operations
#include <sys/stat.h> // For checking directory existence
#include <windows.h> // For Windows-specific functions

using namespace std;

// Function to create directory if it doesn't exist
bool createDirectory(const string& path) {
    size_t pos = 0;
    string dir;
    int ret = 0;

    while ((pos = path.find_first_of("/\\", pos + 1)) != string::npos) {
        dir = path.substr(0, pos);
        struct stat info;
        if (stat(dir.c_str(), &info) != 0) {
            ret = _mkdir(dir.c_str());
            if (ret != 0) return false;
        }
    }
    return true;
}

// XOR Encryption/Decryption Function
string xorEncryptDecrypt(const string &text, char key) {
    string result = text;
    for (size_t i = 0; i < text.size(); ++i) {
        result[i] ^= key; // XOR each character with the key
    }
    return result;
}

// Save Encrypted Data to File
void saveToFile(const string &filename, const string &data) {
    // Create directory if it doesn't exist
    if (!createDirectory(filename)) {
        cout << "Error creating directory!\n";
        return;
    }

    ofstream file(filename.c_str(), ios::binary);
    if (file.is_open()) {
        file.write(data.c_str(), data.size());
        file.close();
        cout << "File saved successfully at: " << filename << "\n";
    } else {
        cout << "Error opening file!\n";
    }
}

// Read Encrypted Data from File
string readFromFile(const string &filename) {
    ifstream file(filename.c_str(), ios::binary);
    if (!file.is_open()) {
        cout << "Error opening file!\n";
        return "";
    }
    string data((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return data;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Redirect standard output to console
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);

    int choice;
    string inputFilename, outputFilename, text, encryptedText, decryptedText;
    char xorKey = 'K'; // XOR encryption key

    while (true) {
        cout << "\n=== File Encryption/Decryption Program ===\n";
        cout << "1. Encrypt File\n";
        cout << "2. Decrypt File\n";
        cout << "3. Exit\n";
        cout << "Enter your choice (1-3): ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            cout << "Enter input filename to encrypt: ";
            getline(cin, inputFilename);
            
            // Read content from existing file
            text = readFromFile(inputFilename);
            if (text.empty()) {
                cout << "Error reading file or file is empty!\n";
                continue;
            }

            cout << "Enter output filename (with full path if needed): ";
            getline(cin, outputFilename);

            encryptedText = xorEncryptDecrypt(text, xorKey);
            saveToFile(outputFilename, encryptedText);

        } else if (choice == 2) {
            cout << "Enter input filename to decrypt: ";
            getline(cin, inputFilename);
            
            cout << "Enter output filename (with full path if needed): ";
            getline(cin, outputFilename);

            encryptedText = readFromFile(inputFilename);
            if (encryptedText.empty()) {
                cout << "Error reading file or file is empty!\n";
                continue;
            }

            decryptedText = xorEncryptDecrypt(encryptedText, xorKey);
            saveToFile(outputFilename, decryptedText);
        } else if (choice == 3) {
            cout << "Thank you for using the program. Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice! Please try again.\n";
        }
    }

    // Clean up console
    FreeConsole();
    return 0;
}