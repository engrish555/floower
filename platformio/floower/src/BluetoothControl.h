#pragma once

#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "Config.h"
#include "hardware/Floower.h"

typedef std::function<void()> RemoteTakeOverCallback;

class BluetoothControl {
    public:
        BluetoothControl(Floower *floower, Config *config);
        void init();
        void startAdvertising();
        void stopAdvertising();
        void setBatteryLevel(uint8_t level, bool charging);
        bool isConnected();
        void onTakeOver(RemoteTakeOverCallback callback);

    private:
        Floower *floower;
        Config *config;
        RemoteTakeOverCallback takeOverCallback;
        BLEServer *server = nullptr;
        BLEService *floowerService = nullptr;
        BLEService *batteryService = nullptr;

        bool deviceConnected = false;
        bool advertising = false;
        bool initialized = false;

        BLECharacteristic* createROCharacteristics(BLEService *service, const char *uuid, const char *value);

        // BLE state characteristics callback
        class StateChangeCharacteristicsCallbacks : public BLECharacteristicCallbacks {
            public:
                StateChangeCharacteristicsCallbacks(BluetoothControl* bluetoothControl) : bluetoothControl(bluetoothControl) {};
            private:
                BluetoothControl* bluetoothControl ;
                void onWrite(BLECharacteristic *characteristic);
        };

        // BLE name characteristics callback
        class NameCharacteristicsCallbacks : public BLECharacteristicCallbacks {
            public:
                NameCharacteristicsCallbacks(BluetoothControl* bluetoothControl) : bluetoothControl(bluetoothControl) {};
            private:
                BluetoothControl* bluetoothControl ;
                void onWrite(BLECharacteristic *characteristic);
        };

        // BLE color scheme characteristics callback
        class ColorsSchemeCharacteristicsCallbacks : public BLECharacteristicCallbacks {
            public:
                ColorsSchemeCharacteristicsCallbacks(BluetoothControl* bluetoothControl) : bluetoothControl(bluetoothControl) {};
            private:
                BluetoothControl* bluetoothControl ;
                void onWrite(BLECharacteristic *characteristic);
        };

        // BLE touch threshold characteristics callback
        class PersonificationCharacteristicsCallbacks : public BLECharacteristicCallbacks {
            public:
                PersonificationCharacteristicsCallbacks(BluetoothControl* bluetoothControl) : bluetoothControl(bluetoothControl) {};
            private:
                BluetoothControl* bluetoothControl ;
                void onWrite(BLECharacteristic *characteristic);
        };

        // BLE server callbacks impl
        class ServerCallbacks : public BLEServerCallbacks {
            public:
                ServerCallbacks(BluetoothControl* bluetoothControl) : bluetoothControl(bluetoothControl) {};
            private:
                BluetoothControl* bluetoothControl ;
                void onConnect(BLEServer* server);
                void onDisconnect(BLEServer* server);
        };
};