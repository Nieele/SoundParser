#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <cpr/cpr.h>
#include <utility>
#include <stdexcept>
#include <filesystem>

void downloadFile(const std::chrono::local_seconds& date) {
    std::string requestLink =
        std::format("http://dbrobo.mgul.ac.ru/core/deb.php?fdate={:%Y-%m-%d}+09%3A00%3A00&sdate={:%Y-%m-%d}+16%3A45%3A00&fileback=1", date, date);
    cpr::Response dataDownload = cpr::Get(cpr::Url{requestLink});
    cpr::Response data = cpr::Get(cpr::Url{"http://dbrobo.mgul.ac.ru/export/log.txt"});
    std::ofstream file(std::format("data\\{:%Y.%m.%d}.json", date), std::ios::binary);
    file.write(data.text.c_str(), data.text.length());
    file.close();
}

std::chrono::local_seconds parseTime(const std::string& timeString)
{
    std::chrono::local_seconds timeStamp;
    std::stringstream iss(timeString);
    if (not (iss >> std::chrono::parse("%Y %m %d", timeStamp)))
        throw std::invalid_argument("incorrect time");
    return timeStamp;
}

std::pair<std::chrono::local_seconds, std::chrono::local_seconds> parseInterval(const std::string& startTime, const std::string& endTime)
{
    return std::pair<std::chrono::local_seconds, std::chrono::local_seconds>(parseTime(startTime), parseTime(endTime));
}

int main() {
    setlocale(LC_ALL, "RU");

    std::string startTimeString, endTimeString;
    std::cout << "Enter time format [YYYY MM DD]" << std::endl;
    std::getline(std::cin, startTimeString);
    std::cout << "Enter time format [YYYY MM DD]" << std::endl;
    std::getline(std::cin, endTimeString);
    auto interval = parseInterval(startTimeString, endTimeString);

    system("cls");
    std::cout << std::format("Start time: {:%Y-%m-%d %T}", interval.first) << std::endl;
    std::cout << std::format("End time {:%Y-%m-%d %T}", interval.second) << std::endl;

    if (not std::filesystem::is_directory("data"))
        std::filesystem::create_directory("data");

    interval.second += std::chrono::days(1);
    auto currentDay = interval.first;

    do {
        // file exists
        if (std::filesystem::exists(std::format("data\\{:%Y.%m.%d}.json", currentDay)))
        {
            std::cout << std::format("file data\\{:%Y.%m.%d}.json already exists. skip ...", currentDay) << std::endl;
            continue;
        }

        std::cout << std::format("Donwload file data\\{:%Y.%m.%d}.json ...", currentDay) << std::endl;
        downloadFile(currentDay);
        currentDay += std::chrono::days(1);
    } while (currentDay < interval.second);

    return 0;
}