// This file holds a simulated in-memory/flash implementation of the EEPROM library.
// It keeps the exact same interface, but entirely ignores any write operations.

#include <Arduino.h>

#define EEPROM_USED_SIZE 125 
#define FLOAT_ARRAY_START 5
#define NAME_ARRAY_START 86

// ============================================================================
// SIMULATED EEPROM CLASS
// ============================================================================
// ============================================================================
// SIMULATED IN-MEMORY EEPROM CLASS (Allows Reads and Writes in RAM)
// ============================================================================
class FakeEEPROM {
private:
    // RAM-based buffer initialized with your default layout
    uint8_t mockMemory[EEPROM_USED_SIZE];

public:
    // Constructor to set up default values on boot
    FakeEEPROM() {
        // Clear everything to 0x00 initially
        memset(mockMemory, 0x00, EEPROM_USED_SIZE);
        
        // Seed the critical defaults so validity checks pass
        mockMemory[0] = 0xAA; // EEPROM[0]: Verification byte (Always Valid)
        mockMemory[1] = 0x80; // EEPROM[1]: Byte A (e.g., A_EEPROM_ENABLE active)
    }

    // Exactly mimics EEPROM.read()
    uint8_t read(int address) {
        if (address >= 0 && address < EEPROM_USED_SIZE) {
            return mockMemory[address];
        }
        return 0xFF; 
    }

    // Now actively updates the RAM buffer
    void write(int address, uint8_t value) {
        if (address >= 0 && address < EEPROM_USED_SIZE) {
            mockMemory[address] = value;
        }
    }

    // Exactly mimics EEPROM.get()
    template <typename T>
    T& get(int address, T& t) {
        if (address >= 0 && address + sizeof(T) <= EEPROM_USED_SIZE) {
            memcpy(&t, &mockMemory[address], sizeof(T));
        }
        return t;
    }

    // Now actively copies complex types into the RAM buffer
    template <typename T>
    const T& put(int address, const T& t) {
        if (address >= 0 && address + sizeof(T) <= EEPROM_USED_SIZE) {
            memcpy(&mockMemory[address], &t, sizeof(T));
        }
        return t;
    }
};

// Instantiate the globally accessible object named exactly 'EEPROM'
FakeEEPROM EEPROM;

// ============================================================================
// ORIGINAL LOGIC (UNCHANGED INTERFACE)
// ============================================================================

#define CHECKSUM_BYTE 0      

#define VERIFICATION_BYTE 0
#define CAL_A 1
#define CAL_B 2
#define CAL_C 3
#define CAL_D 4

// Calibration Byte A Locations
#define A_EEPROM_ENABLE 0x80
#define A_THROTTLE_MODE 0x40 
#define A_TEMP_SENSOR_MODE 0x20
#define A_PWM_ENABLE 0x10
#define A_THROTTLE_RAMP 0x08

// Calibration Byte D Locations
#define D_RPM_NEW 0x80
#define D_SPEED_NEW 0x40

// Float Locations
#define INDEX_TRANSMIT_INTERVAL 0
#define INDEX_WHEEL_MAGNETS 1
#define INDEX_MOTOR_MAGNETS 2
#define INDEX_REF_VOLTAGE 3
#define INDEX_24_VOLTAGE 4
#define INDEX_12_VOLTAGE 5
#define INDEX_CURRENT 6
#define INDEX_TEMP_1_A 7
#define INDEX_TEMP_1_B 8
#define INDEX_TEMP_1_C 9
#define INDEX_TEMP_2_A 10
#define INDEX_TEMP_2_B 11
#define INDEX_TEMP_2_C 12
#define INDEX_THROTTLE_LOW 13
#define INDEX_THROTTLE_HIGH 14
#define INDEX_WHEEL_CIRCUMFERENCE 15
#define INDEX_INTERNAL_REFERENCE_VOLTAGE 16

void EEPROMSetup()
{
    saveCurrCalToEeprom();
}

void saveCurrCalToEeprom()
{
    if (!getVerificationByte())
    {
        setVerificationByte();
    }

    setBinaryCal(CAL_A, A_EEPROM_ENABLE, CAL_USE_EEPROM);
    setBinaryCal(CAL_A, A_THROTTLE_MODE, CAL_THROTTLE_VARIABLE);
    setBinaryCal(CAL_A, A_THROTTLE_RAMP, CAL_THROTTLE_RAMP);
    setBinaryCal(CAL_A, A_PWM_ENABLE, CAL_THROTTLE_OUTPUT_EN);
    setBinaryCal(CAL_A, A_TEMP_SENSOR_MODE, CAL_LINEAR_TEMPERATURE);

    setBinaryCal(CAL_D, D_RPM_NEW, CAL_USE_IMPROVED_RPM_CALCULATION);
    setBinaryCal(CAL_D, D_SPEED_NEW, CAL_USE_IMPROVED_SPEED_CALCULATION);

    setFloatCal(INDEX_TRANSMIT_INTERVAL, (float)CAL_DATA_TRANSMIT_INTERVAL);
    setFloatCal(INDEX_WHEEL_MAGNETS, (float)CAL_WHEEL_MAGNETS);
    setFloatCal(INDEX_MOTOR_MAGNETS, (float)CAL_MOTOR_MAGNETS);
    setFloatCal(INDEX_REF_VOLTAGE, (float)CAL_REFERENCE_VOLTAGE);
    setFloatCal(INDEX_24_VOLTAGE, (float)CAL_BATTERY_TOTAL);
    setFloatCal(INDEX_12_VOLTAGE, (float)CAL_BATTERY_LOWER);
    setFloatCal(INDEX_CURRENT, (float)CAL_CURRENT);
    setFloatCal(INDEX_TEMP_1_A, (float)CAL_THERM1_A);
    setFloatCal(INDEX_TEMP_1_B, (float)CAL_THERM1_B);
    setFloatCal(INDEX_TEMP_1_C, (float)CAL_THERM1_C);
    setFloatCal(INDEX_TEMP_2_A, (float)CAL_THERM2_A);
    setFloatCal(INDEX_TEMP_2_B, (float)CAL_THERM2_B);
    setFloatCal(INDEX_TEMP_2_C, (float)CAL_THERM2_C);
    setFloatCal(INDEX_THROTTLE_LOW, (float)CAL_THROTTLE_LOW);
    setFloatCal(INDEX_THROTTLE_HIGH, (float)CAL_THROTTLE_HIGH);
    setFloatCal(INDEX_WHEEL_CIRCUMFERENCE, (float)CAL_WHEEL_CIRCUMFERENCE);
    setFloatCal(INDEX_INTERNAL_REFERENCE_VOLTAGE, (float)CAL_INTERNAL_REFERENCE_VOLTAGE);

    writeBTName();
    writeEEPROMChecksum();
}

void loadEepromCalibration()
{
    CAL_USE_EEPROM = readBinaryCal(CAL_A, A_EEPROM_ENABLE);
    CAL_THROTTLE_VARIABLE = readBinaryCal(CAL_A, A_THROTTLE_MODE);
    CAL_THROTTLE_OUTPUT_EN = readBinaryCal(CAL_A, A_PWM_ENABLE);
    CAL_THROTTLE_RAMP = readBinaryCal(CAL_A, A_THROTTLE_RAMP);
    CAL_USE_IMPROVED_RPM_CALCULATION = readBinaryCal(CAL_D, D_RPM_NEW);
    CAL_USE_IMPROVED_SPEED_CALCULATION = readBinaryCal(CAL_D, D_SPEED_NEW); 

    CAL_DATA_TRANSMIT_INTERVAL = (unsigned long)getFloatCal(INDEX_TRANSMIT_INTERVAL);
    CAL_WHEEL_MAGNETS = (int)getFloatCal(INDEX_WHEEL_MAGNETS);
    CAL_MOTOR_MAGNETS = (int)getFloatCal(INDEX_MOTOR_MAGNETS);
    CAL_WHEEL_CIRCUMFERENCE = getFloatCal(INDEX_WHEEL_CIRCUMFERENCE);
    CAL_REFERENCE_VOLTAGE = getFloatCal(INDEX_REF_VOLTAGE);
    CAL_INTERNAL_REFERENCE_VOLTAGE = getFloatCal(INDEX_INTERNAL_REFERENCE_VOLTAGE);
    CAL_BATTERY_TOTAL = getFloatCal(INDEX_24_VOLTAGE);
    CAL_BATTERY_LOWER = getFloatCal(INDEX_12_VOLTAGE);
    CAL_CURRENT = getFloatCal(INDEX_CURRENT);
    CAL_THERM1_A = getFloatCal(INDEX_TEMP_1_A);
    CAL_THERM1_B = getFloatCal(INDEX_TEMP_1_B);
    CAL_THERM1_C = getFloatCal(INDEX_TEMP_1_C);
    CAL_THERM2_A = getFloatCal(INDEX_TEMP_2_A);
    CAL_THERM2_B = getFloatCal(INDEX_TEMP_2_B);
    CAL_THERM2_C = getFloatCal(INDEX_TEMP_2_C);
    CAL_THROTTLE_LOW = (int)getFloatCal(INDEX_THROTTLE_LOW);
    CAL_THROTTLE_HIGH = (int)getFloatCal(INDEX_THROTTLE_HIGH);

    if(CAL_INTERNAL_REFERENCE_VOLTAGE == 0 || CAL_INTERNAL_REFERENCE_VOLTAGE == 0xFF ){
      CAL_INTERNAL_REFERENCE_VOLTAGE = 1.1;
      setFloatCal(INDEX_INTERNAL_REFERENCE_VOLTAGE, (float)CAL_INTERNAL_REFERENCE_VOLTAGE);
    }

    getBTName();
}

uint8_t readBinaryCal(char byte, char bit)
{
    char temp = EEPROM.read(byte);
    temp = temp & bit;   
    return temp ? 1 : 0; 
}

byte getBinaryCalByte(char byte)
{
    char temp = EEPROM.read(byte);
    return temp; 
}

void setBinaryCal(char byte, char bit)
{
    char temp = EEPROM.read(byte);
    temp = temp | bit; 
    EEPROM.write(byte, temp);
}

void setBinaryCal(char byte, char bit, uint8_t value)
{
    char temp = EEPROM.read(byte);
    if (value)
    {
        temp = temp | bit; 
    }
    else
    {
        temp = temp & ~bit; 
    }
    EEPROM.write(byte, temp);
}

void clearBinaryCal(char byte, char bit)
{
    char temp = EEPROM.read(byte);
    temp = temp & ~bit; 
    EEPROM.write(byte, temp);
}

void setFloatCal(uint8_t index, float value)
{
    uint8_t address = FLOAT_ARRAY_START + (index * 4);
    EEPROM.put(address, value);
}

float getFloatCal(uint8_t index)
{
    float temp = 0;                                  
    uint8_t address = index * 4 + FLOAT_ARRAY_START; 
    EEPROM.get(address, temp);
    return temp;
}

byte getFloatByte(uint8_t index)
{
    byte temp = 0;                               
    uint8_t address = index + FLOAT_ARRAY_START; 
    EEPROM.get(address, temp);
    return temp;
}

byte getNameByte(uint8_t index)
{
    byte temp = 0;                              
    uint8_t address = index + NAME_ARRAY_START; 
    EEPROM.get(address, temp);
    return temp;
}

uint8_t getVerificationByte()
{
    byte temp = EEPROM.read(0);
    return temp == 0xAA;
}

void setVerificationByte()
{
    EEPROM.write(0, 0xAA);
}

void clearVerificationByte()
{
    EEPROM.write(0, 0xFF);
}

void writeBTName()
{
    char buff[30] = {0xff};
    CAL_BT_NAME.toCharArray(buff, 30);
    EEPROM.put(NAME_ARRAY_START, buff);
}

void getBTName()
{
    String temp = "";
    for (uint8_t i = 0; i < 30; i++)
    {
        char tmpChar = getNameByte(i);
        if (tmpChar != 0xff)
            temp += tmpChar;
    }
    CAL_BT_NAME = temp;
}

uint8_t calculateEEPROMChecksum() {
    uint8_t checksum = 0;
    for (uint8_t i = 1; i < EEPROM_USED_SIZE; i++) { 
        checksum += EEPROM.read(i);
    }
    return checksum;
}

void writeEEPROMChecksum() {
    uint8_t checksum = calculateEEPROMChecksum();
    EEPROM.write(CHECKSUM_BYTE, checksum);
}

bool verifyEEPROMChecksum() {
    uint8_t stored = EEPROM.read(CHECKSUM_BYTE);
    uint8_t calculated = calculateEEPROMChecksum();
    return stored == calculated;
}