#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Settings.h"

void DisplayExit(void);
void DisplayHeader(void);
void DisplayManual(void);
void FileCleaner(void);
void FlagHandler(std::string& str, Settings& inSet);
void InputHandler(std::string& inStr, Settings& inSet);
void ReadLineAndClean(std::ifstream& dataStream, std::string& dataString, const Settings& inSet);
bool ValidFile(const std::string& str);


int main() {
	DisplayHeader();

	while(true){
		FileCleaner();
		std::cout << "Would you like to clean another file?";
	}

	DisplayExit();
	return 0;
}

//Functions

void DisplayExit(void) {
	std::cout << "\nFinished! \nPress Any Key to Close.\n";
	std::cin.get();
}

void DisplayHeader(void) {
	std::cout << "FileReader V1.1-alpha \n";
	std::cout << "Author: MariusVentus \n\n";
	DisplayManual();

}

void DisplayManual(void)
{
	std::cout << "-----Manual-----\n";
	std::cout << "Add flags after the filename to alter how it cleans. \n";
	std::cout << "[/to] Change Filetype. [FileName /to filetype] \n";
	std::cout << "[/sc] Don't convert semi-colons to commas. [FileName /sc] \n";
	std::cout << "[/ws] Don't remove whitespace. [FileName /ws] \n";
	std::cout << "[/nl] Don't remove empty lines. [FileName /nl] \n";
	std::cout << "[/dc] Don't remove double commas. [FileName /dc] \n";
	std::cout << "[/lc] Don't remove leading commas. [FileName /lc] \n";
	std::cout << "[/tc] Don't remove trailing commas. [FileName /tc] \n";
	std::cout << "[/cw] Convert commas to white space. [FileName /cw] \n";
	std::cout << "[/dup] Duplicate lines X number of times. [FileName /dup X] \n";
	std::cout << "[/man] Display Manual [/man] \n";

}

void FileCleaner(void) {
	Settings set;
	//Check FileName
	std::string fileName;
	do {
		fileName.clear();
		std::cout << "\nEnter Filename (including filetype): ";
		std::cin.clear();
		std::getline(std::cin, fileName);
		if (fileName.find("man") != std::string::npos) {
			DisplayManual();
		}
		else {
			InputHandler(fileName, set);
		}
	} while (!ValidFile(fileName));
	std::ifstream in(fileName);

	//Determine Output FileName
	std::string cloneName;
	unsigned fileNum = 0;
	do {
		cloneName = fileName;
		cloneName.insert(cloneName.find_last_of("."), "Clean");
		if (fileNum > 0) {
			cloneName.insert(cloneName.find_last_of("."), std::to_string(fileNum));
		}
		fileNum++;
	} while (ValidFile(cloneName));
	if (set.changeFT == true) {
		cloneName.erase(cloneName.find_last_of(".") + 1);
		cloneName.insert(cloneName.find_last_of(".") + 1, set.newFT);
	}
	std::cout << cloneName << "\n\n";

	//Copy and Clean
	bool firstRun = true;
	bool firstRunEcho = true;
	std::string line;
	std::ofstream out(cloneName, std::ofstream::app);
	line.clear();
	do {
		if (!line.empty()) {
			if (set.keepNL == true) {
				if (firstRunEcho == false) {
					if (set.duplicateLines == true) {
						for (unsigned i = 0; i <= set.dupCount; i++) {
							out << line;
							if (i != set.dupCount) {
								out << "\n";
							}
						}
					}
					else {
						out << line;
					}
				}
				else if (firstRunEcho == true && line.find("\n") != 0) {
					if (set.duplicateLines == true) {
						for (unsigned i = 0; i <= set.dupCount; i++) {
							out << line;
							if (i != set.dupCount) {
								out << "\n";
							}
						}
					}
					else {
						out << line;
					}
					firstRunEcho = false;
				}
				else {
					firstRunEcho = false;
				}
			}
			else {
				if (set.duplicateLines == true) {
					for (unsigned i = 0; i <= set.dupCount; i++) {
						out << line;
						if (i != set.dupCount) {
							out << "\n";
						}
					}
				}
				else {
					out << line;
				}
			}
		}
		line.clear();
		ReadLineAndClean(in, line, set);
		if (set.keepNL == false && !line.empty() && firstRun == false) {
			out << "\n";
		}
		else if (set.keepNL == true && line.find("\n") == std::string::npos && firstRun == false) {
			out << "\n";
		}
		if (firstRun) {
			firstRun = false;
		}
	} while (!in.eof() && !line.empty());

	if (!line.empty()) {
		if (set.duplicateLines == true) {
			for (unsigned i = 0; i <= set.dupCount; i++) {
				out << line;
				if (i != set.dupCount) {
					out << "\n";
				}
			}
		}
		else {
			out << line;
		}
	}
	out.close();
}

void FlagHandler(std::string& str, Settings& inSet)
{
	//Remove Double Whitespace.
	while (str.find("  ") != std::string::npos) {
		str.erase(str.find("  "), 1);
	}

	//Flags
	//Change FileType
	if (str.find("to") != std::string::npos && str.find(".") != std::string::npos) {
		std::stringstream toType(str);
		std::string type;
		std::getline(toType, type, '.');
		std::getline(toType, type, ' ');
		inSet.changeFT = true;
		inSet.newFT = type;
	}
	else if (str.find("to") != std::string::npos && str.find(".") == std::string::npos) {
		std::cout << "New file type not recognized.\n";
	}
	//Don't treat Semi-Colons as Commas
	else if (str.find("sc") != std::string::npos) {
		inSet.semiCasComma = false;
	}
	//Keep WhiteSpace
	else if (str.find("ws") != std::string::npos) {
		inSet.keepWS = true;
	}
	//Keep Extra New Lines 
	else if (str.find("nl") != std::string::npos) {
		inSet.keepNL = true;
	}
	//Keep Double Commas
	else if (str.find("dc") != std::string::npos) {
		inSet.keepDC = true;
	}
	//Keep Leading Commas
	else if (str.find("lc") != std::string::npos) {
		inSet.keepLC = true;
	}
	//Keep Trailing Commas
	else if (str.find("tc") != std::string::npos) {
		inSet.keepTC = true;
	}
	//Convert Commas to White Space
	else if (str.find("cw") != std::string::npos) {
		inSet.commaToWS = true;
	}
	//Duplicate Lines X times.
	else if (str.find("dup") != std::string::npos) {
		std::stringstream toCount(str);
		std::string count;
		inSet.duplicateLines = true;
		std::getline(toCount, count, ' ');
		std::getline(toCount, count, ' ');
		inSet.dupCount = std::stoi(count);
	}
}

void InputHandler(std::string& inStr, Settings& inSet)
{
	//Remove Leading Whitespace.
	while (inStr.find(" ") == 0) {
		inStr.erase(inStr.find(" "), 1);
	}
	//Check Flags, Protect intiial Flag.
	if (inStr.find("/") != std::string::npos) {
		inStr.insert(inStr.find("/"), "/");
		std::stringstream inputStream(inStr);
		std::string flags;
		std::getline(inputStream, inStr, '/');
		do {
			std::getline(inputStream, flags, '/');
			FlagHandler(flags, inSet);
		} while (!inputStream.eof());
	}

	//Remove trailing Whitespace from FileName.
	while (inStr.find_last_of(" ") == inStr.size() - 1 && !inStr.empty()) {
		inStr.pop_back();
	}
	//BadFile
	if (!ValidFile(inStr)) {
		std::cout << "Source file name not recognized. \n";
	}
}

void ReadLineAndClean(std::ifstream& dataStream, std::string& dataString, const Settings& inSet)
{
	do {
		std::getline(dataStream, dataString);

		//SemiColon as Comma
		if (inSet.semiCasComma == true) {
			while (dataString.find(";") != std::string::npos) {
				dataString.replace(dataString.find(";"), 1, ",");
			}
		}
		//Remove White Space
		if (inSet.keepWS == false) {
			while (dataString.find(" ") != std::string::npos) {
				dataString.erase(dataString.find(" "), 1);
			}
			//Tabs included in that. 
			while (dataString.find("\t") != std::string::npos) {
				dataString.erase(dataString.find("\t"), 1);
			}
		}
		//Remove Double Comma
		if (inSet.keepDC == false) {
			while (dataString.find(",,") != std::string::npos) {
				dataString.erase(dataString.find(",,"), 1);
			}
		}
		//Remove Leading Comma
		if (inSet.keepLC == false) {
			while (dataString.find(",") == 0) {
				dataString.erase(dataString.find(","), 1);
			}
		}
		//Remove Trailing Comma
		if (inSet.keepTC == false) {
			while (dataString.find_last_of(",") == dataString.size() - 1 && !dataString.empty()) {
				dataString.pop_back();
			}
		}
		//Keep Extra Newlines
		if (inSet.keepNL == true) {
			if (!dataStream.eof() && dataString.empty()) {
				dataString = "\n";
			}
		}
		//Comma to White Space
		if (inSet.commaToWS == true) {
			while (dataString.find(",") != std::string::npos) {
				dataString.replace(dataString.find(","), 1, " ");
			}
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

