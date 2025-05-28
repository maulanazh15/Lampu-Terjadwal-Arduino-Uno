// CONNECTIONS:
// DS1302 CLK/SCLK --> 5
// DS1302 DAT/IO --> 4
// DS1302 RST/CE --> 2
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND

#include <RtcDS1302.h>

// ==== DEBUG FLAG ====
#define DEBUG 0

// Pin RTC
ThreeWire myWire(4, 5, 2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

// Pin Relai
#define RELAY1_PIN 9
#define RELAY2_PIN 10

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime &dt)
{
#if DEBUG
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
#endif
}

void setup()
{
#if DEBUG
    Serial.begin(57600);
    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);
#endif

    // Set pin relai sebagai output
    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);

    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
#if DEBUG
    Serial.println();
#endif

    if (!Rtc.IsDateTimeValid())
    {
#if DEBUG
        Serial.println("RTC lost confidence in the DateTime!");
#endif
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
#if DEBUG
        Serial.println("RTC was write protected, enabling writing now");
#endif
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
#if DEBUG
        Serial.println("RTC was not actively running, starting now");
#endif
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled)
    {
#if DEBUG
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
#endif
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled)
    {
#if DEBUG
        Serial.println("RTC is newer than compile time. (this is expected, but still we need to update)");
#endif
        Rtc.SetDateTime(compiled);
    }
    else if (now == compiled)
    {
#if DEBUG
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
#endif
    }
}

void loop()
{
    RtcDateTime now = Rtc.GetDateTime();

    printDateTime(now);
#if DEBUG
    Serial.println();
#endif

    if (!now.IsValid())
    {
#if DEBUG
        Serial.println("RTC lost confidence in the DateTime!");
#endif
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
#if DEBUG
        Serial.println("Time period does not span midnight");
#endif
    }
    else
    {
        // Time period spans midnight
        if ((now.Hour() > startHour || (now.Hour() == startHour && now.Minute() >= startMinute)) ||
            (now.Hour() < endHour || (now.Hour() == endHour && now.Minute() < endMinute)))
        {
            lampuNyala = true;
        }
#if DEBUG
        Serial.println("Time period spans midnight");
#endif
    }

#if DEBUG
    Serial.print("lampuNyala: ");
    Serial.println(lampuNyala);
#endif

    if (lampuNyala)
    {
        digitalWrite(RELAY1_PIN, HIGH); // Nyalakan lampu 1
        digitalWrite(RELAY2_PIN, HIGH); // Nyalakan lampu 2
#if DEBUG
        Serial.println("Lampu nyala");
#endif
    }
    else
    {
        digitalWrite(RELAY1_PIN, LOW); // Matikan lampu 1
        digitalWrite(RELAY2_PIN, LOW); // Matikan lampu 2
#if DEBUG
        Serial.println("Lampu mati");
#endif
    }

    delay(10000); // Delay 10 detik
}
