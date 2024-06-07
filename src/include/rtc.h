#ifndef __RTC_H
#define __RTC_H

#include <stdint.h>
#include "system.h"

/**
 * Define constants for CMOS address and data registers
 */
#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71
#define REGISTER_A 0x0A
#define REGISTER_B 0x0B

/**
 * Enum for RTC registers
 */
enum RTCRegisters {
    /**
     * Second register
     */
    SECOND_REGISTER = 0x00,
    /**
     * Minute register
     */
    MINUTE_REGISTER = 0x02,
    /**
     * Hour register
     */
    HOUR_REGISTER = 0x04,
    /**
     * Day register
     */
    DAY_REGISTER = 0x07,
    /**
     * Month register
     */
    MONTH_REGISTER = 0x08,
    /**
     * Year register
     */
    YEAR_REGISTER = 0x09
};

/**
 * Global variables to store RTC values
 */
uint8_t second;
uint8_t minute;
uint8_t hour;
uint8_t day;
uint8_t month;
uint32_t year;

/**
 * Get update in progress flag from RTC
 *
 * @return 1 if update is in progress, 0 otherwise
 */
int get_update_in_progress_flag() {
    outb(CMOS_ADDRESS, REGISTER_A);
    return (inb(CMOS_DATA) & 0x80);
}

/**
 * Read a register from the RTC
 *
 * @param reg The register to read (from enum RTCRegisters)
 * @return The value of the register
 */
uint8_t get_RTC_register(enum RTCRegisters reg) {
    outb(CMOS_ADDRESS, reg);
    return inb(CMOS_DATA);
}

/**
 * Read the RTC and store the values in global variables
 */
void read_rtc() {
    uint8_t registerB;
    uint8_t previousSecond;
    uint8_t previousMinute;
    uint8_t previousHour;
    uint8_t previousDay;
    uint8_t previousMonth;
    uint8_t previousYear;

    // Wait until update is not in progress
    while (get_update_in_progress_flag());

    // Read RTC registers
    second = get_RTC_register(SECOND_REGISTER);
    minute = get_RTC_register(MINUTE_REGISTER);
    hour = get_RTC_register(HOUR_REGISTER);
    day = get_RTC_register(DAY_REGISTER);
    month = get_RTC_register(MONTH_REGISTER);
    year = get_RTC_register(YEAR_REGISTER);

    do {
        previousSecond = second;
        previousMinute = minute;
        previousHour = hour;
        previousDay = day;
        previousMonth = month;
        previousYear = year;

        // Wait until update is not in progress
        while (get_update_in_progress_flag());

        // Read RTC registers again
        second = get_RTC_register(SECOND_REGISTER);
        minute = get_RTC_register(MINUTE_REGISTER);
        hour = get_RTC_register(HOUR_REGISTER);
        day = get_RTC_register(DAY_REGISTER);
        month = get_RTC_register(MONTH_REGISTER);
        year = get_RTC_register(YEAR_REGISTER);
    } while ((previousSecond!= second) || (previousMinute!= minute) || (previousHour!= hour) ||
             (previousDay!= day) || (previousMonth!= month) || (previousYear!= year));

    // Read register B
    registerB = get_RTC_register(REGISTER_B);

    // Convert BCD to binary
    if (!(registerB & 0x04)) {
        second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
        day = (day & 0x0F) + ((day / 16) * 10);
        month = (month & 0x0F) + ((month / 16) * 10);
        year = (year & 0x0F) + ((year / 16) * 10);
    }

    // Adjust hour for 12-hour mode
    if (!(registerB & 0x02) && (hour & 0x80)) {
        hour = ((hour & 0x7F) + 12) % 24;
    }
}

#endif