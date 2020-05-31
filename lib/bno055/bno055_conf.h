#ifndef _BNO055_CONF_H_
#define _BNO055_CONF_H_

// Device I2C address
#define bno055_address  (uint8_t)0x29

// Chip ID reg defines
#define REG_CHIP_ID     0x00
#define LEN_CHIP_ID     0x01


// Operation Mode Defines
#define REG_OPR_MODE    0x3D
#define LEN_OPR_MODE    0x01    
// Operation Modes
#define CONFIGMODE      0b00000000
// Non-fusionmodesb0
#define ACCONLY         0b00000001  
#define MAGONLY         0b00000010
#define GYROONLY        0b00000011
#define ACCMAG          0b00000100
#define ACCGYRO         0b00000101
#define MAGGYRO         0b00000110
#define AMG             0b00000111
// Fusion modesb0
#define IMU             0b00001000
#define COMPASS         0b00001001
#define M4G             0b00001010
#define NDOF_FMC_OFF    0b00001011
#define NDOF            0b00001100

#define OPR_MODE_DELAY  20 // in milliseconds

// Data registers defines
#define REG_TEMP            0x34 //Temperature

#define REG_GRV_Data_Z_MSB  0x33 //Gravity Vector Data Z <15:8>
#define REG_GRV_Data_Z_LSB  0x32 //Gravity Vector Data Z <7:0>
#define REG_GRV_Data_Y_MSB  0x31 //Gravity Vector Data Y <15:8>
#define REG_GRV_Data_Y_LSB  0x30 //Gravity Vector Data Y <7:0>
#define REG_GRV_Data_X_MSB  0x2F //Gravity Vector Data X <15:8>
#define REG_GRV_Data_X_LSB  0x2E //Gravity Vector Data X <7:0>

#define REG_LIA_Data_Z_MBS  0x2D //Linear Acceleration Data Z <15:8>
#define REG_LIA_Data_Z_LSB  0x2C //Linear Acceleration Data Z <7:0>
#define REG_LIA_Data_Y_MBS  0x2B //Linear Acceleration Data Y <15:8>
#define REG_LIA_Data_Y_LSB  0x2A //Linear Acceleration Data Y <7:0>
#define REG_LIA_Data_X_MBS  0x29 //Linear Acceleration Data X <15:8>
#define REG_LIA_Data_X_LSB  0x28 //Linear Acceleration Data X <7:0>

#define REG_QUA_Data_z_MSB  0x27 //Quaternion z Data <15:8>
#define REG_QUA_Data_z_LSB  0x26 //Quaternion z Data <7:0>
#define REG_QUA_Data_y_MSB  0x25 //Quaternion y Data <15:8>
#define REG_QUA_Data_y_LSB  0x24 //Quaternion y Data <7:0>
#define REG_QUA_Data_x_MSB  0x23 //Quaternion x Data <15:8>
#define REG_QUA_Data_x_LSB  0x22 //Quaternion x Data <7:0>
#define REG_QUA_Data_w_MSB  0x21 //Quaternion w Data <15:8>
#define REG_QUA_Data_w_LSB  0x20 //Quaternion w Data <7:0>

#define REG_EUL_Pitch_MSB   0x1F //Pitch Data <15:8>
#define REG_EUL_Pitch_LSB   0x1E //Pitch Data <7:0>

#define REG_EUL_Roll_MSB    0x1D //0x00 Roll Data <15:8>
#define REG_EUL_Roll_LSB    0x1C //0x00 Roll Data <7:0>

#define REG_EUL_Heading_MSB 0x1B //0x00 Heading Data <15:8>
#define REG_EUL_Heading_LSB 0x1A //0x00 Heading Data <7:0>

#define REG_GYR_DATA_Z_MSB  0x19 //0x00 Gyroscope Data Z <15:8>
#define REG_GYR_DATA_Z_LSB  0x18 //0x00 Gyroscope Data Z <7:0>
#define REG_GYR_DATA_Y_MSB  0x17 //0x00 Gyroscope Data Y <15:8>
#define REG_GYR_DATA_Y_LSB  0x16 //0x00 Gyroscope Data Y <7:0>
#define REG_GYR_DATA_X_MSB  0x15 //0x00 Gyroscope Data X <15:8>
#define REG_GYR_DATA_X_LSB  0x14 //0x00 Gyroscope Data X <7:0>

#define REG_MAG_DATA_Z_MSB  0x13 //0x00 Magnetometer Data Z <15:8>
#define REG_MAG_DATA_Z_LSB  0x12 //0x00 Magnetometer Data Z <7:0>
#define REG_MAG_DATA_Y_MSB  0x11 //0x00 Magnetometer Data Y <15:8>
#define REG_MAG_DATA_Y_LSB  0x10 //0x00 Magnetometer Data Y <7:0>
#define REG_MAG_DATA_X_MSB  0x0F //0x00 Magnetometer Data X <15:8>
#define REG_MAG_DATA_X_LSB  0x0E //0x00 Magnetometer Data X <7:0>

#define REG_ACC_DATA_Z_MSB  0x0D //0x00 Acceleration Data Z <15:8>
#define REG_ACC_DATA_Z_LSB  0x0C //0x00 Acceleration Data Z <7:0>
#define REG_ACC_DATA_Y_MSB  0x0B //0x00 Acceleration Data Y <15:8>
#define REG_ACC_DATA_Y_LSB  0x0A //0x00 Acceleration Data Y <7:0>
#define REG_ACC_DATA_X_MSB  0x09 //0x00 Acceleration Data X <15:8>
#define REG_ACC_DATA_X_LSB  0x08 //0x00 Acceleration Data X <7:0>

#define LEN_DATA            0x01


#endif
