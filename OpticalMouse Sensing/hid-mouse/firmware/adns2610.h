/* Function Definitions */

//Function Description: Initializes the I/O for the ADNS2610. Assumes instances of sck and sdio have been defined.
//Inputs: None
//Outputs: None
//Return:  None. 
//usage: setupAdns();
void setupAdns(void);

void sync(void);

 //Function Description: Writes a value to a specified address on the ADNS2619 
//Inputs: char address - ADNS2610 Register Address to write to
//        char value - The value to write to the ADNS2610 register
//Outputs: None
//Return:  None
//usage: write(CONFIGURATION_REGISTER, config_setting);
 void write(char address, char value);
 
 //Function Description: Reads a byte at specified address from the ADNS2610    
//Inputs: char address - ADNS2610 Register Address to write to
//Outputs: None
//Return:  The byt read from the ADNS2610
//usage: value = read(CONFIGURATION_REG);
char read(char address);


/* Pin Definitions for the ADNS2610 Evaluation Board */
#define ADNS_PORT	PORTC
#define ADNS_DIR	DDRC
#define ADNS_PIN	PINC
#define ADNS_SCL	0
#define ADNS_SDIO	1

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))


/* Register Map for the ADNS2610 Optical Mouse Sensor */
#define CONFIGURATION_REG   0x40
#define STATUS_REG          0x41
#define DELTA_Y_REG         0x42
#define DELTA_X_REG         0x43
#define SQUAL_REG           0x44
#define MAXIMUM_PIXEL_REG   0x45
#define MINIMUM_PIXEL_REG   0x46
#define PIXEL_SUM_REG       0x47
#define PIXEL_DATA_REG      0x48
#define SHUTTER_UPPER_REG   0x49
#define SHUTTER_LOWER_REG   0x4A
#define FRAME_PERIOD 0x4B