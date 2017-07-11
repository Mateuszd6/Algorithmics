/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Trie tree data structure. Allows user to insert a words and search them in the linear time.
             Can be used as dictionary or words database.
             Space complexity: O(n*m).
Operations:
	  * Insert - insert new element to the tree. Time complexity: O(n).
      * Find - check if given word exist in the trie. Time complexity: O(n).
Notes:
	  * This trie tree uses compression, storing string instead of single chars where it is possible.
	  * Adding very long words does not spoil the space complexity very match, 
	    because they are stored as one string unless there is a need to spli them.
===============================================================================*/

#include <iostream>
#include <string>
#include "..\Utility.cpp"

// Size of the letters array in node.
const int ALPHABET_SIZE = 25;

// First letter in the used alphabet.
const char FIRST_ALPHABET_SIGN = 'a'; 

// NOTE: Inserted words must must have their leters in this range
//       [FIRST_ALPHABET_SIGN ; FIRST_ALPHABET_SIGN + ALPHABET_SIZE)

class Trie
{
private:
    struct Node
    {
        // Letters stored in the node.
        std::string word;

        // Other nodes array.
        Node **letters;

        // Is any word end direclty in this node?
        bool endOfWord;

        Node (std::string insertedWord)
        {
            word = insertedWord;
            letters = new Node*[ALPHABET_SIZE];
            for (int i = 0; i < ALPHABET_SIZE; ++i)
                letters[i] = NULL;
            endOfWord = true;
        }
        
        ~Node ()
        {
            for (int i = 0; i < ALPHABET_SIZE; ++i)
                if (letters[i] != NULL)
                    delete letters[i];
            delete[] *letters;
        }
    };

    // Root of the tree, main node.
    Node *root;

    // Same code is used to add new item to the tree and check if given item exist.
    // bool isQuery determines what type of action is being done. 
    // NOTE: if isQuery is false, return value of the function can be ignored. 
    bool TraverseTrie (const std::string &word, bool isQuery)
    {
        int currentIndex = 0;
        Node * currentNode = root;

        while (true)
        {
            bool missmatch = false;
            int i = 0;

            // Both indexes must be in range of their words.
            while (currentIndex < word.length() && i < currentNode->word.length())
            {
                // Missmatch between word in the node and given word occured. 
                if (currentNode->word[i] != word[currentIndex])
                {
                    missmatch = true;
                    break;
                }
                ++i;
                ++currentIndex;
            }

            // If there was a missmatch before reaching end of any word...
            if (missmatch)
            {
                if (isQuery)
                {
                    // Missmatch means that the given word does not exist in the trie.
                    return false;
                }
                else
                {
                    // The rest of the word in the node (from the missmatch letter to the end)
                    // goes to the node which will be linked from the current node.
                    // All links (the letter array) is also moved from the current node to this one.
                    // The rest of the inserted word creates new node and is linked from the current node.
                    // i.e:
                    // [ABC] (inserting ABD)
                    //  
                    // is transformed into:
                    //
                    //    [AB]
                    //  |      |
                    //  v      v
                    // [C]    [D]

                    // Create a node to store the part of the text in the node.
                    Node *oldNode = new Node(currentNode->word.substr(i, currentNode->word.length()));
                    oldNode->endOfWord = currentNode->endOfWord;
                    for (int i = 0; i < ALPHABET_SIZE; ++i)
                        oldNode->letters[i] = currentNode->letters[i];
                    
                    // This node will contain the rest of the inserted text.
                    Node *newNode = new Node(word.substr(currentIndex, word.length()));
                    for (int i = 0; i  < ALPHABET_SIZE; ++i)
                        newNode->letters[i] = NULL;

                    for (int i = 0; i < ALPHABET_SIZE; ++i)
                        currentNode->letters[i] = NULL;
                    
                    // Link the nodes.
                    currentNode->letters[int(currentNode->word[i] - FIRST_ALPHABET_SIGN)] = oldNode;
                    currentNode->letters[int(word[currentIndex] - FIRST_ALPHABET_SIGN)] = newNode;
                    currentNode->word = currentNode->word.substr(0, i);
                    currentNode->endOfWord = false;
                    break;
                }
            }

            // If ends of the both words are reached at the same time...
            else if (i >= currentNode->word.length() && currentIndex >= word.length())
            {
                if (isQuery)
                {
                    // If word ends in this node boolean endOfWord is true. 
                    return currentNode->endOfWord;
                }
                else
                {
                    // Note that there is a word ending in this node.
                    currentNode->endOfWord = true;
                    break;
                }
            }

            // If end of the word in the node is reached...
            else if (i >= currentNode->word.length())
            {
                Node *nextNode = currentNode->letters[int(word[currentIndex] - FIRST_ALPHABET_SIGN)];
                if (nextNode == NULL)
                {
                    if (isQuery)
                    {
                        // Comparing stoped in the middle of the node, so there is no given word in the trie.
                        return false;
                    }
                    else
                    {
                        nextNode = new Node(word.substr(currentIndex, word.length()));
                        nextNode->endOfWord = true;

                        currentNode->letters[int(word[currentIndex] - FIRST_ALPHABET_SIGN)] = nextNode;
                        break;
                    }
                }
                else
                {
                    // Jump to next node. This works for both inserting and finding.
                    currentNode = nextNode;
                    continue;
                }
            }

            // If the end of the inserting word is reached...
            else 
            {
                if (isQuery)
                {
                    // No match because word in the node has not ended yet.
                    return false;
                }
                else
                {
                    // Do the similar operation to one done when  missmatch, but only one node will be linked. 
                    Node *oldNode = new Node(currentNode->word.substr(i, currentNode->word.length()));
                    oldNode->endOfWord = currentNode->endOfWord;
                    for (int i = 0; i < ALPHABET_SIZE; ++i)
                        oldNode->letters[i] = currentNode->letters[i];

                    for (int i = 0; i < ALPHABET_SIZE; ++i)
                        currentNode->letters[i] = NULL;
                    
                    currentNode->letters[int(currentNode->word[i] - FIRST_ALPHABET_SIGN)] = oldNode;
                    currentNode->word = currentNode->word.substr(0, i);
                    currentNode->endOfWord = true;
                    break;
                }
            }
        }

        // assert(!isQuery);
        return true;
    }

public:
    Trie ()
    {
        root = new Node("");
    }

    // Before deleting the node it deletes all its childrens.
    // (Deleting is called recursively across all nodes in the trie).
    ~Trie()
    {
        delete root;
    }

    // Insert a new word into trie.
    void Insert(const std::string &insertedWord)
    {
        bool tmp = TraverseTrie(insertedWord, false);        
    }

    // Check if given word exist in the trie.
    bool Find(const std::string &insertedWord)
    {
        return TraverseTrie(insertedWord, true);
    }
};

/*
//============== USAGE EXAMPLE ===================

// Print info if given text exist in the trie.
void PrintIfExistInTrie(Trie *trieTree, const std::string text)
{
    std::cout << text << " ";
    if (trieTree->Find(text))
        std::cout << "exist in the tire.\n";
    else 
        std::cout << "doesn't exist in the trie.\n";
} 

int main()
{
    Trie *myTrie = new Trie();

    std::cout << "Adding 'abcabcabcde' to the trie.\n";
    myTrie->Insert("abcabcabcde");
    std::cout << "Adding 'abcabcabcae' to the trie.\n";
    myTrie->Insert("abcabcabcae");
    std::cout << "Adding 'abcdee' to the trie.\n";
    myTrie->Insert("abcdee");
    std::cout << "Adding 'abcd' to the trie.\n";
    myTrie->Insert("abcd");
    std::cout << "Adding 'abcdef' to the trie.\n";
    myTrie->Insert("abcdef");
    std::cout << "\n";
    PrintIfExistInTrie(myTrie, "abcd"); 
    PrintIfExistInTrie(myTrie, "abcde"); 
    PrintIfExistInTrie(myTrie, "ab");    
    PrintIfExistInTrie(myTrie, "abc");
    PrintIfExistInTrie(myTrie, "abcdef");
    std::cout << "\n";
    std::cout << "Adding 'abcde' to the trie.\n";
    myTrie->Insert("abcde");
    PrintIfExistInTrie(myTrie, "abcde"); 
    // Output:
    //Adding 'abcabcabcde' to the trie.
    //Adding 'abcabcabcae' to the trie.
    //Adding 'abcdee' to the trie.
    //Adding 'abcd' to the trie.
    //Adding 'abcdef' to the trie.
    //
    //abcd exist in the tire.
    //abcde doesn't exist in the trie.
    //ab doesn't exist in the trie.
    //abc doesn't exist in the trie.
    //abcdef exist in the tire.
    //
    //Adding 'abcde' to the trie.
    //abcde exist in the tire.
}
*/