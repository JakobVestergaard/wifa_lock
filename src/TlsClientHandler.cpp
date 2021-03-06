#include "TlsClientHandler.h"
#define KEY "*******************"

TlsClientHandler::TlsClientHandler(/* args */)
{
}

TlsClientHandler::~TlsClientHandler()
{
}
void TlsClientHandler::setup() {
        // need a Particle time sync for X509 certificates verify.
    if (millis() - lastSync > ONE_DAY_MILLIS) {
        Particle.syncTime();
        lastSync = millis();
    }
}


char *TlsClientHandler::Detect() {

    http.setPath("/face/v1.0/detect?returnFaceId=true");
    http.setHost("iot-facial-compare-test.cognitiveservices.azure.com");
    http.setKey(KEY);
    http.setContent(http.HTTP_OCTET);
    http.setContentLength(0); //set to the right size
            // setup Root CA pem. have to be done before every connect! 
    client.init(letencryptCaPem, sizeof(letencryptCaPem));
    //connect to client on dis, port
    Serial.println(http.makeVerify());
    client.connect(this->URL, 443);

    // check server certificate. if verify failed, TLS connection is alive.
    if (!client.verify()) {
      Serial.println("TLS connection is alive\r\n");
    }

    // Send header to azure.
    int len = sprintf((char *)buff, http.makeVerify());
    client.write(buff, len );

    // GET HTTPS response.
    memset(buff, 0, sizeof(buff));
    while(1) {
        // read renponse.
        memset(buff, 0, sizeof(buff));
        int ret = client.read(buff, sizeof(buff) - 1);
        if (ret == MBEDTLS_ERR_SSL_WANT_READ) {
            delay(100);
        } else if (ret <= 0) {
            break;
        } else {
            Serial.println((char *)buff);
        }
    }
    return nullptr;

}

char * TlsClientHandler::Verify() {
    http.setPath("/face/v1.0/verify");
    http.setHost("iot-facial-compare-test.cognitiveservices.azure.com");
    http.setKey(KEY);
    http.setContent(http.HTTP_JSON);
    http.setBody("c5c24a82-6845-4031-9d5d-978df9175426","815df99c-598f-4926-930a-a734b3fd651c","sample_group");
            // setup Root CA pem. have to be done before every connect! 
    client.init(letencryptCaPem, sizeof(letencryptCaPem));
    //connect to client on dis, port
    Serial.println(http.getHost());
    Serial.println(http.makeVerify());
    client.connect(this->URL, 443);

    // check server certificate. if verify failed, TLS connection is alive.
    if (!client.verify()) {
      Serial.println("TLS connection is alive\r\n");
    }

    // Send header to azure.
    int len = sprintf((char *)buff, http.makeVerify());
    client.write(buff, len );
    //make loop that 

    // GET HTTPS response.
    memset(buff, 0, sizeof(buff));
    while(1) {
        // read renponse.
        memset(buff, 0, sizeof(buff));
        int ret = client.read(buff, sizeof(buff) - 1);
        if (ret == MBEDTLS_ERR_SSL_WANT_READ) {
            delay(100);
        } else if (ret <= 0) {
            break;
        } else {
            Serial.println((char *)buff);
        }
    }
    return nullptr;

}
