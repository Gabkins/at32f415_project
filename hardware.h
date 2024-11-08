#ifndef  _HARDWARE_H_
#define  _HARDWARE_H_
/* Global definitions for hardware configuration */
#include "at32f415.h"
#include "system_at32f415.h"
#include "at32f415_conf.h"


#define _DISABLE_       0
#define _ENABLE_        1

/******************************************************************************/
                           /* I2C Eeprom */
/******************************************************************************/
//////////////////////////////////////////////////////////////////////////////
// ##########################   М А К Р О С Ы    ##############################
//////////////////////////////////////////////////////////////////////////////

#define io_port(_p, _b) (GPIO##_p)
#define io_pin(_p, _b) (1 << _b)
#define io_pin_source(_p, _b) (_b)

#define PORT(port) io_port(port)
#define PIN(pin) io_pin(pin)
#define PIN_Source(pin) io_pin_source(pin)

#define ios_bit(_p, _b) (GPIO_PINS_##_b)
#define ioc_bit(_p, _b) (GPIO_PINS_##_b)
#define ioch_bit(_p, _b) (GPIO_PINS_##_b)

#define setportbit(_addr, _b) (_addr->scr = _b)
#define clrportbit(_addr, _b) (_addr->clr = _b)
#define checkportbit(_addr, _b) ((_addr->idt) & _b)

#define io_set(port_bit) setportbit(io_port(port_bit), ios_bit(port_bit))
#define io_clr(port_bit) clrportbit(io_port(port_bit), ioc_bit(port_bit))
#define io_check(port_bit) checkportbit(io_port(port_bit), ioch_bit(port_bit))


/* Для переменных */
#define BIT(_BIT) (1 << (_BIT))
#define SETBIT(_ADDRESS, _BIT) (_ADDRESS) |= BIT(_BIT)
#define CLRBIT(_ADDRESS, _BIT) (_ADDRESS) &= ~BIT(_BIT)
#define CHKBIT(_ADDRESS, _BIT) ((_ADDRESS)&BIT(_BIT))
#define SHFBIT(n) (1 << (n))

#define TIM_PAUSE TMR10
#define TIM_PAUSE_CLOCK CRM_TMR10_PERIPH_CLOCK
#define SYSCLK_FREQ 48000000

// HARDWARE I2C
#define  I2CB              I2C1
#define  SDA_EE            B,7 //SDA
#define  SCL_EE            B,6 //SCL
#define  EEPROM_WP         C,5 //WP

#define  EEPROM_WP_ON()    io_set(EEPROM_WP)
#define  EEPROM_WP_OFF()   io_clr(EEPROM_WP)

// PROGRAM I2C
#define  SET_SDA()         io_set(SDA_EE)
#define  CLR_SDA()         io_clr(SDA_EE)
#define  SET_SCL()         io_set(SCL_EE)
#define  CLR_SCL()         io_clr(SCL_EE)

#define  set_sda()         io_set(SDA_EE)
#define  clr_sda()         io_clr(SDA_EE)
#define  set_scl()         io_set(SCL_EE)
#define  clr_scl()         io_clr(SCL_EE)

#define  READ_SDA()        io_check(SDA_EE)
#define  READ_SCL()        io_check(SCL_EE)

//////////////////////////////////////////////////////////////////////////////
// ##########################    INDICATION AND BUTTONS   #########################
//////////////////////////////////////////////////////////////////////////////
#define LDC B,5
#define LDD B,4
#define KN B,3
#define STR B,9
#define DATA C,11
#define CLK C,10

//////////////////////////////////////////////////////////////////////////////
// ##########################    FLASH AND EEPROM  #########################
//////////////////////////////////////////////////////////////////////////////
#define CS1 A,4
#define SCKL1 A,5
#define MISO1 A,6
#define MOSI1 A,7
#define SIO3 A,2
#define WP A,3

#define SCL B,6
#define SDA B,7
#define WP_EE C,5

//////////////////////////////////////////////////////////////////////////////
// ##########################    RS-485  #########################
//////////////////////////////////////////////////////////////////////////////
#define RXD A,10
#define TXD A,9
#define TR A,8

/******************************************************************************/
                          /* SPI Flash MAX25L1633E */
/******************************************************************************/
////////////  FLASH MX25L1633E 2Mb/////////
////////////  FLASH MX25L3235E 4Mb/////////
#define  SPI_FLASH         SPI1
#define  FLASH_SIO3        A,2 //SIO3 // должна быть подтянута к 1
#define  FLASH_WP          A,3 //WP
#define  FLASH_CS          A,4 //NSS1
#define  FLASH_SCLK        A,5 //SCLK1
#define  FLASH_MISO        A,6 //MISO1
#define  FLASH_MOSI        A,7 //MOSI1
#define  FLASH_WP_ON()     io_clr(FLASH_WP)
#define  FLASH_WP_OFF()    io_set(FLASH_WP)
#define  DFCS_ON()         io_clr(FLASH_CS)
#define  DFCS_OFF()        io_set(FLASH_CS)


//////////////////////////////////////////////////////////////////////////////
// ##########################    ADC  #########################
//////////////////////////////////////////////////////////////////////////////
#define SPI_ADC SPI2
#define CS2 B,12
#define SCKL2 B,13
#define MISO2 B,14
#define MOSI2 B,15
#define  ADC_RDY_STATE()   (!io_check(MISO2))

#define ADC_CS_ON io_clr(CS2)
#define ADC_CS_OFF io_set(CS2)

/******************************************************************************/
                                /* ADC IN */
/******************************************************************************/
#define  ADC_ACC_PIN       C,1  // канал 11
#define  ADC_ACC           ADC1

/******************************************************************************/
                                /* USB */
/******************************************************************************/
#define  USB_DP            A,12 //DATA+
#define  USB_DM            A,11 //DATA-
/******************************************************************************/

#endif /* _HARDWARE_H_ */

