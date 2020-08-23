/*
 *Author : Vishnu Saradhara 
 *Date   : 08/08/2020
 */
#include "BLEBeacon.h"
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEScan.h"
#include "BLEAdvertisedDevice.h"

int LED_BUILTIN = 2;

#define ENDIAN_CHANGE_U16(x)((((x) &0xFF00) >> 8) + (((x) &0xFF) << 8))
#define BEACON_UUID "8ec76ea3-6668-48da-9866-2462ABDD4230"  // UUID 1 128-Bit (may use linux tool uuidgen or random numbers via https://www.uuidgenerator.net/)
#define TX_POWER - 57 // measured RSSI at 1m distance
#define THRESHOLD - 64  // cut off
int scanTime = 1; //In seconds
BLEScan * pBLEScan;
BLEAdvertising * pAdvertising;

uint16_t str_to_uint16(const char *str) {
    char *end;
    uint16_t res;
    errno = 0;
    long val = strtol(str, &end, 10);
    if (errno || end == str || *end != '\0' || val < 0 || val >= 0x10000) {
        return false;
    }
    res = (uint16_t)val;
    return res;
}
/*iBeacon Transmitter called from setup*/
void setBeacon()
{
  BLEBeacon oBeacon = BLEBeacon();
  oBeacon.setManufacturerId(0x4C00);  // fake Apple 0x004C LSB (ENDIAN_CHANGE_U16!)
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  String uuid = BEACON_UUID + mac;
  uint16_t uuid_uint = uuid_to_uint_16();
  oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
  oBeacon.setMajor(0x8153);
  oBeacon.setMinor(0x0);
  oBeacon.setSignalPower(TX_POWER);
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();
  oAdvertisementData.setFlags(0x04);  // BR_EDR_NOT_SUPPORTED 0x04
  std::string strServiceData = "";
  strServiceData += (char) 26;  // Len
  strServiceData += (char) 0xFF;  // Type
  strServiceData += oBeacon.getData();
  oAdvertisementData.addData(strServiceData);
  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);
}

/*iBeacon Scanner funcation called by startBLEScan Task*/
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
  /*keep the status of sending data */
  BaseType_t xStatus;
  /*time to block the task until the queue has free space */
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  /*create data to send */
  Trace data;
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    std::string strServiceData = advertisedDevice.getServiceData();
    uint8_t cServiceData[100];
    strServiceData.copy((char*) cServiceData, strServiceData.length(), 0);
    if (advertisedDevice.haveManufacturerData() == true)
    {
      std::string strManufacturerData = advertisedDevice.getManufacturerData();

      uint8_t cManufacturerData[100];
      strManufacturerData.copy((char*) cManufacturerData, strManufacturerData.length(), 0);

      if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00)
      {
        BLEBeacon oBeacon = BLEBeacon();
        oBeacon.setData(strManufacturerData);
        Serial.printf("iBeacon Frame =>");
        Serial.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d RSSI: %d\n", oBeacon.getManufacturerId(), ENDIAN_CHANGE_U16(oBeacon.getMajor()), ENDIAN_CHANGE_U16(oBeacon.getMinor()), oBeacon.getProximityUUID().toString().c_str(), oBeacon.getSignalPower(), advertisedDevice.getRSSI());
        int rssi = advertisedDevice.getRSSI();
        int tx_power = oBeacon.getSignalPower();
        if (rssi >= THRESHOLD)
        {
          float distance = calculate_distance(tx_power, rssi);
          String uuid = oBeacon.getProximityUUID().toString().c_str();
          String covid_risk = "";
          if (distance <= 1)
          {
            covid_risk = "HIGH RISK";
            //buzzer code place here
            digitalWrite(LED_BUILTIN, HIGH);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            digitalWrite(LED_BUILTIN, LOW);
          }
          else if (distance <= 2)
          {
            covid_risk = "MEDIUM RISK";
            //buzzer code place here
            digitalWrite(LED_BUILTIN, HIGH);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            digitalWrite(LED_BUILTIN, LOW);
          }
          else
          {
            covid_risk = "LOW RISK";
            digitalWrite(LED_BUILTIN, HIGH);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            digitalWrite(LED_BUILTIN, LOW);
          }

          data.distance = distance;
          data.uuid = uuid;
          data.risk = covid_risk;
          Serial.printf("distance violated by: %s, d=%fm \n", oBeacon.getProximityUUID().toString().c_str(), distance);
          xStatus = xQueueSendToFront(xQueue, &data, xTicksToWait);
          if (xStatus != pdPASS)
          {
            Serial.printf("Queue is full !!");
          }
        }
      }
    }
  }
};
/*iBeacon Scanner Task*/
void startBLEScan(void *parameter)
{
  for (;;)
  {
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  }

  vTaskDelete(NULL);
};
