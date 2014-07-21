/**
 * Load an image in from Optical Mouse
 * Display as grayscale image
 */


import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;      // Data received from the serial port

PImage myImage; 

int linefeed = 10;   // Linefeed in ASCII
int numSensors = 18*18;  // we will be expecting for reading data from four sensors
int sensors[];       // array to read the 4 values
int pSensors[];      // array to store the previuos reading, usefur for comparing
// actual reading with the last one


void setup() 
{
  size(800, 800);
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200);
  myImage = createImage(18,18,ALPHA); //Alpha means grayscale
myImage = loadImage("testmouseImage.png");
  background(255,0,0);
    image (myImage, 0, 0, width, height);
  
}

void draw()
{
  
  /*
  if ( myPort.available() > 0) {  // If data is available,
    val = myPort.read();         // read it and store it in val
  }
  background(255);             // Set background to white
  if (val == 0) {              // If the serial value is 0,
    fill(0);                   // set fill to black
  } 
  else {                       // If the serial value is not 0,
    fill(204);                 // set fill to light gray
  }
  rect(50, 50, 100, 100);
  
  */
}



/*

// Wiring / Arduino Code
// Code for sensing a switch status and writing the value to the serial port.

int switchPin = 4;                       // Switch connected to pin 4

void setup() {
  pinMode(switchPin, INPUT);             // Set pin 0 as an input
  Serial.begin(9600);                    // Start serial communication at 9600 bps
}

void loop() {
  if (digitalRead(switchPin) == HIGH) {  // If switch is ON,
    Serial.print(1, BYTE);               // send 1 to Processing
  } else {                               // If the switch is not ON,
    Serial.print(0, BYTE);               // send 0 to Processing
  }
  delay(100);                            // Wait 100 milliseconds
}

*/

void serialEvent(Serial myPort) {

  // read the serial buffer:
  String myString = myPort.readStringUntil(linefeed);

  // if you got any bytes other than the linefeed:
  if (myString != null) {

    myString = trim(myString);

    // split the string at the commas
    // and convert the sections into integers:

    pSensors = sensors;
    sensors = int(split(myString, ','));

    // print out the values you got:
    
    myImage.loadPixels();
for (int i = 1; i < myImage.pixels.length-2; i++) {
  myImage.pixels[i] = color(sensors[i]); 
 // print(sensors[i] + ", ");
}
//println();
myImage.updatePixels();
  image (myImage, 0, 0, width, height);
    
    /*
    for (int sensorNum = 0; sensorNum < sensors.length; sensorNum++) {
      print("Sensor " + sensorNum + ": " + sensors[sensorNum] + "\t");
    }
    
    // add a linefeed after all the sensor values are printed:
    println();
*/
  }
}
