// TimeUtils.hpp
#ifndef TIMEUTILS_HPP
#define TIMEUTILS_HPP

#include <ctime>
#include <string>
#include <iostream>

class TimeUtils {
public:
    // 安全的时间转换函数
    static struct tm* safeLocaltime(const std::time_t* time, struct tm* result) {
        if (!time || !result) return nullptr;

#ifdef _WIN32
        // Windows平台使用localtime_s
        localtime_s(result, time);
#else
        // Linux/Unix平台使用localtime_r
        localtime_r(time, result);
#endif
        return result;
    }

    // 时间转换为字符串
    static std::string timeToString(std::time_t time, const char* format = "%Y-%m-%d %H:%M:%S") {
        if (time == 0) return "未设置";

        char buffer[80];
        struct tm timeinfo;

        if (safeLocaltime(&time, &timeinfo)) {
            std::strftime(buffer, sizeof(buffer), format, &timeinfo);
            return std::string(buffer);
        }
        return "时间转换失败";
    }

    // 获取当前时间
    static std::time_t getCurrentTime() {
        return std::time(nullptr);
    }

    // 创建指定日期时间
    static std::time_t createDateTime(int year, int month, int day,
        int hour = 0, int minute = 0, int second = 0) {
        struct tm timeinfo = {};
        timeinfo.tm_year = year - 1900;
        timeinfo.tm_mon = month - 1;
        timeinfo.tm_mday = day;
        timeinfo.tm_hour = hour;
        timeinfo.tm_min = minute;
        timeinfo.tm_sec = second;
        timeinfo.tm_isdst = -1; // 自动判断夏令时

        return mktime(&timeinfo);
    }
};

#endif // TIMEUTILS_HPP