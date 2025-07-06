#ifndef DATA_H
#define DATA_H

#include <string>

class Data {
public:
    int timestamp;

    Data(int ts = 0);
    static Data fromString(const std::string& str);
    bool operator<(const Data& outra) const;
    std::string toString() const;
};

#endif // DATA_H