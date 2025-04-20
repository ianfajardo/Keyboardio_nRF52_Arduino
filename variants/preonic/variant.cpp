/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.
  Copyright (c) 2016 Sandeep Mistry All right reserved.
  Copyright (c) 2018, Adafruit Industries (adafruit.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "variant.h"
#include "wiring_constants.h"
#include "wiring_digital.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_power.h"


static constexpr uint32_t P1_BANK = 32;

const uint32_t g_ADigitalPinMap[] =
{
  // D0 .. D30
  

  P1_BANK + 13, // D0 is P1.13 is ROW1
  11,           // D1 is P0.11 is ROW2
  P1_BANK + 8,  // D2 is P1.08 is ROW3
  27,           // D3 is P0.27 is ROW4
  8,            // D4 is P0.08 is ROW5
  6,            // D5 is P0.06 is ROW6
  P1_BANK + 14, // D6 is P1.14 is COL1
  P1_BANK + 12, // D7 is P1.12 is COL2
  25,           // D8 is P0.25 is COL3
  26,           // D9 is P0.26 is COL4
  21,           // D10 is P0.21 is COL5
  23,           // D11 is P0.23 is COL6
  12,           // D12 is P0.12 is COL7
  P1_BANK + 9,  // D13 is P1.09 is COL8
  4,            // D14 is P0.04 is COL9
  31,           // D15 is P0.31 is COL10
  30,           // D16 is P0.30 is COL11
  29,           // D17 is P0.29 is COL12
  22,           // D18 is P0.22 is SPEAKER
  24,           // D19 is P0.24 is RGB_DATA
  P1_BANK + 5,  // D20 is P1.05 is RGB_EN
  P1_BANK + 7,  // D21 is P1.07 is BATT_ALERT
  P1_BANK + 2,  // D22 is P1.02 is CHG_STATUS
  P1_BANK + 11, // D23 is P1.11 is ENC1_A
  P1_BANK + 10, // D24 is P1.10 is ENC1_B
  P1_BANK + 15, // D25 is P1.15 is ENC2_A
  3,            // D26 is P0.03 is ENC2_B
  2,            // D27 is P0.02 is ENC3_A
  28,           // D28 is P0.28 is ENC3_B
  5,            // D29 is P0.05 is I2C_SCL
  7,            // D30 is P0.07 is I2C_SDA
  19, 		// D31 is P0.19 is PGOOD
};

void disableUnusedPin(NRF_GPIO_Type *port, uint8_t pin) {
    // Configure for lowest power consumption
    port->PIN_CNF[pin] = ((uint32_t)GPIO_PIN_CNF_DIR_Input        << GPIO_PIN_CNF_DIR_Pos)
                       | ((uint32_t)GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos)
                       | ((uint32_t)GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)
                       | ((uint32_t)GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos)
                       | ((uint32_t)GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos);
}




void initVariant()
{
  // Enable DC/DC converter for better power efficiency
  sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);

 
// Don't touch pins P0.0, P0.1 as they're not routed by the HolyIOT 18010
//  nrf_gpio_cfg_input(0, NRF_GPIO_PIN_PULLDOWN);   // P0.00
//  nrf_gpio_cfg_input(1, NRF_GPIO_PIN_PULLDOWN);   // P0.01

// Don't touch pin P0.18. That's RESET
  // nrf_gpio_cfg_input(18, NRF_GPIO_PIN_PULLDOWN);  // P0.18

 
     // Configure unused P0.xx pins
    disableUnusedPin(NRF_P0, 9);   // P0.09
    disableUnusedPin(NRF_P0, 10);  // P0.10
    disableUnusedPin(NRF_P0, 13);  // P0.13
    disableUnusedPin(NRF_P0, 14);  // P0.14
    disableUnusedPin(NRF_P0, 15);  // P0.15
    disableUnusedPin(NRF_P0, 16);  // P0.16
    disableUnusedPin(NRF_P0, 17);  // P0.17
    disableUnusedPin(NRF_P0, 20);  // P0.20
    
    // Configure unused P1.xx pins
    disableUnusedPin(NRF_P1, 0);  // P1.00
    disableUnusedPin(NRF_P1, 1);  // P1.01
    disableUnusedPin(NRF_P1, 3);  // P1.03
    disableUnusedPin(NRF_P1, 4);  // P1.04
    disableUnusedPin(NRF_P1, 6);  // P1.06
  // Disable debug interface if not actively debugging
  NRF_CLOCK->TRACECONFIG = 0;

  // Disable unused analog inputs
  NRF_SAADC->ENABLE = 0;
}

