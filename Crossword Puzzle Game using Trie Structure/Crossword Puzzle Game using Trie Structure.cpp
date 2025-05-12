#include <iostream>   
#include <string>      
#include <chrono>      // For measuring time
#include <cmath>       
using namespace std;

// Class representing a single node in the Trie
class TrieNode {
public:
    TrieNode* children[26];
    bool isEndOfWord;
    int clueIndex;          // Index of the clue associated with the word


    TrieNode() {
        isEndOfWord = false;
        clueIndex = -1;
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};


class Trie {
private:
    TrieNode* root;

public:

    Trie() {
        root = new TrieNode();
    }

    // Method to insert a word into the Trie along with its clue index
    void insert(const string& word, int clue) {
        TrieNode* curr = root; // Start from the root node
        for (char c : word) {  // Iterate through each character in the word
            int index = c - 'A'; // Calculate the index (0-25) for the character
            if (!curr->children[index]) { // If the child doesn't exist
                curr->children[index] = new TrieNode(); // Create a new child node
            }
            curr = curr->children[index]; // Move to the child node
        }
        curr->isEndOfWord = true;
        curr->clueIndex = clue;   // Set the clue index
    }

    // Method to search for a word and retrieve its clue index if found
    bool search(const string& word, int& clue) {
        TrieNode* ptr = root; // Start from the root node
        for (char c : word) { // Iterate through each character in the word
            int index = c - 'A'; // Calculate the index for the character
            if (!ptr->children[index]) { // If the child doesn't exist
                return false; // Word is not found
            }
            ptr = ptr->children[index]; // Move to the child node
        }
        if (ptr != nullptr && ptr->isEndOfWord) { // Check if it's the end of a word
            clue = ptr->clueIndex; // Retrieve the clue index
            return true; // Word found
        }
        return false; // Word not found
    }

    // Destructor to clean up the Trie
    ~Trie() {
        deleteTrie(root); // Delete the root node (and all its children)
    }

    // Recursive function to delete all nodes in the Trie
    void deleteTrie(TrieNode* node) {
        for (int i = 0; i < 26; i++) { // Iterate through all possible children
            if (node->children[i]) { // If the child exists
                deleteTrie(node->children[i]); // Recursively delete the child
            }
        }
        delete node; // Delete the current node
    }
};

// Class to manage the crossword puzzle
class Crossword {
    string currentAnswer; // Store the current answer from the user
    int count;            // Count of solved clues
    Trie trie;           // Trie to store clues
    int score;           // Score based on correct answers
    string answers[10];  // Array to hold answers for 10 clues
    bool solvedClues[10]; // Array to track whether clues are solved

public:

    Crossword() {
        currentAnswer = "";
        count = 0;
        score = 0;
        for (int i = 0; i < 10; i++) {
            answers[i] = "       "; // Initialize answers with empty strings
            solvedClues[i] = false; // Mark all clues as unsolved
        }
    }

    // Method to assign clues to the Trie
    void assign(const string clues[]) {
        for (int i = 0; i < 10; i++) {
            trie.insert(clues[i], i + 1); // Insert each clue with its index
        }
    }

    // Method to check the user's answer for a specific clue
    void check(int clue) {
        if (solvedClues[clue - 1]) { // Check if the clue has already been solved
            cout << "\nThis clue has already been solved! Choose another clue.\n" << endl;
            return; // Exit the method
        }

        cout << "Enter your answer (IN CAPITALS ONLY): "; // Prompt for answer
        auto start = chrono::high_resolution_clock::now(); // Start timing
        cin >> currentAnswer;

        int clueIndex; // Variable to store the clue index
        if (trie.search(currentAnswer, clueIndex)) { // Search for the answer in the Trie
            cout << endl << "!! Correct answer !!" << endl; // Notify correct answer
            count++; // Increment count of solved clues
            answers[clueIndex - 1] = currentAnswer; // Store the correct answer
            solvedClues[clueIndex - 1] = true; // Mark the clue as solved
            score += pow(2, currentAnswer.length()); // Calculate score based on answer length
            cout << "Score: " << score << endl; // Display score
        }
        else {
            cout << "\n!! Incorrect answer !!" << endl; // Notify incorrect answer
        }

        auto end = chrono::high_resolution_clock::now(); // End timing
        chrono::duration<double> duration = end - start; // Calculate duration
        cout << "Time taken for this clue: " << duration.count() << " seconds" << endl; // Display time taken
    }

    // Method to display the current answers and score
    void display() {
        cout << "\n====================== CROSSWORD ANSWERS ======================\n";
        for (int i = 0; i < 10; i++) {
            cout << "Clue " << (i + 1) << ": " << answers[i] << endl; // Display each clue's answer
        }
        cout << "===============================================================" << endl;
        cout << "Current score: " << score << endl; // Display current score
        if (count == 10) { // Check if all clues are solved
            cout << "\n!!!! YOU HAVE COMPLETED THE CROSSWORD. CONGRATULATIONS !!!!\n";
        }
    }

    // Method to get the count of solved clues
    int getCount() const {
        return count; // Return the count
    }
};

// Class to manage multiple Crossword instances
class Multiple {
    Crossword c[5]; // Array of 5 Crossword objects
    int clue; // Variable to store the selected clue index

public:

    Multiple() {
        clue = 0; // Initialize clue index to 0
    }

    // Method to handle the crossword clue-solving process
    void clues() {
        // Define clues for 5 crosswords
        const string clues[5][10] = {
            {"WORSE", "BLEAT", "FLOW", "ROBE", "LEAP", "TALE", "MEAN", "DISEASE", "ONCE", "PAN"},
            {"REMOTE", "ITEM", "ARM", "HORSE", "ALLERGY", "MELON", "WASP", "BREAD", "READ", "PLATE"},
            {"BAKER", "MANE", "SPACE", "DIAPERS", "WIDER", "DANGER", "GLARE", "ASLEEP", "SEAL", "LAIR"},
            {"COAST", "ELBOW", "LISTEN", "SPELT", "EDIT", "SERPENT", "STEAK", "ERA", "SOFTER", "TRAITS"},
            {"STEAL", "ENLARGE", "STAIN", "HEART", "SHAPE", "ALERT", "TABLES", "SALT", "SILVER", "OWN"}
        };

        // Assign clues to each Crossword object
        for (int i = 0; i < 5; i++)
            c[i].assign(clues[i]);  // Call the assign method for each crossword

        int ch, ch2; // Variables for user input for crossword selection and next action
        do {
            cout << "\tWhich crossword do you wish to solve?\n" << endl; // Prompt for crossword selection
            cout << "\t1. Crossword 1\n\t2. Crossword 2\n\t3. Crossword 3\n\t4. Crossword 4\n\t5. Crossword 5\n\t0. Exit" << endl;

            // Loop for valid crossword selection
            while (true) {
                cin >> ch; // Read user input
                if (ch >= 0 && ch <= 5) break; // Break if input is valid
                cout << "Invalid index. Please select a valid crossword (0-5): "; // Prompt for valid input
            }

            if (ch != 0) { // If user did not choose to exit
                cout << endl << "\t\t****** CLUES *******" << endl << endl;
                auto total_start = chrono::high_resolution_clock::now(); // Start total timer

                do {
                    // Display clues based on the selected crossword
                    switch (ch) {
                    case 1:
                        cout << "\t1.  More than bad (WORSE)\n\t2.  Sound made by a sheep (BLEAT)\n\t3.  Movement of water (FLOW)\n\t4.  A long dress (ROBE)"
                            << "\n\t5.  To jump (LEAP)\n\t6.  A story (TALE)\n\t7.  Average (MEAN)\n\t8.  A sickness (DISEASE)\n\t9.  One time (ONCE)\n\t10. Vessel (PAN)"
                            << endl;
                        break;
                    case 2:
                        cout << "\t1.  Far away (REMOTE)\n\t2.  A thing on a list (ITEM)\n\t3.  A limb (ARM)\n\t4.  An animal we ride (HORSE)"
                            << "\n\t5.  Something that makes you sneeze (ALLERGY)\n\t6.  A juicy fruit (MELON)\n\t7.  An insect that stings (WASP)\n\t8.  Something you bake (BREAD)"
                            << "\n\t9.  We ____ a book (READ)\n\t10. Something you serve on (PLATE)" << endl;
                        break;
                    case 3:
                        cout << "\t1.  A person who makes bread (BAKER)\n\t2.  Hair around a lion's neck (MANE)\n\t3.  A place to see stars and planets (SPACE)"
                            << "\n\t4.  A thing that babies wear (DIAPERS)\n\t5.  Less narrow (WIDER)\n\t6.  Something to be careful of (DANGER)"
                            << "\n\t7.  An angry stare (GLARE)\n\t8.  Not awake (ASLEEP)\n\t9.  A marine mammal (SEAL)\n\t10. A dragon's home (LAIR)"
                            << endl;
                        break;
                    case 4:
                        cout << "\t1.  Where land meets sea (COAST)\n\t2.  A part of your arm (ELBOW)\n\t3.  Try to hear (LISTEN)"
                            << "\n\t4.  Past tense of spell (SPELT)\n\t5.  Fix a written work (EDIT)\n\t6.  Another word for snake (SERPENT)"
                            << "\n\t7.  A piece of beef (STEAK)\n\t8.  A period of time (ERA)\n\t9.  Not as hard (SOFTER)\n\t10. Characteristics of something (TRAITS)"
                            << endl;
                        break;
                    case 5:
                        cout << "\t1. Be a thief (STEAL)\n\t2. Make bigger (ENLARGE)\n\t3. A spot on your shirt (STAIN)"
                            << "\n\t4. An organ that pumps blood (HEART)\n\t5. A square or a triangle (SHAPE)\n\t6. Another word for warn (ALERT)"
                            << "\n\t7. Pieces of furniture to serve food on (TABLES)\n\t8. A seasoning (SALT)"
                            << "\n\t9. A precious metal (SILVER)\n\t10. Have belongings (OWN)" << endl;
                        break;
                    }

                    cout << "Enter the index of the clue you wish to solve: "; // Prompt for clue selection

                    // Loop for valid clue selection
                    while (true) {
                        cin >> clue; // Read user input
                        if (clue >= 1 && clue <= 10) break; // Break if input is valid
                        cout << "Invalid index. Please enter a number between 1 and 10: "; // Prompt for valid input
                    }

                    c[ch - 1].check(clue); // Check the clue using the corresponding Crossword object
                    c[ch - 1].display(); // Display current answers

                    // Prompt for user action after solving a clue
                    while (true) {
                        cout << "\n1. Solve another clue\n0. Save and exit" << endl; // Prompt for next action
                        cin >> ch2; // Read user input

                        if (ch2 == 1 || ch2 == 0) break; // Break if input is valid

                        cout << "Invalid input. Please enter 1 to solve another clue or 0 to save and exit." << endl; // Prompt for valid input
                    }

                } while (ch2 != 0); // Continue until the user chooses to exit

                auto total_end = chrono::high_resolution_clock::now(); // End total timer
                chrono::duration<double> total_duration = total_end - total_start; // Calculate total duration
                cout << "\nTotal time taken to complete the crossword: " << total_duration.count() << " seconds" << endl; // Display total time taken
            }

        } while (ch != 0); // Continue until the user chooses to exit
    }
};


int main() {
    Multiple m;            // Create an instance of the Multiple class
    m.clues();             // Start the clue-solving process
    return 0;
}