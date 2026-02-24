#pragma once
#include <RtcDS1302.h>

// Fungsi untuk mengecek apakah lampu harus nyala/mati berdasarkan jam dan menit
bool cekStatusLampu(uint8_t jam, uint8_t menit) {
    int startHour = 17;
    int startMinute = 10;
    int endHour = 6;
    int endMinute = 10;

    if (startHour < endHour || (startHour == endHour && startMinute < endMinute)) {
        return ((jam > startHour || (jam == startHour && menit >= startMinute)) &&
                (jam < endHour || (jam == endHour && menit < endMinute)));
    } else {
        return ((jam > startHour || (jam == startHour && menit >= startMinute)) ||
                (jam < endHour || (jam == endHour && menit < endMinute)));
    }
}

// Fungsi untuk menghitung kalkulasi kompensasi waktu
RtcDateTime hitungKoreksiDrift(RtcDateTime waktuSekarang, int koreksiDetik) {
    uint32_t correctedUnix = waktuSekarang.Unix32Time() + koreksiDetik;
    RtcDateTime correctedTime;
    correctedTime.InitWithUnix32Time(correctedUnix);
    return correctedTime;
}