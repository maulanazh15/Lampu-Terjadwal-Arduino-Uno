#include <Arduino.h>
#include <unity.h>
#include "LogikaWaktu.h"

// Sebelum test dijalankan
void setUp(void) {
    // Kosongkan jika tidak ada inisialisasi pin/hardware spesifik untuk test
}

// Setelah test dijalankan
void tearDown(void) {
    // Kosongkan
}

// --- TEST CASE 1: Memastikan lampu menyala di malam hari ---
void test_jadwal_lampu_nyala(void) {
    TEST_ASSERT_TRUE(cekStatusLampu(17, 10)); // Tepat saat mulai
    TEST_ASSERT_TRUE(cekStatusLampu(18, 30)); // Tengah malam / Malam hari
    TEST_ASSERT_TRUE(cekStatusLampu(0, 0));   // Pergantian hari
    TEST_ASSERT_TRUE(cekStatusLampu(6, 9));   // 1 menit sebelum mati
}

// --- TEST CASE 2: Memastikan lampu mati di siang hari ---
void test_jadwal_lampu_mati(void) {
    TEST_ASSERT_FALSE(cekStatusLampu(6, 10));  // Tepat saat mati
    TEST_ASSERT_FALSE(cekStatusLampu(12, 0));  // Tengah hari
    TEST_ASSERT_FALSE(cekStatusLampu(17, 9));  // 1 menit sebelum nyala
}

// --- TEST CASE 3: Memastikan kalkulasi Unix Time tidak error / lompat ke 2099 ---
void test_koreksi_drift_mundur_20_detik(void) {
    // Simulasi waktu RTC saat ini: 24 Feb 2026, 12:00:20
    RtcDateTime waktuSimulasi(2026, 2, 24, 12, 0, 20);
    
    // Terapkan fungsi koreksi mundur 20 detik
    RtcDateTime hasilKoreksi = hitungKoreksiDrift(waktuSimulasi, -20);
    
    // Validasi apakah hasilnya tepat 12:00:00 (mundur 20 detik tanpa merusak tahun)
    TEST_ASSERT_EQUAL_INT16(2026, hasilKoreksi.Year());
    TEST_ASSERT_EQUAL_INT8(2, hasilKoreksi.Month());
    TEST_ASSERT_EQUAL_INT8(24, hasilKoreksi.Day());
    TEST_ASSERT_EQUAL_INT8(12, hasilKoreksi.Hour());
    TEST_ASSERT_EQUAL_INT8(0, hasilKoreksi.Minute());
    TEST_ASSERT_EQUAL_INT8(0, hasilKoreksi.Second()); 
}

void setup() {
    delay(2000); // Wajib ada delay agar koneksi Serial siap sebelum test dimulai
    
    UNITY_BEGIN(); // Mulai testing
    
    RUN_TEST(test_jadwal_lampu_nyala);
    RUN_TEST(test_jadwal_lampu_mati);
    RUN_TEST(test_koreksi_drift_mundur_20_detik);
    
    UNITY_END(); // Selesai
}

void loop() {
    // Dibiarkan kosong, karena ini hanya running test
}