#include <fstream>
#include <iostream>
#include <string>

void DisplayExit(void);
void DisplayHeader(void);
void FileCleaner(void);
void ReadLineAndClean(std::ifstream& dataStream, std::string& dataString);
bool ValidFile(const std::string& str);
bool YnQ(const std::string& out);


int main() {
	DisplayHeader();

	do {
		FileCleaner();
	} while (YnQ("Would you like to clean another file?"));

	DisplayExit();
	return 0;
}

//Functions

void DisplayExit(void) {
	std::cout << "\nFinished! \nPress Any Key to Close.\n";
	std::cin.get();
}

void DisplayHeader(void) {
	std::cout << "FileReader V1.0 \n\n";
}

void FileCleaner(void) {
	//Check FileName
	std::string fileName;
	do {
		fileName.clear();
		std::cout << "Enter Filename (including .txt filetype): ";
		std::cin.clear();
		std::getline(std::cin, fileName);
	} while (!ValidFile(fileName));
	std::ifstream in(fileName);

	//Determine Output FileName
	std::string cloneName;
	unsigned fileNum = 0;
	do {
		cloneName = fileName;
		cloneName.insert(cloneName.size() - 4, "Clean");
		if (fileNum > 0) {
			cloneName.insert(cloneName.size() - 4, std::to_string(fileNum));
		}
		fileNum++;
	} while (ValidFile(cloneName));

	std::cout << cloneName << "\n\n";

	//Copy and Clean
	std::string line;
	std::ofstream out(cloneName, std::ofstream::app);
	do {
		line.clear();
		ReadLineAndClean(in, line);
		out << line << "\n";
	} while (!line.empty());
	out.close();
}

void ReadLineAndClean(std::ifstream& dataStream, std::string& dataString)
{
	do {
		std::getline(dataStream, dataString);

		//SemiColon as Comma < May make a setting >
		while (dataString.find(";") != std::string::npos) {
			dataString.replace(dataString.find(";"), 1, ",");
		}
		//Remove White Space, Empty New Lines, Leading and Double Commas.
		while (dataString.find(" ") != std::string::npos) {
			dataString.erase(dataString.find(" "), 1);
		}
		while (dataString.find("\t") != std::string::npos) {
			dataString.erase(dataString.find("\t"), 1);
		}
		while (dataString.find(",,") != std::string::npos) {
			dataString.erase(dataString.find(",,"), 1);
		}
		while (dataString.find(",") == 0) {
			dataString.erase(dataString.find(","), 1);
		}
		if(dataString.find_last_of(",") == dataString.size() - 1 && !dataString.empty()) {
			dataString.pop_back();
		}
	} while (!dataStream.eof() && dataString.empty());
}

bool ValidFile(const std::string& str) {
	std::ifstream in(str);
	if (in) {
		return true;
	}
	else {
		return false;
	}
}

bool YnQ(const std::string& out)
{
	std::string answer;
	int errorCount = 0;
	while (errorCount < 3) {

		std::cout << out << "\n";
		std::cin.clear();
		std::getline(std::cin, answer);
		std::cout << "\n";
		if (answer == "y" || answer == "Y" || answer == "yes" || answer == "Yes") {
			return true;
		}
		else if (answer == "n" || answer == "N" || answer == "No" || answer == "no") {
			return false;
		}
		else {
			errorCount++;
			if (errorCount < 3) {
				std::cout << "I'm sorry, I didn't understand. Allow me to reset the question. Please Re-enter. \n";
			}
		}
	}
	std::cout << "I'm sorry, I didn't understand. Assuming No.\n \n";
	return false;
}