// CONNECTIONS:
// DS1302 CLK/SCLK --> 5
// DS1302 DAT/IO --> 4
// DS1302 RST/CE --> 2
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

#include <RtcDS1302.h>

// ==== PENGATURAN LOG/DEBUG ====
// Ubah menjadi 'false' jika Anda sudah tidak ingin melihat log di Serial Monitor
const bool DEBUG_MODE = false; 
const bool FIX_TIME = false;

// Pin RTC
ThreeWire myWire(4, 5, 2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

// Pin Relai
#define RELAY1_PIN 9
#define RELAY2_PIN 10

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime &dt)
{
    if (DEBUG_MODE) {
        char datestring[26];
        snprintf_P(datestring,
                   countof(datestring),
                   PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
                   dt.Month(),
                   dt.Day(),
                   dt.Year(),
                   dt.Hour(),
                   dt.Minute(),
                   dt.Second());
        Serial.print(datestring);
    }
}

void setup()
{
    if (DEBUG_MODE) {
        Serial.begin(115200);
        Serial.print("compiled: ");
        Serial.print(__DATE__);
        Serial.println(__TIME__);
    }

    // Set pin relai sebagai output
    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);

    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    
    if (DEBUG_MODE) {
        Serial.println();
    }

    if (!Rtc.IsDateTimeValid())
    {
        if (DEBUG_MODE) Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
        if (DEBUG_MODE) Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        if (DEBUG_MODE) Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled)
    {
        if (DEBUG_MODE) Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled)
    {
        if (DEBUG_MODE) Serial.println("RTC is newer than compile time. (this is expected, but still we need to update, if necessary (the drift is too far))");

        if (FIX_TIME) {
            Rtc.SetDateTime(compiled);
        }
    }
    else if (now == compiled)
    {
        if (DEBUG_MODE) Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }
}

void loop()
{
    RtcDateTime now = Rtc.GetDateTime();

    // ==============================================================
    // FITUR KOMPENSASI DRIFT OTOMATIS (Mundur 20 detik setiap hari)
    // ==============================================================
    static uint8_t lastDay = 99; // Variabel statis untuk melacak perubahan hari
    
    // Jika hari berganti (melewati tengah malam)
    if (now.Day() != lastDay && lastDay != 99) {
        int driftCorrectionSeconds = -20; 
        
        // Ambil waktu saat ini dalam format Unix Epoch, tambahkan koreksi (-20 detik)
        uint32_t correctedEpoch = now.Unix32Time() + driftCorrectionSeconds;
        
        // Simpan waktu yang sudah dikoreksi kembali ke modul RTC
        Rtc.SetDateTime(RtcDateTime(correctedEpoch));
        
        // Perbarui variabel 'now' dengan waktu yang baru dikoreksi
        now = Rtc.GetDateTime(); 
        
        if (DEBUG_MODE) {
            Serial.println("Kompensasi waktu harian diterapkan: -20 detik");
        }
    }
    lastDay = now.Day(); // Simpan tanggal hari ini untuk pengecekan besok
    // ==============================================================

    printDateTime(now);
    
    if (DEBUG_MODE) {
        Serial.println();
        Serial.print("Last Day: ");
        Serial.print(lastDay);
        Serial.print(", Now Day: ");
        Serial.println(now.Day());
    }

    if (!now.IsValid())
    {
        if (DEBUG_MODE) Serial.println("RTC lost confidence in the DateTime!");
    }

    // Atur jadwal nyala/mati lampu
    int startHour = 17; // Jam mulai nyala
    int startMinute = 10;
    int endHour = 6; // Jam mati
    int endMinute = 10;

    bool lampuNyala = false;

    if (startHour < endHour || (startHour == endHour && startMinute < endMinute))
    {
        // Time period does not span midnight
        if ((now.Hour() > startHour || (now.Hour() == startHour && now.Minute() >= startMinute)) &&
            (now.Hour() < endHour || (now.Hour() == endHour && now.Minute() < endMinute)))
        {
            lampuNyala = true;
        }
        if (DEBUG_MODE) Serial.println("Time period does not span midnight");
    }
    else
    {
        // Time period spans midnight
        if ((now.Hour() > startHour || (now.Hour() == startHour && now.Minute() >= startMinute)) ||
            (now.Hour() < endHour || (now.Hour() == endHour && now.Minute() < endMinute)))
        {
            lampuNyala = true;
        }
        if (DEBUG_MODE) Serial.println("Time period spans midnight");
    }

    if (DEBUG_MODE) {
        Serial.print("lampuNyala: ");
        Serial.println(lampuNyala);
    }

    if (lampuNyala)
    {
        digitalWrite(RELAY1_PIN, HIGH); // Nyalakan lampu 1
        digitalWrite(RELAY2_PIN, HIGH); // Nyalakan lampu 2
        if (DEBUG_MODE) Serial.println("Lampu nyala");
    }
    else
    {
        digitalWrite(RELAY1_PIN, LOW); // Matikan lampu 1
        digitalWrite(RELAY2_PIN, LOW); // Matikan lampu 2
        if (DEBUG_MODE) Serial.println("Lampu mati");
    }

    delay(10000); // Delay 10 detik
}