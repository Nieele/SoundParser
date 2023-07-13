const int MIN_SERIAL_NUM_DEVICE = 0;
const int MAX_SERIAL_NUM_DEVICE = 5;

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <xlnt/xlnt.hpp>
#include <memory>

void createNamedTable(xlnt::worksheet ws, size_t countSheets) {
	ws.title(std::to_string(countSheets));
	ws.cell("A1").value("42,4");
	ws.cell("B1").value("63");
	ws.cell("C1").value("125");
	ws.cell("D1").value("500");
	ws.cell("E1").value("1000");
	ws.cell("F1").value("1588");
	ws.cell("G1").value("1990");
	ws.cell("H1").value("2482");
	ws.cell("I1").value("3040");
	ws.cell("J1").value("3447");
	ws.cell("K1").value("3747");
	ws.cell("L1").value("4047,2");
	ws.cell("M1").value("4395,4");
	ws.cell("N1").value("4747,6");
	ws.cell("O1").value("5047,7");
	ws.cell("P1").value("5396,2");
	ws.cell("Q1").value("5748");
	ws.cell("R1").value("6048");
	ws.cell("S1").value("6348");
	ws.cell("T1").value("6600");
}

void parse(std::ifstream& inFile, int serialNum) 
{
	int hours_begin, minutes_begin;
	int hours_end, minutes_end;

	char answer = 0;
	size_t countSheets = 1;
	
	nlohmann::json jsonData = nlohmann::json::parse(inFile);
	xlnt::workbook wb;
	std::string serialNum_str = "0" + std::to_string(serialNum);

	do {
		std::cout << "Enter a time range\n begin (h, m): ";
		std::cin >> hours_begin >> minutes_begin;
		std::cout << "end (h, m): ";
		std::cin >> hours_end >> minutes_end;

		auto ws = wb.create_sheet();
		//createNamedTable(ws, countSheets);

		int column = 1, row = 2;

		for (auto& el : jsonData) {
			if (el.find("uName").value() == "Shok-MN" && el.find("serial").value() == serialNum_str) {

				ws.cell(column + 20, row).value(std::string(el.find("Date").value()));

				std::string valueData;
				auto data = el.find("data").value();
				for (size_t i = 0; i < 20; ++i) {
					valueData = data.find("system_" + std::to_string(i)).value();
					ws.cell(column + i, row).number_format(xlnt::number_format::number_comma_separated1());
					ws.cell(column + i, row).value(std::stod(valueData));
				}
				++row;
			}
		}

		do {
			std::cout << "choose the next interval? [y/n]: ";
			std::cin >> answer;
		} while (answer != 'y' && answer != 'n');

		++countSheets;
	} while (answer != 'n');

	wb.save("example.xlsx");
}

bool checkCountArguments(int argc)
{
	if (argc != 3)
	{
		std::cout << "Too few arguments" << std::endl;
		return false;
	}
	return true;
}

bool checkFileIsOpen(std::ifstream& inFile, const char filename[])
{
	if (!inFile.is_open())
	{
		std::cout << "Can't find the file (" << filename << ")" << std::endl;
		return false;
	}
	return true;
}

int getSerialNumberDevice() {
	std::cout << "Enter the serial number of the device: ";
	int serialNum;
	std::cin >> serialNum;

	if (serialNum < MIN_SERIAL_NUM_DEVICE || serialNum > MAX_SERIAL_NUM_DEVICE) 
	{
		std::cout << "Incorrect serial number device" << std::endl;
		return -1;
	}
	return serialNum;
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