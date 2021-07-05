#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include <string>
#define SUCC 0
#define FAIL -1
class CommonUtil
{
public:
    static int InsertByLineNumber(const std::string &filePath, const std::string &insertContent, int insertLine);
};


#endif
