#ifndef AOS_NMEA_GPGLL_H
#define AOS_NMEA_GPGLL_H

#include <string>
#include <string.h>

#define PART_LATITUDE  0
#define PART_LAT_HEMI  1
#define PART_LONGITUDE 2
#define PART_LNG_HEMI  3
#define PART_UTC_TIME  4
#define PART_VALID     5
#define PART_MODE      6

#define PART_MAX       7
#define PART_LEN       15

/**
 * Message Structure:
 * $GPGLL,Latitude,N,Longitude,E,hhmmss.ss,Valid,Mode*cs<CR><LF>
 *
 * Example:
 * $GPGLL,4717.11364,N,00833.91565,E,092321.00,A,A*60
 *
 *
 * | Field No. | Example     | Format     | Name       | Description                                                |
 * |===========|=============|============|============|============================================================|
 * | 1         | 4717.11364  | ddmm.mmmm  | Latitude   | Latitude, Degrees + minutes, see Format description        |
 * | 2         | N           | character  | N          | N/S Indicator, hemisphere N=north or S=south               |
 * | 3         | 00833.91565 | dddmm.mmmm | Longitude  | Longitude, Degrees + minutes                               |
 * | 4         | E           | character  | E          | E/W indicator, E=east or W=west                            |
 * | 5         | 092321.00   | hhmmss.sss | hhmmss.sss | UTC Time, Current time                                     |
 * | 6         | A           | character  | Valid      | V = Data invalid or receiver warning, A = Data valid.      |
 * | 7         | A           | character  | Mode       | Positioning Mode                                           |
 *
 * Note: Only pass in Field 2 - 7 as 'data' to this class! This is the 'data' segment of the NmeaFrame!
 */

class NmeaGpgll {
public:
    std::string data_;
    char parts_[PART_MAX][PART_LEN];

    int latDeg_ = 0;
    int latMin_ = 0;
    float latMinFrac_ = 0;
    float latSec_ = 0;
    char latHemisphere_ = 'N';

    int lngDeg_ = 0;
    int lngMin_ = 0;
    float lngMinFrac_ = 0;
    float lngSec_ = 0;
    char lngHemisphere_ = 'E';

    void parseLat();

    void parseLng();

public:
    /**
     * Constructor, takes NMEA data segment as argument.
     *
     * @param data
     */
    NmeaGpgll(std::string data);

    /**
     * Returns if the coordinates acquired are valid.
     *
     * @return
     */
    bool isValid();

    /**
     * Returns latitude degrees.
     *
     * @return
     */
    int getLatDeg();

    /**
     * Returns latitude minutes as whole number.
     *
     * @return
     */
    int getLatMin();

    /**
     * Returns latitude minutes with fractional part.
     *
     * @return
     */
    float getLatMinFrac();

    /**
     * Returns latitude seconds.
     *
     * @return
     */
    float getLatSec();

    /**
     * Returns the latitude hemisphere.
     *
     * @return
     */
    char getLatHemisphere();

    /**
     * Returns latitude in DMS format.
     *
     * @return
     */
    std::string getLatDMS();

    /**
     * Returns latitude in DMM format.
     *
     * @return
     */
    std::string getLatDMM();

    /**
     * Returns latitude in DD format.
     *
     * @return
     */
    float getLatDD();

    /**
     * Returns longitude degrees.
     *
     * @return
     */
    int getLngDeg();

    /**
     * Returns longitude minutes as whole number.
     *
     * @return
     */
    int getLngMin();

    /**
     * Returns longitude minutes with fractional part.
     *
     * @return
     */
    float getLngMinFrac();

    /**
     * Returns longitude seconds.
     *
     * @return
     */
    float getLngSec();

    /**
     * Returns the longitude hemisphere.
     *
     * @return
     */
    char getLngHemisphere();

    /**
     * Returns longitude in DMS format, e.g.: ""
     *
     * @return std::string
     */
    std::string getLngDMS();

    /**
     * Returns longitude in DMM format, e.g.: "52 17.23434"
     *
     * @return std::string
     */
    std::string getLngDMM();

    /**
     * Returns longitude in DD format, e.g.: "52.044846"
     *
     * @return float
     */
    float getLngDD();
};


#endif // AOS_NMEA_GPGLL_H
