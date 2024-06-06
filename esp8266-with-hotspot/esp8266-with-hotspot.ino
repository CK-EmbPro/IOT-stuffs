#include <ESP8266WiFi.h>

WiFiClient wifiClient;
WiFiServer server(80); // Create a server on port 80

const char* ssid = "ESP8266_Hotspot";
const char* password = "12345678";
const char* host = "10.5.222.64";
const int httpPort = 8080;
const char* stationSsid = "RCA-WiFii";
const char* stationPassword = "@rca@2023";

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_AP_STA); // Enable both AP and STA modes
    setupHotspot(ssid, password); // Set up the hotspot
    connectToWiFi(stationSsid, stationPassword); // Connect to WiFi network
}

void loop() {
    // Handle hotspot clients
    WiFiClient client = server.available();
    if (client) {
        Serial.println("New Client Connected.");
        String currentLine = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        // HTTP headers received, send response
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();
                        client.print("Hello, you are connected to ");
                        client.print(ssid);
                        client.println("<br>Use this network to communicate with ESP8266");
                        client.println();
                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }
            }
        }
        client.stop();
        Serial.println("Client Disconnected.");
    }

    // Send data to the server
    String mData = "device=ckTHinkBOok&temperature=25.00";
    connectToHost(httpPort);
    transferData(mData, "/transferdata");
    delay(30000);
}

void connectToWiFi(const char* ssid, const char* passwd) {
    WiFi.begin(ssid, passwd); // Connect to your WiFi router
    Serial.println("Connecting to WiFi...");
    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
        retryCount++;
        if (retryCount >= 30) { // Try for 30 seconds
            Serial.println();
            Serial.println("Failed to connect to WiFi");
            return;
        }
    }
    Serial.println();
    Serial.print("WiFi Connected. IP Address: ");
    Serial.println(WiFi.localIP());
}

void setupHotspot(const char* ssid, const char* passwd) {
    WiFi.softAP(ssid, passwd); // Start the Access Point with the given SSID and password

    IPAddress IP = WiFi.softAPIP(); // Get the IP address of the Access Point
    Serial.print("Access Point IP Address: ");
    Serial.println(IP);
    
    server.begin(); // Start the server
}

void connectToHost(const int httpPort) {
    int retry_counter = 0; // To be used while retrying to get connected
    wifiClient.setTimeout(15000); // 15 Seconds
    delay(1000);
    Serial.printf("Connecting to \"%s:%d\"\n", host, httpPort);

    while ((!wifiClient.connect(host, httpPort)) && (retry_counter <= 30)) {
        delay(100);
        Serial.print(".");
        retry_counter++;
    }

    if (retry_counter == 31) {
        Serial.println("\nConnection failed.");
        return;
    } else {
        Serial.println("Connected.");
    }
}

void transferData(String data, const char* filepath) {
    wifiClient.println("POST " + String(filepath) + " HTTP/1.1");
    wifiClient.println("Host: " + String(host));
    wifiClient.println("User-Agent: ESP8266/1.0");
    wifiClient.println("Content-Type: application/x-www-form-urlencoded");
    wifiClient.println("Content-Length: " + String(data.length()));
    wifiClient.println();
    wifiClient.print(data);
    Serial.println("Uploading data...");
    parseResponse();
}

/*
 * GET FEEDBACK
 */
void parseResponse() {
    String datarx;
    while (wifiClient.connected()) {
        String line = wifiClient.readStringUntil('\n');
        if (line == "\r") {
            break;
        }
    }
    while (wifiClient.available()) {
        datarx += wifiClient.readStringUntil('\n');
    }
    Serial.print("Response from server: ");
    Serial.println(datarx);
    datarx = "";
}
