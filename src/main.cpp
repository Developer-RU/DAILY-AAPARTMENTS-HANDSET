#include "main.hpp"


#ifdef __cplusplus

extern "C" {
    #endif
        void SWI2_IRQHandler(void);
    #ifdef __cplusplus
}

#endif


BLEPeripheral blePeripheral = BLEPeripheral();

//// Gap service ////
BLEService GenericAccess_PS_H = BLEService(GENERIC_ACCESS_SERVICE_UUID);
BLECharCharacteristic GenericAccess_DeviceName_DP_H = BLECharCharacteristic(GENERIC_ACCESS_DEVICE_NAME, BLERead | BLENotify);
BLECharCharacteristic CONN_PARAM_DP_H = BLECharCharacteristic(GENERIC_ACCESS_CONN_PARAM, BLERead | BLENotify);

//// Gatt ////
BLEService GenericAttribute_PS_H = BLEService(GENERIC_ATTR_SERVICE_UUID);
BLECharCharacteristic GenericAccess_Appearance_DP_H = BLECharCharacteristic(GENERIC_ACCESS_APPERANCE, BLERead | BLENotify);

//// Device information service ////
BLEService DeviceInformation_PS_H = BLEService(DEVICE_INFO_SERVICE_UUID);
BLEStringCharacteristic DeviceInformation_SerialN_DP_H = BLEStringCharacteristic(DEVICE_INFO_SERIAL_N, BLERead, sizeof((String)DEVICE_INFO_SERIAL_N));
BLEStringCharacteristic DeviceInformation_FirmRev_DP_H = BLEStringCharacteristic(DEVICE_INFO_FIRM_REV, BLERead, sizeof((String)DEVICE_INFO_FIRM_REV));
BLEStringCharacteristic DeviceInformation_HardRev_DP_H = BLEStringCharacteristic(DEVICE_INFO_HARD_REV, BLERead, sizeof((String)DEVICE_INFO_HARD_REV));

//// UART service ////
BLEService UART_SERVICE_H = BLEService(UART_SERVICE_UUID);
BLEStringCharacteristic UART_SERVICE_RX_H = BLEStringCharacteristic(UART_SERVICE_RX, BLEWrite, sizeof((String)UART_SERVICE_RX));
BLEStringCharacteristic UART_SERVICE_TX_H = BLEStringCharacteristic(UART_SERVICE_TX, BLERead | BLENotify, sizeof((String)UART_SERVICE_TX));
BLEDescriptor UART_SERVICE_DESCRIPTOR = BLEDescriptor("2902", "BLE UART");

String cpuID = "";

unsigned long time_connect = 0;
bool new_command = 0;       // 0 - false, 1 - true
bool command = 0;           // 1 - open
bool new_start = 0;
unsigned long timeUp = 0;


/**
 * @brief 
 * 
 */
void SWI2_IRQHandler(void) 
{
    // NOOP
    // sd_power_system_off();
}

/**
 * @brief 
 * 
 * @param new_time data
 */
void updateAdvertisingScanData(unsigned long new_time)
{
    unsigned char srData[32];
    unsigned char srDataLen = 0;
    
    int scanDataSize = 3;

    BLEEirData scanData[scanDataSize];

    // - Local name
    scanData[0].length = strlen(DEVICE_NAME);
    scanData[0].type = 0x09;
    memcpy(scanData[0].data, DEVICE_NAME, scanData[0].length);

    // - Tx Power
    scanData[1].length = 1;
    scanData[1].type = 0x0A;
    scanData[1].data[0] = TX_POWER;

    // - Manufacturer Data
    scanData[2].length = 4;
    scanData[2].type = 0xFF;
    
    // Manufacturer ID
    scanData[2].data[0] = 0xFF;
    scanData[2].data[1] = 0xFF;

    // Manufacturer data content
    scanData[2].data[2] = new_time & 0xFF;
    scanData[2].data[3] = (new_time >> 8) & 0xFF;
    scanData[2].data[6] = (new_time >> 16) & 0xFF;
    scanData[2].data[7] = (new_time >> 24) & 0xFF;

    if (scanDataSize && scanData)
    {
        for (int i = 0; i < scanDataSize; i++)
        {
            srData[srDataLen + 0] = scanData[i].length + 1;
            srData[srDataLen + 1] = scanData[i].type;
            srDataLen += 2;

            memcpy(&srData[srDataLen], scanData[i].data, scanData[i].length);

            srDataLen += scanData[i].length;
        }
    }

    // - Sets only avertising scan data
    sd_ble_gap_adv_data_set(srData, srDataLen, srData, srDataLen);
}

/**
 * @brief 
 * 
 * @param central 
 * @param characteristic 
 */
void characteristicWrittenCallback(BLECentral& central, BLECharacteristic& characteristic) 
{
    if (UART_SERVICE_RX_H.value()) 
    {
        String message =  UART_SERVICE_RX_H.value();

        // Serial.println("*********");
        // Serial.print("Received Value: ");

        // Serial.print(message);

        // Serial.println();
        // Serial.println("*********");

        if(message.indexOf("open") > -1) { command = 1; new_command = 1; }

        /* code other commands */
    }
}

/**
 * @brief 
 * 
 * @param central 
 */
void blePeripheralConnectHandler(BLECentral& central) 
{
    /////////////////////(F("Connected event, central: "));
    /////////////////////(central.address());
}

/**
 * @brief 
 * 
 * @param central 
 */
void blePeripheralDisconnectHandler(BLECentral& central) 
{
    /////////////////////(F("Disconnected event, central: "));
    /////////////////////(central.address());
}

/**
 * @brief Get the cpuID object
 * 
 */
void get_cpuID()
{
    // cpuID += NRF_FICR->DEVICEADDR[1];	
    cpuID += NRF_FICR->DEVICEADDR[0];	
}

/**
 * @brief 
 * 
 */
void setup()
{
    // Serial.begin(115200);
    
    pinMode(PIN_BUTTON1, INPUT);

    pinMode(PIN_LED1, OUTPUT); digitalWrite(PIN_LED1, LOW);
    pinMode(PIN_LED2, OUTPUT); digitalWrite(PIN_LED2, LOW);
    pinMode(PIN_LED3, OUTPUT); digitalWrite(PIN_LED2, LOW);

    get_cpuID();

    blePeripheral.setLocalName(LOCALNAME);
    blePeripheral.setDeviceName(DEVICE_NAME);

    //// Device information service ////
    blePeripheral.setAdvertisedServiceUuid(DeviceInformation_PS_H.uuid());
    blePeripheral.addAttribute(DeviceInformation_PS_H);
    blePeripheral.addAttribute(DeviceInformation_SerialN_DP_H); 
    blePeripheral.addAttribute(DeviceInformation_FirmRev_DP_H); 
    blePeripheral.addAttribute(DeviceInformation_HardRev_DP_H); 

    //// UART service ////
    blePeripheral.setAdvertisedServiceUuid(UART_SERVICE_H.uuid());
    blePeripheral.addAttribute(UART_SERVICE_H);
    blePeripheral.addAttribute(UART_SERVICE_TX_H);    
    blePeripheral.addAttribute(UART_SERVICE_RX_H);
    blePeripheral.addAttribute(UART_SERVICE_DESCRIPTOR);   

    blePeripheral.setAdvertisingInterval(ADVERTISING_INTERVAL);    
    blePeripheral.setTxPower(TX_POWER);

    blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
    blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

    UART_SERVICE_RX_H.setEventHandler(BLEWritten, characteristicWrittenCallback);

    blePeripheral.begin();


    DeviceInformation_SerialN_DP_H.writeValue((String)NRF_FICR->DEVICEADDR[0]);
    DeviceInformation_FirmRev_DP_H.writeValue((String)FIRMWARE);
    DeviceInformation_HardRev_DP_H.writeValue((String)REVISION);

    updateAdvertisingScanData(0);

    /////////////////////(cpuID);	
    /////////////////////("=============");
    /////////////////////("NRF_POWER_MODE_LOWPWR");	
    /////////////////////("=============");

    NRF_GPIO->PIN_CNF[PIN_BUTTON1] |= ((uint32_t)GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos);
   
    sd_power_mode_set(SD_POWER_SYSTEM_OFF);
}

/**
 * @brief 
 * 
 */
void loop()
{   
    if(new_start == 0) 
    {
        new_start = 1; 
        timeUp = millis(); 
    }

    if(millis() > timeUp + 15000)
    {
        digitalWrite(PIN_LED1, LOW);

        sd_ble_gap_adv_stop();
        new_start = 0; 
        sd_power_system_off();
    }


    delay(200);
    
    digitalWrite(PIN_LED1, HIGH);

    blePeripheral.poll();
    BLECentral central = blePeripheral.central();

    if (central)
    {        
        time_connect = millis();

        while (central.connected())
        {
            if(new_command == 1)
            {
                new_command = 0;

                if(command == 1)
                {
                    digitalWrite(PIN_LED2, HIGH);
                    delay(2500); 


                    digitalWrite(PIN_LED3, HIGH);
                    delay(1500); 
                    digitalWrite(PIN_LED3, LOW);
                    delay(500); 
                    
                    digitalWrite(PIN_LED3, HIGH);
                    delay(1500); 
                    digitalWrite(PIN_LED3, LOW);
                    delay(500); 

                    digitalWrite(PIN_LED3, HIGH);
                    delay(1500); 
                    digitalWrite(PIN_LED3, LOW);
                    delay(500); 

                    
                    digitalWrite(PIN_LED2, HIGH);
                    delay(500); 


                    digitalWrite(PIN_LED2, LOW);


                    timeUp = millis() + 30000;

                    time_connect = millis() + APP_ADV_TIMEOUT_IN_SECONDS * 1000;
                }

                /* other commands */
            }

            if(millis() > time_connect + APP_ADV_TIMEOUT_IN_SECONDS * 1000) { central.disconnect(); blePeripheral.disconnect(); }

            delay(10); 
            sd_nvic_ClearPendingIRQ(SWI2_IRQn);
        }
    }
}
