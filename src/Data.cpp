#include "Data.h"
#include <iomanip>
#include <sstream>

Data::Data(int ts) : timestamp(ts) {}

Data Data::fromString(const std::string& str) {
    return Data(std::stoi(str));
}

bool Data::operator<(const Data& outra) const {
    return this->timestamp < outra.timestamp;
}

std::string Data::toString() const {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(7) << timestamp;
    return oss.str();
}

