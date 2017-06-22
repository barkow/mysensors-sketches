/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Changed for MySensors usage by Bart Eversdijk
 * Version 1.1 - Added option to record manual presets up to 240
 * Version 2.0 - Migrated to MySensrors version 2.0
 * 
 * DESCRIPTION
 *
 * IRrecord: record and play back IR signals as a minimal 
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * An IR LED must be connected to the output PWM pin 3.
 *
 *
 * The logic is:
 * If a V_IR_RECORD is received the node enters in record mode and once a valid IR message has been received 
 * it is stored in EEPROM. The first byte of the V_IR_RECORD message will be used as preset ID 
 * 
 * If a V_IR_SEND the IR message beloning to the preset number of the first message byte is broadcasted
 *
 *
 * Version 0.11 September, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24

#define MY_NODE_ID      5

#include <SPI.h>
#include <MySensors.h>

#define CHILD_ID  2  

void setup()  
{  
  Serial.println(F("Init done..."));
}

void presentation () 
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("IR Rec/Playback", "2.0");

  // Register a sensors to gw. Use binary light for test purposes.
  present(CHILD_ID, S_IR);
}

void loop() 
{
}

void executeCommand(char * command) {
  unsigned int i = 0;
  unsigned char family;
  unsigned char group;
  unsigned char device;
  bool state;
    
  char * tok = strtok (command,":");
  while (tok != NULL)
  {
    switch(i){
      case 0:
        family = tok[0];
        if ((family < 'a') || (family > ('a' + 15))){
          return;
        }
        break;
      case 1:
        group = atoi(tok);
        break;
      case 2:
        device = atoi(tok);
        break;
      case 3:
        state =  atoi(tok) != 0;
        break;
    }
    i++;
    tok = strtok (NULL, ":");
  }
  if (i!=4){
    return;
  }
  if (state){
    Serial.print("Switch On family:");
    Serial.print(family);
    Serial.print(" group:");
    Serial.print(group);
    Serial.print(" device:");
    Serial.println(device);
    //mySwitch.switchOn(family, group, device);
  } else {
    Serial.print("Switch Off family:");
    Serial.print(family);
    Serial.print(" group:");
    Serial.print(group);
    Serial.print(" device:");
    Serial.println(device);
    //mySwitch.switchOff(family, group, device);
  }
}

void receive(const MyMessage &message) {
  //Serial.print(F("New message: "));
  //Serial.println(message.type);
  
  if (message.type == V_IR_SEND) {
    Serial.print(F("Send IR preset: "));
    char* command = message.getString();
    Serial.println(command);
    executeCommand(command); 
  }
}
