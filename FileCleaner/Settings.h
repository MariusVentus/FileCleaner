#pragma once
#include <string>

struct Settings
{
public:
//Vars
	// Treat Semi-Colons as Commas
	bool semiCasComma = true;
	//Keep Whitespace
	bool keepWS = false;
	//Keep Empty New Lines
	bool keepNL = false;
	//Keep Double Commas
	bool keepDC = false;
	//Keep Leading Commas
	bool keepLC = false;
	//Keep Trailing Commas
	bool keepTC = false;
	//Change Filetype
	bool changeFT = false;
	std::string newFT;
	//Convert Commas to White Space. 
	bool commaToWS = false;
	//Duplicate lines X times.  
	bool duplicateLines = false;
	unsigned dupCount = 0;
private:

};