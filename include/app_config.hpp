#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_


#include "Arduino.h"


//// Gap service ////
/**********************************************************************************************/
#define GENERIC_ACCESS_SERVICE_UUID             "1800"
#define GENERIC_ACCESS_DEVICE_NAME              "2A00"      // VALUE: device name String, Prop: Read | Notify
#define GENERIC_ACCESS_CONN_PARAM               "2A04"      //VALUE: connParameter HEX, Prop: Read

//// Gatt ////
/**********************************************************************************************/
#define GENERIC_ATTR_SERVICE_UUID               "1801"
#define GENERIC_ACCESS_APPERANCE                "2A01"      // VALUE: appearance HEX, Prop: Read

//// Device information service ////
/**********************************************************************************************/
#define DEVICE_INFO_SERVICE_UUID                "180A"
#define DEVICE_INFO_SERIAL_N                    "2A25"      // VALUE: Serial Number String, Prop: Read
#define DEVICE_INFO_FIRM_REV                    "2A26"      // VALUE: Firmware Revision String, Prop: Read
#define DEVICE_INFO_HARD_REV                    "2A27"      // VALUE: Hardware Revision, String, Prop: Read

//// UART_SERVICE ////
/**********************************************************************************************/
#define UART_SERVICE_UUID                       "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"    /////////////////////////////////1805
#define UART_SERVICE_TX                         "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"      // VALUE: batVal Int, Prop: Write
#define UART_SERVICE_RX                         "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"      // VALUE: batVal Int, Prop: Read | Notify

 /*
        UUID_DATE_TIME_CHAR                     = 0x2A08,
        UUID_CURRENT_TIME_CHAR                  = 0x2A2B,
*/

#define BLE_ADV_DIRECTED_ENABLED                true
#define ADVERTISING_INTERVAL                    100                                 /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */

#define APP_ADV_TIMEOUT_IN_SECONDS              30                                   /**< The advertising timeout (in units of seconds). */


#define MIN_CONN_INTERVAL                       (double)7.5                         /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL                       60                                  /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                           0                                   /**< slave latency. */
#define CONN_SUP_TIMEOUT                        40000                               /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */


#define TX_POWER                                0

#define DEVICE_NAME                             "BLE-HANDSET"                       /**< Name of device. Will be included in the advertising data. */
#define LOCALNAME                               DEVICE_NAME

#define FIRMWARE                                "1.0.0"
#define REVISION                                "1.0"


#endif