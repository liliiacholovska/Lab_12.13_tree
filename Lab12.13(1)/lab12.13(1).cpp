#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>  

using namespace std;

// Structure to store information about a guest
struct Note {
    string lastName;
    string firstName;
    string phoneNumber;
    int birthDate[3]; // day, month, year
};

// Node of the Trie
class TreeNode {
public:
    unordered_map<char, TreeNode*> children;
    vector<Note> notes;
    bool isEndOfWord;

    TreeNode() : isEndOfWord(false) {}
};

// Trie class
class Tree {
private:
    TreeNode* root;

    // Helper function for saving to file
    void saveHelper(TreeNode* node, ofstream& outFile, string currentPrefix) {
        if (node->isEndOfWord) {
            for (const auto& note : node->notes) {
                outFile << currentPrefix << " " << note.firstName << " " << note.phoneNumber << " " << note.birthDate[0] << " " << note.birthDate[1] << " " << note.birthDate[2] << "\n";
            }
        }

        for (const auto& child : node->children) {
            saveHelper(child.second, outFile, currentPrefix + child.first);
        }
    }

    // Helper function for loading from file
    void loadHelper(const string& line) {
        istringstream iss(line);
        Note note;
        iss >> note.lastName >> note.firstName >> note.phoneNumber >> note.birthDate[0] >> note.birthDate[1] >> note.birthDate[2];
        string fullName = note.firstName + " " + note.lastName;

        // Check if the note already exists in the tree
        if (!findNoteByFullName(fullName)) {
            addNote(note);
        }
    }

    // Find guest note by full name
    bool findNoteByFullName(const string& fullName) {
        // Traverse the tree to find the note by full name
        for (const auto& note : root->notes) {
            if ((note.firstName + " " + note.lastName) == fullName) {
                return true;
            }
        }
        return false;
    }

public:
    Tree() : root(new TreeNode()) {}

    // Add a guest note
    void addNote(const Note& note) {
        TreeNode* currentNode = root;
        for (char ch : note.lastName) {
            if (!currentNode->children[ch]) {
                currentNode->children[ch] = new TreeNode();
            }
            currentNode = currentNode->children[ch];
        }
        currentNode->isEndOfWord = true; 
        currentNode->notes.push_back(note);
    }

    // Find guest note by last name
    bool findNoteByLastName(const string& lastName, vector<Note>& result) {
        TreeNode* currentNode = root;
        for (char ch : lastName) {
            if (!currentNode->children[ch]) {
                return false;
            }
            currentNode = currentNode->children[ch];
        }

        if (currentNode->isEndOfWord) {
            result = currentNode->notes;
            return true;
        }
        return false;
    }

    // Delete guest note
    void deleteNote(const string& lastName, const string& firstName) {
        TreeNode* currentNode = root;
        for (char ch : lastName) {
            if (!currentNode->children[ch]) return;
            currentNode = currentNode->children[ch];
        }
        if (currentNode->isEndOfWord) {
            auto& notes = currentNode->notes;
            notes.erase(remove_if(notes.begin(), notes.end(), [&](const Note& n) {
                return n.firstName == firstName;
                }), notes.end());

            if (notes.empty()) {
                currentNode->isEndOfWord = false;
            }
        }
    }


    // Save to file
    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            saveHelper(root, outFile, "");
            outFile.close();
            cout << "Data saved to file successfully.\n";
        }
        else {
            cerr << "Error opening file for writing.\n";
        }
    }

    // Load from file
    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                loadHelper(line);
            }
            inFile.close();
        }
        else {
            cerr << "Error opening file for reading.\n";
        }
    }

    // Display all guest notes
    void displayAllNotes() {
        displayAllNotesHelper(root, "");
    }

private:
    // Helper function to display all guest notes
    void displayAllNotesHelper(TreeNode* node, string currentPrefix) {
        if (node->isEndOfWord) {
            for (const auto& note : node->notes) {
                cout << currentPrefix << " " << note.firstName << " " << note.phoneNumber << " " << note.birthDate[0] << " " << note.birthDate[1] << " " << note.birthDate[2] << "\n";
            }
        }

        for (const auto& child : node->children) {
            displayAllNotesHelper(child.second, currentPrefix + child.first);
        }
    }
};

// Function to input guest information from keyboard
Note inputNoteInfo() {
    Note note;
    cout << "Enter last name: ";
    cin >> note.lastName;
    cout << "Enter first name: ";
    cin >> note.firstName;
    cout << "Enter phone number: ";
    cin >> note.phoneNumber;
    cout << "Enter birth date (day month year): ";
    for (int i = 0; i < 3; ++i) {
        while (!(cin >> note.birthDate[i])) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid date: ";
        }
    }
    return note;
}

int main() {
    Tree trie;
    int choice;
    do {
        cout << "1. Add a guest note\n";
        cout << "2. Delete a guest note\n";
        cout << "3. Find a guest note by last name\n";
        cout << "4. Save data to a file\n";
        cout << "5. Load data from a file\n";
        cout << "6. Display all guest notes\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            Note note = inputNoteInfo();
            trie.addNote(note);
            break;
        }
        case 2: {
            string lastName, firstName;
            cout << "Enter last name: ";
            cin >> lastName;
            cout << "Enter first name: ";
            cin >> firstName;
            trie.deleteNote(lastName, firstName);
            break;
        }
        case 3: {
            string lastName;
            cout << "Enter last name: ";
            cin >> lastName;
            vector<Note> result;
            if (trie.findNoteByLastName(lastName, result)) {
                for (const auto& note : result) {
                    cout << "Found: " << note.lastName << " " << note.firstName << " " << note.phoneNumber << " " << note.birthDate[0] << "." << note.birthDate[1] << "." << note.birthDate[2] << endl;
                }
            }
            else {
                cout << "No guest notes found with last name " << lastName << ".\n";
            }
            break;
        }
        case 4: {
            string filename;
            cout << "Enter the filename to save: ";
            cin >> filename;
            trie.saveToFile(filename);
            break;
        }
        case 5: {
            string filename;
            cout << "Enter the filename to load: ";
            cin >> filename;
            trie.loadFromFile(filename);
            break;
        }
        case 6: {
            cout << "Displaying all guest notes:\n";
            trie.displayAllNotes();
            break;
        }
        case 7:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 7);
    return 0;
}