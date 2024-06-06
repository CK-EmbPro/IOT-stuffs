#include <ESP8266WiFi.h>

WiFiClient wifiClient;
const char* ssid = "RCA-WiFii";
const char* password = "@rca@2023";
const char* host = "10.5.222.64";
const int httpPort = 8080;

void setup() {
    Serial.begin(115200);
    connectToWiFi(ssid, password);
}

void loop() {
    String mData = "Hello this is ESP8266 wifi module";
    connectToHost(httpPort);
    transferData(mData, "/transferdata");
    delay(30000);
}

void connectToWiFi(const char* ssid, const char* passwd) {
    WiFi.mode(WIFI_OFF); // This prevents reconnection issues
    delay(10);
    WiFi.mode(WIFI_STA); // This hides the viewing of ESP as wifi hotspot
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
    wifiClient.println("Content-Type: text/plain");
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
    Serial.println("Response from server:");
    Serial.println(datarx);
    Serial.println("*******************\n");
    datarx = "";
}
