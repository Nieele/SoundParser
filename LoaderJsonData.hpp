#pragma once
#define NOMINMAX 
#include <iostream>
#include <cpr/cpr.h>
#include <string>
#include <ctime>
#include <sstream>
#include <filesystem>
#include <fmt/format.h>

class LoaderJsonData
{
public:
	static enum Mode {
		Default,
		AllDonwload,
		Allskip
	};

	static void load(const std::tm& date) noexcept;
	static void load(const std::tm& beginDate, const std::tm& endData) noexcept;

private:
	static std::string generateFilename(const std::tm& date) noexcept;
	static bool isFileExist(const std::string& filename) noexcept;
	static Mode mode;
};

