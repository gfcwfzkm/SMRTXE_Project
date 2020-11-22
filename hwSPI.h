/**
 * @mainpage Hardware-SPI Bibliothek für Atmel AVR Mikrocontroller
 * @brief Die Bibliothek erlaubt die einfache Anwendung der Hardware-SPI
 * Peripherie des Atmel Microcontrollers und kann nachträglich vom
 * Anwender mit einer Interrupt-Routine erweitert werden.
 *
 * @version 1.0
 * -Veröffentlichung der Bibliothek
 *
 * @author gfcwfzkm
 * @date 25.09.2015
 */

/**
 * @file hwSPI.h
 * @brief Headerdatei der Hardware-SPI Bibliothek
 */

#ifndef HWSPI_H_
#define HWSPI_H_

#include <avr/io.h>

/**
 * @brief SPI-Takt = CPU-Takt (Fosc) durch 4 teilen
 */
#define hwSPI_CLOCK_DIV4	0x00
/**
 * @brief SPI-Takt = CPU-Takt (Fosc) durch 16 teilen
 */
#define hwSPI_CLOCK_DIV16	0x01
/**
 * @brief SPI-Takt = CPU-Takt (Fosc) durch 64 teilen
 */
#define hwSPI_CLOCK_DIV64	0x02
/**
 * @brief SPI-Takt = CPU-Takt (Fosc) durch 28 teilen
 */
#define hwSPI_CLOCK_DIV128	0x03
/**
 * @brief SPI-Takt = CPU-Takt (Fosc) durch 2 teilen
 */
#define hwSPI_CLOCK_DIV2	0x04
/**
 * @brief SPI-Takt = CPU-Takt (Fosc) durch 8 teilen
 */
#define hwSPI_CLOCK_DIV8	0x05
/**
 * @brief SPI-Takt = CPU-Takt (Fosc) durch 32 teilen
 */
#define hwSPI_CLOCK_DIV32	0x06

/**
 * @brief SPI-Modus: CPOL: Rising, CPHA: Sample
 */
#define hwSPI_MODE0	0x00
/**
 * @brief SPI-Modus: CPOL: Rising, CPHA: Setup
 */
#define hwSPI_MODE1	0x04
/**
 * @brief SPI-Modus: CPOL: Falling, CPHA: Sample
 */
#define hwSPI_MODE2	0x08
/**
 * @brief SPI-Modus: CPOL: Falling, CPHA: Setup
 */
#define hwSPI_MODE3	0x0C

/**
 * @brief MODE Mask
 */
#define hwSPI_MODE_MASK		0x0C
/**
 * @brief Clock Mask
 */
#define hwSPI_CLOCK_MASK	0x03
/**
 * @brief 2xClock Mask
 */
#define hwSPI_2XCLOCK_MASK	0x01

/**
 * @brief Definition des LSB-Makros
 */
#define hwSPI_LSBFIRST	1

/**
 * @brief Definition des MSB-Makros
 */
#define hwSPI_MSBFIRST	0

/**
 * @brief Hardware-SPI Schnittstelle initialisieren
 *
 * Initialisiert die SPI-Schnittstelle und stellt diesen als Master ein.
 * \a hwSPI_configurePins muss zuerst aufgerufen werden!
 */
void hwSPI_init();

/**
 * @brief Hardware-SPI Pins definieren
 *
 * Übergibt Port, DDR  & die Pins für 'SCK', 'MOSI' und 'SS' and die
 * Bibliothek weiter, welche für die Initialisierung notwendig sind.
 * \n Beispiel: \n \code{.c}
 * hwSPI_configurePins(&PORTB, &DDRB, SCKPIN, MOSIPIN, SSPIN);
 * \endcode Übergibt die SPI-Pins eines ATmega88 (oder ähnlich) an die Bibliothek weiter.
 * @param spiPort Port der SPI-Schnittstelle
 * @param spiDDR DDR-Register des Port's der SPI-Schnittstelle
 * @param sckBIT Pin des Port's der SPI-Schnittstelle
 * @param mosiBIT Pin des Port's der SPI-Schnittstelle
 * @param ssBIT Pin des Port's der SPI-Schnittstelle
 */
void hwSPI_configurePins(volatile uint8_t *spiPort, volatile uint8_t *spiDDR,
						 uint8_t sckBIT, uint8_t mosiBIT, uint8_t ssBIT);

/**
 * @brief SPI-Schnittstelle beenden/ausschalten
 *
 * Deaktiviert die SPI-Schnittstelle. Die eingestellten SPI-Pins bleiben weiterhin
 * gespeichert.
 */
void hwSPI_close();

/**
 * @brief Datenübertragung MSB oder LSB
 *
 * Stellt die SPI-Hardware darauf ein, ob das LSB oder MSB
 * zuerst gesendet werden soll.
 * @param bitOrder Stellt die Bitreihenfolge ein (MSB oder LSB First)
 */
void hwSPI_setBitOrder(uint8_t bitOrder);

/**
 * @brief Datenmodus setzten
 *
 * Stellt den Datenmodus der SPI-Peripherie ein. Siehe dazu
 * das Datenblatt für weitere Infos.
 * @param mode SPI-Modus (z.B. hwSPI_MODE0)
 */
void hwSPI_setDataMode(uint8_t mode);

/**
 * @brief Taktvorteiler einstellen
 *
 * Stellt den Taktvorteiler ein, womit der CPU-Takt
 * für den SPI-BUS vorgeteilt wird (mind.: 2, max.: 128)
 * @param rate Taktvorteiler (z.B. hwSPI_CLOCK_DIV2)
 */
void hwSPI_setClockDivider(uint8_t rate);

/**
 * @brief Einstellungsänderungen anzeigen
 *
 * Gibt zurück, ob und welche Einstellungen geändert wurden 
 * z.B. durch Bibliotheken für andere Peripherie-ICs
 * @return Geänderte Einstellung (xxxx'x ClockDividerChangedBit DatamodeChangedBit BitorderchangedBit)
 */
uint8_t hwSPI_getChangedSettings();

/**
 * @brief Daten senden und empfangen
 *
 * Einfache Funktion zum senden und auslesen
 * von Daten in/aus dem SPI-BUS.
 * \n Beispiel: \n \code{.c}
 * hwSPI_transfer(0xAA);					// Wert 0xAA in den BUS schreiben/senden
 * uint8_t _getData = hwSPI_transfer(0);	// Byte aus dem BUS / Empfangsregister lesen
 * \endcode 
 * @param _data Das zu sendende Byte
 * @return Das ausgelesene Byte
 */
uint8_t hwSPI_transfer(uint8_t _data);


#endif /* HWSPI_H_ */
