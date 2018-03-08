#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cctype>
#include <string>

#ifndef MERLIN_LIB
#define MERLIN_LIB



/**
 * Stand des Projekts:
 * Das Programm lässt den Merlin-Head um einen angegebenen Winkel im Uhrzeigersinn drehen, dreht den oberen Motor um 10°,
 * dreht den Kopf wieder auf die Urstellung zurück und dreht den oberen Motor um 10°. Dies geschieht solange, bis eine
 * Halbkugel abgelaufen wurde.
 *
 * Sequenzen die nicht im Detail beschrieben wurden, sind in der WIKI-Dokumentation unter Ressourcen zu finden.
 */

// Funktionsprototypen - Beschreibungen siehe an den entsprechenden Funktionen
int initCOM();

unsigned char *initServos(unsigned char *BUF_TX, unsigned char *TX);

unsigned char *stopMotor(unsigned char *TX, unsigned char axis);

unsigned char *gotoDegree(unsigned char *TX, long degree, unsigned char axis);

unsigned char *startMotor(unsigned char *TX, unsigned char axis);

unsigned char *readAxisState(unsigned char *TX, unsigned char axis);

unsigned char *mainSequence(unsigned char *TX, int angle, char axis);

void doSequenceStep(int angle, char axis);

void checkServoMoving(char axis);


// Winkel um den Motor 2 nach jedem Durchlauf erhöht werden soll
const int ANGLE_UP = 10;

// Winkel um den sich Motor 1 drehen soll, bevor sich Motor 2 bewegt - Für den Produkttivbetrieb 360
const int ANGLE_SIDE = 360;

// Pivot-Winkel - Wird das Gerät eingeschaltet ist dies die Referenzstellung
static const int PIVOT_ANGLE = 0x800000;

// Größe eines 1° Winkels - Derzeit empierischer Wert, nicht ganz korrekt.
static const int SINGLE_DEGREE = 0xA00;

// Konstante um Motor1 (untere Motor) anzusprechen
#define ENGINE_BOTTOM ((const unsigned char)'1')

// Konstante um Motor2 (oberen Motor) anzusprechen
#define ENGINE_UP ((const unsigned char)'2')


//int main()
//{
//    // Hauptprogrammschleife
//    // Bewegt Motor 1 immer um ANGLE_SIDE im Uhrzeigersinn, dann Motor 2 um ANGLE_UP Im Uhrzeigersinn, dann Motor 1 um
//    // ANGLE_SIDE gegen den Uhrezigersinn etc.
//    for (int sequence = 0; sequence < 90 / ANGLE_UP; sequence++)
//    {
//        if (sequence % 2 == 0)
//        {
//            // Motor 1 um ANGLE_SIDE drehen
//            doSequenceStep(ANGLE_SIDE, ENGINE_BOTTOM);
//        } else
//        {
//            // Motor 2 um ANGLE_SIDE zurück drehen (auf 0°)
//            doSequenceStep(0, ENGINE_BOTTOM);
//        }
//
//        // Motor 2 um ANGLE_UP weiter drehen
//        doSequenceStep(ANGLE_UP * (sequence + 1), ENGINE_UP);
//    }
//
//    return 0;
//}

/**
 * Sendet mainSequence auf die Serielle Schnittstelle und startet den Ablauf um mit einem Motor auf einen bestimmten
 * Winkel zu fahren.
 * Parameter: angle = Zielwinkel, axis = Nummer des Motors
 */
void doSequenceStep(int angle, char axis)
{
    // Größe des Ein- und Ausgabepuffers für RX und TX des seriellen Ports des Raspberry
    const int SIZE = 80;

    // Öffnen und initialisieren des Streams
    int uart0_filestream = initCOM();;

    // RX- und TX Puffer anlegen
    unsigned char BUF_TX[SIZE];
    unsigned char BUF_RX[SIZE];

    // Referenz auf TX-Puffer (später nur noch TX genannt)
    unsigned char *TX;

    // Initialisierungssequenz der Motoren 1 und 2 in TX schreiben
    TX = initServos(BUF_TX, TX);

    // mainSequence starten und dessen Werte in TX schreiben
    TX = mainSequence(TX, angle, axis);

    if (uart0_filestream != -1)
    {
        int out = -1;

        // Befehle, die in TX gepuffert sind auf der seriellen Schnittstelle ausgeben
        for (int i = 0; i < SIZE; ++i)
        {
            out = write(uart0_filestream, &BUF_TX[i], sizeof(BUF_TX[i]));

            // WICHIG! Delay [10ms] zwischen den gesendeten Characters
            usleep(10000);

            if (BUF_TX[i] == '\r')
            {
                // WICHIG! Delay [10ms] zwischen zwei gesendeten Befehlen
                usleep(10000);
            }
        }
    } // if uart0

    // Stream schließen
    close(uart0_filestream);

    // Prüfroutine für den laufenden Motor starten, testet ob ein Motor Bewegungsstatus 0 erreicht hat.
    checkServoMoving(axis);
}


/**
 * Prüft ob sich der angegebene Motor (laut den Daten des Merlins) bewegt.
 * ACHTUNG: Der gesendete Datenstream stimmt nicht mit dem überein, was man sieht. Auch wenn ein Motor offensichtlich
 *          schon steht, ist dieser unter Umständen noch in Bewegung
 */
void checkServoMoving(char axis)
{
    // Größe des Ein- und Ausgabepuffers für RX und TX des seriellen Ports des Raspberry
    const int SIZE_NEW = 5;

    // Abbruchbedingung der While-Schleife
    int flag = 1;

    /**
     * Sende :j1 oder :j2 und Prüfe solange dessen Output, bis der RX Stream bekannt gibt, dass der Motor sich nicht mehr
     * bewegt
     */
    while (flag)
    {

        // Öffnen und Initialisieren des Streams
        int uart0_filestream = initCOM();

        // RX- und TX Puffer anlegen
        unsigned char BUF_TX[SIZE_NEW];
        unsigned char BUF_RX[SIZE_NEW];

        // Referenz auf TX- und RX-Puffer (später nur noch TX und RX genannt)
        unsigned char *TX;
        unsigned char *RX;

        // Iterierbare Referenz auf RX und TX
        TX = &BUF_TX[0];
        RX = &BUF_RX[0];

        // readAxisState starten und dessen Werte in TX schreiben
        TX = readAxisState(TX, axis);

        if (uart0_filestream != -1)
        {
            int out = -1;

            // Befehle, die in TX gepuffert sind auf der seriellen Schnittstelle ausgeben
            for (int i = 0; i < SIZE_NEW; ++i)
            {
                out = write(uart0_filestream, &BUF_TX[i], sizeof(BUF_TX[i]));

                // WICHIG! Delay [10ms] zwischen den gesendeten Characters
                usleep(10000);


                if (BUF_TX[i] == '\r')
                {
                    // WICHIG! Delay [10ms] zwischen zwei gesendeten Befehlen
                    usleep(10000);
                }

                // Puffer, der die RX Ausgabe managt
                char output[20];

                // Auslesen des RX-Streams
                int rx_length = read(uart0_filestream, (void *) BUF_RX, SIZE_NEW);
                BUF_RX[SIZE_NEW] = '\0';

                // Prüfung ob im 4. Durchlauf (entspricht der 4. Antwort) die Antwort die Form XXX0X hat
                // Auf :j2 können zwei Antworten folgen:
                //  - =X0X (X steht für eine beliebige (für uns bedeutungslose Zahl) - Motor steht
                //  - =X1X (X steht für eine beliebige (für uns bedeutungslose Zahl) - Motor läuft
                if (i == SIZE_NEW - 1)
                {
                    if (RX[3] == '0')
                    {
                        flag = 0;
                    }
                }
            }
        } // if uart0

        // Stream schließen
        close(uart0_filestream);
    }
}


/**
 * Stoppt Motor 2, lässt diesen um ANGLE_SIDE weiter rotieren und stoppt.
 */
unsigned char *mainSequence(unsigned char *TX, int angle, char axis)
{
    // Motor stoppen
    TX = stopMotor(TX, axis);

    // Motor um ANGLE_UP weiter rotieren lassen
    TX = gotoDegree(TX, (long) angle, axis);

    // Motor starten
    TX = startMotor(TX, axis);

    return TX;
}


/**
 * Sendet Befehl :f1\r oder :f2\r
 * liest den Status der ausgewählten Achse aus
 * Gibt auf RX eine Antwort der Form: =X0X oder =X1X
 * X sind Zahlen, derer Bedeutung derzeit unbekannt ist.
 */
unsigned char *readAxisState(unsigned char *TX, unsigned char axis)
{
    *TX++ = ':';
    *TX++ = 'f';
    *TX++ = axis;
    *TX++ = '\r';
    return TX;
}

/**
 * Sendet Befehl :J1\r oder :J2\r
 * startet den gewählten Motor
 */
unsigned char *startMotor(unsigned char *TX, unsigned char axis)
{
    *TX++ = ':';
    *TX++ = 'J';
    *TX++ = axis;
    *TX++ = '\r';
    return TX;
}

/**
 * Sendet Befehl :L1\r oder :L2\r
 * stoppt den gewählten Motor
 */
unsigned char *stopMotor(unsigned char *TX, unsigned char axis)
{
    *TX++ = ':';
    *TX++ = 'L';
    *TX++ = axis;
    *TX++ = '\r';
    return TX;
}

/**
 * Fährt den gewählten Motor an einen angegebenen Winkel.
 * degree wird in Grad angegeben (Drehung auf Position 90° durch 90, Drehung auf 45° durch 45 etc)
 */
unsigned char *gotoDegree(unsigned char *TX, long degree, unsigned char axis)
{
    // Referenzwert für Ausgangsstellung nach Einschalten des Merlin
    long pivotAngle = PIVOT_ANGLE;

    // Hexwert, der dem Winkel 1° entspricht
    // TODO: 0xA00 trifft nicht genau 1°. Versuchter Näherungswert. Der Wert der sich aus der Onlinedoku ergeben würde
    //       kann nicht benutzt werden!!!
    long singleDegree = SINGLE_DEGREE;

    // Winkel der angefahren werden soll
    long targetAngle = 0x00000;

    // Wert für den Winkel der auf den Referenzwert der Ausgangsstellung addiert werden soll.
    long angleInterval = 0x00000;

    // enthält später High-Byte von targetAngle
    char blockHigh[8];
    // enthält später Mittleres-Byte von targetAngle
    char blockMid[8];
    // enthält später Low-Byte von targetAngle
    char blockLow[8];

    // Erstes und zweites Byte
    unsigned char helperHighHigh = 0;
    unsigned char helperHighLow = 0;

    // Drittes und viertes Byte
    unsigned char helperMidHigh = 0;
    unsigned char helperMidLow = 0;

    // Fünftes und sechstes Byte
    unsigned char helperLowHigh = 0;
    unsigned char helperLowLow = 0;

    // Berechnung des zu addierenden Werts. z.B: SINGLE_DEGREE * 90
    angleInterval = degree * singleDegree;
    // Addition von angleInterval auf den Referenzwert der Ausgangsstellung um den Zielwinkel zu berechnen
    targetAngle = pivotAngle + angleInterval;

    // Füllen von blockLow, blockMid, blockHigh.
    // Es wird jeweils targetAngle genommen und mit der entsprechenden Bitmaske verunded um nur die ersten 2 Bytes, die
    // nächsten 2 Bytes oder die letzten 2 Bytes auszulesen. Durch Shiften werden nur nch die benötigten Bytes gefiltert.
    // Diese werden jeweils als char in blockLow (etc) gespeichert um diese auf der seriellen Schnittstelle auszugeben.
    // Anmerkung: Kleiner Work-Around, da sprintf mit long nicht funktioniert!
    sprintf(blockLow, "%x", (unsigned int) (targetAngle & 0xFF0000) >> 16);
    sprintf(blockMid, "%x", (unsigned int) (targetAngle & 0x00FF00) >> 8);
    sprintf(blockHigh, "%x", (unsigned int) (targetAngle & 0x0000FF) >> 0);

    // Prüfen, ob ein extrahierter char NULL ist, falls ja: durch '0' ersetzen, ansonsten Originalwert benutzen
    helperHighHigh = ((int) blockHigh[1] == 0) ? '0' : (unsigned char) toupper(blockHigh[1]);
    helperHighLow = ((int) blockHigh[0] == 0) ? '0' : (unsigned char) toupper(blockHigh[0]);
    helperMidHigh = ((int) blockMid[1] == 0) ? '0' : (unsigned char) toupper(blockMid[1]);
    helperMidLow = ((int) blockMid[0] == 0) ? '0' : (unsigned char) toupper(blockMid[0]);
    helperLowHigh = ((int) blockLow[1] == 0) ? '0' : (unsigned char) toupper(blockLow[1]);
    helperLowLow = ((int) blockLow[0] == 0) ? '0' : (unsigned char) toupper(blockLow[0]);

    // Füllen von TX mit der Sequenz um zu einem Winkel drehen zu können
    *TX++ = ':';
    *TX++ = 'G';
    *TX++ = axis;
    *TX++ = '0';
    *TX++ = '0';
    *TX++ = '\r';
    *TX++ = ':';
    *TX++ = 'S';
    *TX++ = axis;
    *TX++ = helperHighLow;
    *TX++ = helperHighHigh;
    *TX++ = helperMidLow;
    *TX++ = helperMidHigh;
    *TX++ = helperLowLow;
    *TX++ = helperLowHigh;
    *TX++ = '\r';
    return TX;
}

/**
 * Bewegt Motor 1 auf seine Urposition, falls dieser noch nicht an dieser Position steht.
 */
unsigned char *InitPositionL1(unsigned char *TX)
{
    *TX++ = ':';
    *TX++ = 'G';
    *TX++ = '1';
    *TX++ = '0';
    *TX++ = '0';
    *TX++ = '\r';
    *TX++ = ':';
    *TX++ = 'S';
    *TX++ = '1';
    *TX++ = '0';
    *TX++ = '0';
    *TX++ = '0';
    *TX++ = '0';
    *TX++ = '8';
    *TX++ = '0';
    *TX++ = '\r';
    return TX;
}

/**
 * Bewegt Motor 2 auf seine Urposition, falls dieser noch nicht an dieser Position steht.
 */
unsigned char *InitPositionL2(unsigned char *TX)
{
    *TX++ = ':';
    *TX++ = 'G';
    *TX++ = '2';
    *TX++ = '0';
    *TX++ = '0';
    *TX++ = '\r';
    *TX++ = ':';
    *TX++ = 'S';
    *TX++ = '2';
    *TX++ = '0';
    *TX++ = '0';
    *TX++ = '0';
    *TX++ = '0';
    *TX++ = '8';
    *TX++ = '0';
    *TX++ = '\r';
    return TX;
}

/**
 * Initialisierungssequenz beider Servomotoren
 */
unsigned char *initServos(unsigned char *BUF_TX, unsigned char *TX)
{
    TX = &BUF_TX[0];
    *TX++ = ':';
    *TX++ = 'F';
    *TX++ = '1';
    *TX++ = '\r';
    *TX++ = ':';
    *TX++ = 'F';
    *TX++ = '2';
    *TX++ = '\r';
    *TX++ = ':';
    *TX++ = 'a';
    *TX++ = '1';
    *TX++ = '\r';
    *TX++ = ':';
    *TX++ = 'D';
    *TX++ = '1';
    *TX++ = '\r';
    *TX++ = ':';
    *TX++ = 'a';
    *TX++ = '2';
    *TX++ = '\r';
    *TX++ = ':';
    *TX++ = 'D';
    *TX++ = '2';
    *TX++ = '\r'; // Bis hier init

    return TX;
}

/**
 * Initialisierung der Seriellen Schnittstelle
 */
int initCOM()
{
    int uart0_filestream = -1;
    uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1)
    {
        printf("[ERROR] UART0 open()\n");
    }

    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
    return uart0_filestream;
}

#endif