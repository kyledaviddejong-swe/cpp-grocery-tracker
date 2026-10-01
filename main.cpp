/* 
Dev: Kyle DeJong
Date: 12/12/2025
Purpose: Grocery item frequency tracker
*/

#include <iostream>
#include <fstream> // File i/o
#include <string> 
#include <map> // Storing item string/frequency
#include <sstream> // Validate complete input lines

using namespace std; 

// Loads a .txt file of groceries for counting the frequency of occurence with item lookup, full output, and histogram output
class GroceryTracker {
public:
	GroceryTracker(); 
	bool loadFile(const string& fileName = "groceries.txt"); // Loads the file and creates a map of word frequency
	void printToStream(ostream& out = cout) const; // Output map to console in full
	void printHistogram(ostream& out = cout) const; // Output map to console in full with asterisks instead of a number
	void itemFind(const string& item) const; // Look up a specific item frequency

private:
	bool createBackup() const; // Creates a .dat file containing the map 
	map<string, int> itemFrequency; // Ordered map for storing item names/frequency
};

// Empty constructor
GroceryTracker::GroceryTracker() { } 

bool GroceryTracker::loadFile(const string &fileName) {
	string item;
	ifstream fileInput(fileName);

	if (!fileInput.is_open()) { // Check if the ifstream found a valid file
		cout << "Error: Failed to open file." << endl;
		return false; // Report failure to main() 
	}

	while (fileInput >> item) { // Loop until EOF
		itemFrequency[item]++; // Iterate the map by key 
	}

	if (!fileInput.eof()) {
		cout << "Error: Failed while reading input file." << endl;
		return false;
	}

	fileInput.close();

	if (!createBackup()) { // Attempt to create a .dat backup of the map
		cout << "Error: Failed to create backup file \"frequency.dat\"." << endl;
		return false; // Report failure to main()
	}

	return true;
}

void GroceryTracker::printToStream(ostream& out) const {
	for (const auto& pair : itemFrequency) { // Iterate through every element pair in the map
		out << pair.first << " " << pair.second << endl; 
	}
}

void GroceryTracker::printHistogram(ostream& out) const {
	for (const auto& pair : itemFrequency) {
		out << pair.first << " ";
		for (int i = 0; i < pair.second; i++) { // Instead of the frequency number, output equivalent amount of asterisks
			out << "*";
		}
		out << endl;
	}
}

bool GroceryTracker::createBackup() const {
	ofstream fileOutput("frequency.dat"); // Create backup file in system
	if (!fileOutput.is_open()) { // Check if the file was created
		cout << "Error: Failed to create backup file." << endl;
		return false; // Report failure to main()
	}
	printToStream(fileOutput); // Output the map to the created file via the ofstream
	fileOutput.close();
	return !fileOutput.fail(); // Includes write, flush and close failures
}

void GroceryTracker::itemFind(const string& item) const {
	auto temp = itemFrequency.find(item); // Check the map by key value

	if (temp != itemFrequency.end()) { // If the key exists in the map
		cout << temp->first << " " << temp->second; // Output item name and frequency
	}
	else {
		cout << item << " 0" << endl; // If not in the map, output item name and 0 
	}
}

static void printMenu() {
	cout << "\nCorner Grocer Menu" << endl;
	cout << "__________________" << endl;
	cout << "1. Item frequency search" << endl;
	cout << "2. All item frequency display" << endl;
	cout << "3. All item frequency histogram" << endl;
	cout << "4. Exit" << endl;
	cout << "Please enter a choice: ";
}

int main() {
	GroceryTracker gt;
	
	if (!gt.loadFile()) { // Initialize the GroceryTracker, automatically creates a backup file, abort on failure
		cout << "Error: Failed to properly initialize program, exiting." << endl; 
		return 1; 
	}	
	
	int userChoice;
	while (true) {		
		printMenu();

		string inputLine;
		if (!getline(cin, inputLine)) {
			cout << "\nInput closed. Exiting the program." << endl;
			return cin.eof() ? 0 : 1;
		}
		istringstream menuInput(inputLine);
		char extra;
		if (!(menuInput >> userChoice) || (menuInput >> extra) || userChoice < 1 || userChoice > 4) {
			cout << "Error: Invalid option. Please only enter a valid number." << endl;
			continue;
		}

		string userItem; 
		/* 1. Item lookup
		* 2. Full item output
		* 3. Full histogram output
		* 4. Exit */
		switch (userChoice) {			
		case 1:
			cout << "\nPlease enter an item or word to look up: ";
			if (!getline(cin, inputLine)) {
				cout << "\nInput closed. Exiting the program." << endl;
				return cin.eof() ? 0 : 1;
			}
			{
				istringstream itemInput(inputLine);
				if (!(itemInput >> userItem) || (itemInput >> extra)) {
					cout << "Error: Enter one item name." << endl;
				} else {
					gt.itemFind(userItem);
				}
			}
			cout << endl;
			break;
		case 2:
			cout << endl;
			gt.printToStream(); // Full map output
			cout << endl; 
			break;
		case 3:
			cout << endl;
			gt.printHistogram(); // Full histogram output
			cout << endl;
			break;
		case 4:
			cout << "\nExiting the program." << endl;
			return 0; 
		default: // Shouldn't ever be hit, but included for formality
			cout << "Error: Invalid menu option has succeeded." << endl;
			break;
		}

	}

	return 0; 
}