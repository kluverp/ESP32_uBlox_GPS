#include "../include/aos_nmea_gpgll.h"

void NmeaGpgll::parseLat() {
    char deg[] = {parts_[PART_LATITUDE][0], parts_[PART_LATITUDE][1], '\0'};
    latDeg_ = strtol(deg, NULL, 10);

    char min[] = {parts_[PART_LATITUDE][2], parts_[PART_LATITUDE][3], '\0'};
    latMin_ = strtol(min, NULL, 10);

    latMinFrac_ = strtof(&parts_[PART_LATITUDE][2], NULL);

    latSec_ = strtof(&parts_[PART_LATITUDE][4], NULL) * 60;

    latHemisphere_ = parts_[PART_LAT_HEMI][0];
}

void NmeaGpgll::parseLng() {
    char deg[] = {parts_[PART_LONGITUDE][0], parts_[PART_LONGITUDE][1], parts_[PART_LONGITUDE][2], '\0'};
    lngDeg_ = strtol(deg, NULL, 10);

    char min[] = {parts_[PART_LONGITUDE][3], parts_[PART_LONGITUDE][4], '\0'};
    lngMin_ = strtol(min, NULL, 10);

    lngMinFrac_ = strtof(&parts_[PART_LONGITUDE][3], NULL);

    lngSec_ = strtof(&parts_[PART_LONGITUDE][5], NULL) * 60;

    lngHemisphere_ = parts_[PART_LNG_HEMI][0];
}

NmeaGpgll::NmeaGpgll(std::string data) {
    data_ = data;

    // create a copy of the data, so we can modify the string
    // with strtok().
    char copy[data.length() + 1] = {'\0'};
    strcpy(copy, data.c_str());

    // extract the first token
    char *token = strtok(copy, ",");

    // loop through the string to extract all other tokens
    int i = 0;
    while (token != NULL) {
        strncpy(parts_[i], token, sizeof(parts_[i]));
        token = strtok(NULL, ",");
        i++;
    }

    // parse the data
    parseLat();
    parseLng();
}

bool NmeaGpgll::isValid() {
    return parts_[PART_VALID][0] == 'A';
}

int NmeaGpgll::getLatDeg() {
    return latDeg_;
}

int NmeaGpgll::getLatMin() {
    return latMin_;
}

float NmeaGpgll::getLatMinFrac() {
    return latMinFrac_;
}

float NmeaGpgll::getLatSec() {
    return latSec_;
}

char NmeaGpgll::getLatHemisphere() {
    return latHemisphere_;
}

std::string NmeaGpgll::getLatDMS() {
    char lat[20] = {'\0'};
    snprintf(lat, sizeof(lat), "%d°%d'%.1f\"%c", getLatDeg(), getLatMin(), getLatSec(), getLatHemisphere());
    return std::string(lat);
}

std::string NmeaGpgll::getLatDMM() {
    char lat[20] = {'\0'};
    snprintf(lat, sizeof(lat), "%d %f", getLatDeg(), getLatMinFrac());
    return std::string(lat);
}

float NmeaGpgll::getLatDD() {
    return getLatDeg() + (getLatMinFrac() / 60);
}

int NmeaGpgll::getLngDeg() {
    return lngDeg_;
}

int NmeaGpgll::getLngMin() {
    return lngMin_;
}

float NmeaGpgll::getLngMinFrac() {
    return lngMinFrac_;
}

float NmeaGpgll::getLngSec() {
    return lngSec_;
}

char NmeaGpgll::getLngHemisphere() {
    return lngHemisphere_;
}

std::string NmeaGpgll::getLngDMS() {
    char lng[20] = {'\0'};
    snprintf(lng, sizeof(lng), "%d°%d'%.1f\"%c", getLngDeg(), getLngMin(), getLngSec(), getLngHemisphere());
    return std::string(lng);
}

std::string NmeaGpgll::getLngDMM() {
    char lng[20] = {'\0'};
    snprintf(lng, sizeof(lng), "%d %f", getLngDeg(), getLngMinFrac());
    return std::string(lng);
}

float NmeaGpgll::getLngDD() {
    return getLngDeg() + (getLngMinFrac() / 60);
}
