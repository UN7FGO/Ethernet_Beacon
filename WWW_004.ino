#include <SPI.h>
#include <Ethernet.h>
#include <String.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(172, 25, 241, 222);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

int mode = 1;
String inString = String(35);

void setup() {
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop() {

  inString = "";
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (inString.length() < 35) 
        {
          inString.concat(c);
        } 

        if(inString.indexOf("?mm=")>0 ){
          mode = int(inString.charAt(inString.indexOf("?mm=")+4))-48;
          if ( (mode>4) || (mode<0) ){
            mode = 0;
          }
        }

        if (c == '\n' && currentLineIsBlank) {
 
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  
          client.println("Refresh: 30");  
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<title>Arduino beacon web-server</title>");
          client.println("<html><PRE>");

          client.println("<b>GENERAL INFORMATION</b>");
          client.println();
          client.print("CURRENT TIME ON BEACON: <b><font color=#0000FF>");
          client.print("HH:MM");
          client.println("</font></b>");
          client.println();

          client.print("CURRENT TRANSMITTER TEMPERATURE: <b><font color=#0000FF>");
          client.print("23 C");
          client.println("</font></b>");
          client.println();

          client.print("COOLING FAN  CONDITION: <b><font color=#0000FF>");
          client.print("OFF");
          client.println("</font></b>");
          client.println();

          client.print("INHIBITION OF WORK ON OVERHEATING: <b><font color=#0000FF>");
          client.print("OFF");
          client.println("</font></b>");
          client.println();

          client.print("CURRENT STATE: <b><font color=#00AA00>");
          client.print("WAIT / WORK");
          client.println("</font></b>");
          client.println();

          client.print("CURRENT MODE : <b><font color=#FF0000>");
          switch (mode) {
            case 0:
              client.print("Stop");
              break;
            case 1:
              client.print("always off");
              break;
            case 2:
              client.print("Working every 5 minutes");
              break;
            case 3:
              client.print("Working 10 minutes at the beginning of each hour");
              break;
            case 4:
              client.print("always on");
              break;
              
          }
          client.println("</font></b>");
          
          client.println();
          client.println("<b>SWITCH TO MODE:</b>");
          client.println("[ <a href=/?mm=1>MODE - 1</a> ] - always off");
          client.println("[ <a href=/?mm=2>MODE - 2</a> ] - every 5 minutes");
          client.println("[ <a href=/?mm=3>MODE - 3</a> ] - 10 minutes at the beginning of each hour");
          client.println("[ <a href=/?mm=4>MODE - 4</a> ] - always on");
          client.println();
          client.println("[ <a href=/?mm=0>S T O P</a>  ] - stop working");

          client.println();
          client.println();
          client.println("Arduino beacon (C) <a href=http://un7fgo.gengen.ru>un7fgo.gengen.ru</a>");
          client.println();


            
          client.println("</PRE></html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}



