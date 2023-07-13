#include "LoaderJsonData.hpp"

static LoaderJsonData::Mode mode = LoaderJsonData::Mode::Default;

void LoaderJsonData::load(const std::tm& date) noexcept
{
	std::string filename = generateFilename(date);
	if (mode == Mode::Default || mode == Mode::Allskip)
	{
		if (isFileExist(filename))
		{
			if (mode == Mode::Allskip)
				return;

			char answ;
			do {
				std::cout << "Warning! The file already exists" << std::endl;
				std::cout << "1) download" << std::endl;
				std::cout << "2) skip" << std::endl;
				std::cout << "3) skip all files" << std::endl;
				std::cout << "4) download all files" << std::endl;

				std::cin >> answ;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			} while (!(answ >= '1' && answ <= '4'));

			switch (answ)
			{
			case '2':
				return;
			case '3':
				mode = Mode::Allskip;
				return;
			case '4':
				mode = Mode::AllDonwload;
			}
		}
	}
	std::string requestLink = fmt::format
	("http://dbrobo.mgul.ac.ru/core/deb.php?fdate={:04d}-{:02d}-{:02d}+09%3A00%3A00&sdate={:04d}-{:02d}-{:02d}+16%3A45%3A00&fileback=1",
		date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_year + 1900, date.tm_mon + 1, date.tm_mday);
}

void LoaderJsonData::load(const std::tm& first, const std::tm& second) noexcept
{
	std::string filename;
}

std::string LoaderJsonData::generateFilename(const std::tm& date) noexcept
{
	std::stringstream ss;
	ss << date.tm_mday << '.' << date.tm_mon + 1 << '.' << date.tm_year + 1900 << ".json";
	return ss.str();
}

bool LoaderJsonData::isFileExist(const std::string& filename) noexcept
{
	return false;
}
