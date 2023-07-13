#define NOMINMAX 
#include <iostream>
#include <locale>
#include <limits>
#include <sstream>
#include "LoaderJsonData.hpp"

int main() 
{
	std::setlocale(LC_ALL, "");

	char answ;
	do {
		std::cout << "1) Download data in one day" << std::endl;
		std::cout << "2) Download data for a period of time" << std::endl;
		std::cin >> answ;

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} while (answ != '1' && answ != '2');

	if (answ == '1')
	{
		std::cout << "Enter date [DD MM YYYY]" << std::endl;
		std::string timeString;
		std::istringstream iss;
		std::tm timeStruct;
		do {
			std::getline(std::cin, timeString);
			iss.str(timeString);
			iss >> std::get_time(&timeStruct, "%d %m %y");

			if (iss.fail())
			{
				std::cout << "Incorrect time format, try again" << std::endl;
				iss.clear();
			}
		} while (iss.fail());

		LoaderJsonData::load(timeStruct);
	}

// int argc, char* argv[]
int main() 
{
	int argc = 3;
	const char* argv[] = { "SoundParser", "log.json", "table.xls"};
	if (!checkCountArguments(argc))
		return 1;

	/*std::shared_ptr<std::ifstream> inFile = std::make_shared<std::ifstream>(std::ifstream(argv[1]), [](std::ifstream& file) {
		file.close();
		});*/
	std::ifstream inFile(argv[1]);

	if (!checkFileIsOpen(inFile, argv[1])) 
		return 2;

	int serialNum = getSerialNumberDevice();
	if (serialNum == -1)
		return 3;

	parse(inFile, serialNum);

	inFile.close();

	return 0;
}