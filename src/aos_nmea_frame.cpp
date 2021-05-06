#include "aos_nmea_frame.h"

void NmeaFrame::parse(const char *str, size_t len) {
    bool gotAddr = false;
    int startData = 0;

    if (len > 0) {
        if (str[0] == '$') {
            for (int i = 0; i < len; i++) {

                // when we encounter the first ',', we know this is the end of the address.
                // we take the chars between start '$' and the first ','.
                if (str[i] == ',' && gotAddr == false) {
                    address_ = std::string(str + 1, i - 1);
                    gotAddr = true;
                    startData = i + 1;
                }

                // when we encouter the '*', we know this is the end of the data block.
                // this also marks the start of the checksum. The checksum is always two chars.
                if (str[i] == '*') {
                    data_ = std::string(str + startData, i - startData);

                    // parse the checksum value in base 16, since it is given in hex format.
                    char checksum[2] = {str[i + 1], str[i + 2]};
                    checksum_ = strtol(checksum, NULL, 16);
                }

                // calculate the "longitudinal redundancy check". We XOR each byte between
                // the '$' and the '*' char. This gives use the checksum.
                if (i > 0 && i < len - 4) {
                    lrc_ = lrc_ ^ str[i];
                }
            }
        }
    }
}

NmeaFrame::NmeaFrame(const char *str, size_t len) {
    parse(str, len);
}

bool NmeaFrame::isValid(void) {
    return checksum_ == lrc_;
}

std::string NmeaFrame::getAddress(void) {
    return address_;
}

std::string NmeaFrame::getData(void) {
    return data_;
}

int NmeaFrame::getChecksum(void) {
    return checksum_;
}

int NmeaFrame::getLrc(void) {
    return lrc_;
}