#ifndef AOS_NMEA_FRAME_H
#define AOS_NMEA_FRAME_H

#include <string>

/**
 * NMEA protocol frame:
 *
 *   1       2            3            4            5
 * -------------------------------------------------------
 * | $ | <address> | {,<value>} | *<checksum> | <CR><LF> |
 * -------------------------------------------------------
 *
 * 1 - Start character (always '$')
 * 2 - Address field (only digits and uppercase letters)
 * 3 - Data field(s) (length can vary, delimited by ',')
 * 4 - Checksum field. Start with an '*' and consists of two characters. Representing a hex number.
 *     The checksum is the exclusive OR of the chars between '$' and '*'.
 * 5 - End sequence. (always <CR><LF>)
 */

class NmeaFrame {
private:
    std::string address_;
    std::string data_;
    int checksum_ = 0;
    int lrc_ = 0;

    /**
     * Parses the frame into an <address>, <data> and <checksum> field.
     *
     * @param str
     * @param len
     */
    void parse(const char *str, size_t len);

public:
    /**
     * Constructor.
     *
     * @param str
     * @param len
     */
    NmeaFrame(const char *str, size_t len);

    /**
     * Performs checksum by comparing the frame checksum value againt the LRC.
     * The checksum is the exclusive OR of all characters between '$' and '*'
     *
     * @return
     */
    bool isValid(void);

    /**
     * Returns the address segment.
     *
     * @return
     */
    std::string getAddress(void);

    /**
     * Returns the NMEA frame data segment.
     *
     * @return
     */
    std::string getData(void);

    /**
     * Returns the NMEA frame checksum value in decimal format.
     *
     * @return
     */
    int getChecksum(void);

    /**
     * Returns the calculated Longitudinal Redundancy Check.
     *
     * @return
     */
    int getLrc(void);
};


#endif // AOS_NMEA_FRAME_H
