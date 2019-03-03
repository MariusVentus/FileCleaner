#include <fstream>
#include <iostream>
#include <string>

void ReadLineAndClean(std::ifstream& dataStream, std::string& dataString)
{
	do {
		std::getline(dataStream, dataString);

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

int main() {
	//Check FileName
	std::string fileName;
	do {
		fileName.clear();
		std::cout << "Enter Filename (including .txt filetype): ";
		std::getline(std::cin, fileName);
	} while (!ValidFile(fileName));
	std::ifstream in(fileName);
	
	//Determine Output FileName
	std::string cloneName;
	unsigned fileNum = 0;
	do {
		cloneName = fileName;
		cloneName.insert(cloneName.size() - 4, "Clone");
		if (fileNum > 0) {
			cloneName.insert(cloneName.size() - 4, std::to_string(fileNum));
		}
		fileNum++;
	} while (ValidFile(cloneName));
	
	std::cout << cloneName;
	
	//Copy and Clean
	std::string line; 
	do {
		line.clear();
		ReadLineAndClean(in, line);
		
	} while (!line.empty());



	std::cin.get();
	return 0;
}