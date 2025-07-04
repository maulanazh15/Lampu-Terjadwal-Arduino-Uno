
---

# 💡 Proyek Arduino: Kontrol Lampu Otomatis Berbasis Waktu dengan RTC DS1302

Proyek ini bertujuan untuk mengontrol dua lampu secara otomatis menggunakan modul RTC DS1302. Lampu akan menyala dan mati pada waktu yang telah ditentukan, misalnya dari pukul 17:10 hingga 06:10.

## 🧰 Komponen yang Digunakan

* Arduino Uno (atau kompatibel)
* Modul RTC DS1302
* 2 x Modul Relay
* Kabel Jumper
* Sumber daya 5V([Lonely Binary][1])

## 🔌 Koneksi Perangkat Keras

| Pin DS1302 | Pin Arduino |   |
| ---------- | ----------- | - |
| CLK/SCLK   | D5          |   |
| DAT/IO     | D4          |   |
| RST/CE     | D2          |   |
| VCC        | 3.3V - 5V   |   |
| GND        | GND         |   |

| Relay   | Pin Arduino |   |
| ------- | ----------- | - |
| Relay 1 | D9          |   |
| Relay 2 | D10         |   |

## 📦 Library yang Digunakan

Proyek ini menggunakan library [Rtc by Makuna](https://github.com/Makuna/Rtc) untuk berkomunikasi dengan modul RTC DS1302.([Arduino Libraries][2])

### 🔧 Instalasi Library

1. Buka Arduino IDE.
2. Masuk ke menu **Sketch > Include Library > Manage Libraries...**
3. Di kotak pencarian, ketik "Rtc by Makuna".
4. Pilih library tersebut dan klik **Install**.([Wolles Elektronikkiste][3])

## 🛠️ Fitur Utama

* Kontrol otomatis dua lampu berdasarkan waktu yang ditentukan.
* Menggunakan RTC DS1302 untuk menjaga waktu tetap akurat meskipun Arduino dimatikan.
* Fitur debugging yang dapat diaktifkan atau dinonaktifkan dengan mudah menggunakan flag `DEBUG`.

## 🖥️ Penggunaan Flag DEBUG

Untuk mempermudah proses debugging selama pengembangan, proyek ini menggunakan flag `DEBUG`. Dengan mengatur nilai flag ini, Anda dapat mengaktifkan atau menonaktifkan output serial untuk debugging.

### 🔧 Mengaktifkan DEBUG

Untuk mengaktifkan output serial debugging, ubah nilai `DEBUG` menjadi `1`:

```cpp
#define DEBUG 1
```



### 🚫 Menonaktifkan DEBUG

Untuk menonaktifkan output serial debugging (misalnya, saat proyek sudah dalam tahap produksi), ubah nilai `DEBUG` menjadi `0`:

```cpp
#define DEBUG 0
```



Dengan menggunakan flag ini, Anda dapat dengan mudah mengontrol output serial tanpa harus menghapus atau mengomentari setiap baris `Serial.print()` secara manual.

## 📄 Lisensi

Proyek ini menggunakan library [Rtc by Makuna](https://github.com/Makuna/Rtc) yang dilisensikan di bawah [LGPL 3.0](https://www.gnu.org/licenses/lgpl-3.0.html).([GitHub][4])

---

Untuk informasi lebih lanjut dan dokumentasi lengkap, silakan kunjungi [Wiki Library Rtc by Makuna](https://github.com/Makuna/Rtc/wiki).([GitHub][5])

[1]: https://lonelybinary.com/blogs/learn/ds1302?srsltid=AfmBOorRzhSfIIAt3gvFXF7OTvTr3-jNsMrl_A9UYZXs-AlshoJTfh2Z&utm_source=chatgpt.com "Using the Real-Time Clock DS1302 module with Arduino UNO R3"
[2]: https://www.arduinolibraries.info/libraries/rtc-by-makuna?utm_source=chatgpt.com "Rtc by Makuna - Arduino Library List"
[3]: https://wolles-elektronikkiste.de/en/ds1302-and-ds1307-rtc-real-time-clock?utm_source=chatgpt.com "DS1302 and DS1307 RTC Real-Time Clock - Wolles Elektronikkiste"
[4]: https://github.com/Makuna/Rtc?utm_source=chatgpt.com "Makuna/Rtc - GitHub"
[5]: https://github.com/Makuna/Rtc/blob/master/src/RtcDS1302.h?utm_source=chatgpt.com "Rtc/src/RtcDS1302.h at master · Makuna/Rtc - GitHub"
