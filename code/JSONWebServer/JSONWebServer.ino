// arduino IDE 1.0
// for W5100 ethernet shield
// the IP address will be dependent on your local network/router
// port 80 is default for HTTP, but can be changed as needed
// use IP address like http://192.168.1.102:84 in your brouser
// or http://zoomkat.no-ip.com:84 with dynamic IP service
// use the \ slash to escape the " in the html
// meta refresh set for 2 seconds

#include <SPI.h>
#include <Ethernet.h>

int x=0; //set refresh counter to 0
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,102); // ip in lan
EthernetServer server(84); //server is using port 84

void setup()
{
  // start the server
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
     while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // see if HTTP request has ended with blank line
        if (c == '\n') {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          
          //meta-refresh page every 2 seconds
          x=x+1;
          client.println("<HTML>");
          client.print("<HEAD>");
          client.print("<meta http-equiv=\"refresh\" content=\"2\">");
          client.print("<TITLE />Zoomkat's meta-refresh test</title>");
          client.print("</head>");
          client.println("<BODY>");
          client.print("Zoomkat's meta-refresh test IDE 1.0");
          client.println("<br />");
                    
          client.print("page refresh number ");
          client.println(x); //current refresh count
          client.println("<br />");
          client.println("<br />");
          
          client.print("Zoomkat's arduino analog input values:");
          client.println("<br />");
          client.println("<br />");
          
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(analogRead(analogChannel));
            client.println("<br />");
            }
           break;
          client.println("</BODY>");
          client.println("</HTML>");
         }
        }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}