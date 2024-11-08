
  // ESP32 webserver -  https://navody.dratek.cz/navody-k-produktum/esp-wifi-webserver.html

#define ESP32

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>

const char* root_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFjDCCA3SgAwIBAgINAgCOsgIzNmWLZM3bmzANBgkqhkiG9w0BAQsFADBHMQsw\n"\
"CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n"\
"MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMjAwODEzMDAwMDQyWhcNMjcwOTMwMDAw\n"\
"MDQyWjBGMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n"\
"Y2VzIExMQzETMBEGA1UEAxMKR1RTIENBIDFENDCCASIwDQYJKoZIhvcNAQEBBQAD\n"\
"ggEPADCCAQoCggEBAKvAqqPCE27l0w9zC8dTPIE89bA+xTmDaG7y7VfQ4c+mOWhl\n"\
"UebUQpK0yv2r678RJExK0HWDjeq+nLIHN1Em5j6rARZixmyRSjhIR0KOQPGBMUld\n"\
"saztIIJ7O0g/82qj/vGDl//3t4tTqxiRhLQnTLXJdeB+2DhkdU6IIgx6wN7E5NcU\n"\
"H3Rcsejcqj8p5Sj19vBm6i1FhqLGymhMFroWVUGO3xtIH91dsgy4eFKcfKVLWK3o\n"\
"2190Q0Lm/SiKmLbRJ5Au4y1euFJm2JM9eB84Fkqa3ivrXWUeVtye0CQdKvsY2Fka\n"\
"zvxtxvusLJzLWYHk55zcRAacDA2SeEtBbQfD1qsCAwEAAaOCAXYwggFyMA4GA1Ud\n"\
"DwEB/wQEAwIBhjAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwEgYDVR0T\n"\
"AQH/BAgwBgEB/wIBADAdBgNVHQ4EFgQUJeIYDrJXkZQq5dRdhpCD3lOzuJIwHwYD\n"\
"VR0jBBgwFoAU5K8rJnEaK0gnhS9SZizv8IkTcT4waAYIKwYBBQUHAQEEXDBaMCYG\n"\
"CCsGAQUFBzABhhpodHRwOi8vb2NzcC5wa2kuZ29vZy9ndHNyMTAwBggrBgEFBQcw\n"\
"AoYkaHR0cDovL3BraS5nb29nL3JlcG8vY2VydHMvZ3RzcjEuZGVyMDQGA1UdHwQt\n"\
"MCswKaAnoCWGI2h0dHA6Ly9jcmwucGtpLmdvb2cvZ3RzcjEvZ3RzcjEuY3JsME0G\n"\
"A1UdIARGMEQwCAYGZ4EMAQIBMDgGCisGAQQB1nkCBQMwKjAoBggrBgEFBQcCARYc\n"\
"aHR0cHM6Ly9wa2kuZ29vZy9yZXBvc2l0b3J5LzANBgkqhkiG9w0BAQsFAAOCAgEA\n"\
"IVToy24jwXUr0rAPc924vuSVbKQuYw3nLflLfLh5AYWEeVl/Du18QAWUMdcJ6o/q\n"\
"FZbhXkBH0PNcw97thaf2BeoDYY9Ck/b+UGluhx06zd4EBf7H9P84nnrwpR+4GBDZ\n"\
"K+Xh3I0tqJy2rgOqNDflr5IMQ8ZTWA3yltakzSBKZ6XpF0PpqyCRvp/NCGv2KX2T\n"\
"uPCJvscp1/m2pVTtyBjYPRQ+QuCQGAJKjtN7R5DFrfTqMWvYgVlpCJBkwlu7+7KY\n"\
"3cTIfzE7cmALskMKNLuDz+RzCcsYTsVaU7Vp3xL60OYhqFkuAOOxDZ6pHOj9+OJm\n"\
"YgPmOT4X3+7L51fXJyRH9KfLRP6nT31D5nmsGAOgZ26/8T9hsBW1uo9ju5fZLZXV\n"\
"VS5H0HyIBMEKyGMIPhFWrlt/hFS28N1zaKI0ZBGD3gYgDLbiDT9fGXstpk+Fmc4o\n"\
"lVlWPzXe81vdoEnFbr5M272HdgJWo+WhT9BYM0Ji+wdVmnRffXgloEoluTNcWzc4\n"\
"1dFpgJu8fF3LG0gl2ibSYiCi9a6hvU0TppjJyIWXhkJTcMJlPrWx1VytEUGrX2l0\n"\
"JDwRjW/656r0KVB02xHRKvm2ZKI03TglLIpmVCK3kBKkKNpBNkFt8rhafcCKOb9J\n"\
"x/9tpNFlQTl7B39rJlJWkR17QnZqVptFePFORoZmFzM=\n"\
"-----END CERTIFICATE-----\n";

String Predpoved = "Predpoved...";
String Predpoved_rano = "Rano";
String Predpoved_poledne = "Poledne";
String Predpoved_odpoledne = "Odpoledne";
String Predpoved_dest = "dest";

float aht_temp;
float aht_hum;
float co2;
float co2Z;

String SD_ukazatel = "black";

WebServer server(80);

void zpravaHlavni() {

  const char* obrazek = R"rawliteral(
  <!DOCTYPE HTML><html>
      <img src="https://www.chmi.cz/files/portal/docs/meteo/om/predpovedi/tyden/p_tyden_graf.png" alt="CHMI Image">
      </body>
    </html>
  )rawliteral";
  
  char temp[3500];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  int Dsec = (millis() - casPrijmu) / 1000;
  int Dmin = Dsec / 60;
  int Dhr = Dmin / 60;
 
  snprintf(temp, 3500,

           "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESPwebserver</title>\
    <style>\
      body { background-color: #AFD5F4; font-family: Arial, Helvetica, Sans-Serif; Color: ##243436; }\ 
    </style>\
  </head>\
  <body>\
    <h1><p style=\"text-align: center;\">Meteostanice</p></h1>\
    <hr style=\"width:800px;height:2px;border-width:0;color:#293272;background-color:#293272\">\
    <p style=\"text-align: left;\">Dnesni predpoved:</p>\
    <p style=\"text-align: center;\">Rano: <b>%s</b> ......... Poledne: <b>%s</b> ......... Odpoledne: <b>%s</b> ......... Srazky: <b>%s</b></p>\
    <hr style=\"width:800px;height:2px;border-width:0;color:#293272;background-color:#293272\">\
    <p style=\"text-align: left;\">Aktualne:</p>\
    <center><table width=\"500px\"\"><tr>\
      <td width=\"250px\">\
        <h2><p style=\"text-align: center;\">IN</p></h2>\
        <p style=\"text-align: center;\">Teplota: <b>%.2f *C</b> </p>\
        <p style=\"text-align: center;\">Vlhkost: <b>%.2f %%</b> </p>\
        <p style=\"text-align: center;\">CO<sub>2</sub>: <b>%.2f ppm</b> </p>\
        <p style=\"text-align: center; color: %s;\"><b>SD</b></p>\
      </td><td width=\"250px\">\
        <h2><p style=\"text-align: center;\">OUT</p></h2>\
        <p style=\"text-align: center;\">Teplota v ustredne: <b>%.2f *C</b> </p>\
        <p style=\"text-align: center;\">Teplota: <b>%.2f *C</b> </p>\
        <p style=\"text-align: center;\">Tlak: <b>%.2f hPa</b> </p>\
        <p style=\"text-align: center;\">Vlhkost: <b>%.2f %%</b> </p>\
        <p style=\"text-align: center;\">Rychlost vetru: <b>%.2f m/s</b> </p>\
        <p style=\"text-align: center;\">Naraz vetru: <b>%.2f m/s</b> </p>\
      </td></tr>\
    </table></center>\
    <p style=\"text-align: left;\">Dnes - max/min:</p>\
    <h3><p style=\"text-align: center;\">Teplota:</p></h3>\
    <center><table width=\"400px\"\"><tr>\
      <td width=\"200px\">\
        <p style=\"text-align: center;\">Min: <b>%.2f *C</b></p>\
      </td><td width=\"200px\">\
        <p style=\"text-align: center;\">Max: <b>%.2f *C</b></p>\
      </td></tr>\
    </table></center>\
    <h3><p style=\"text-align: center;\">Vlhkost:</p></h3>\
    <center><table width=\"400px\"\"><tr>\
      <td width=\"200px\">\
        <p style=\"text-align: center;\">Min: <b>%.2f %%</b></p>\
      </td><td width=\"200px\">\
        <p style=\"text-align: center;\">Max: <b>%.2f %%</b></p>\
      </td></tr>\
    </table></center>\
    <h3><p style=\"text-align: center;\">Tlak:</p></h3>\
    <center><table width=\"400px\"\"><tr>\
      <td width=\"200px\">\
        <p style=\"text-align: center;\">Min: <b>%.2f hPa</b></p>\
      </td><td width=\"200px\">\
        <p style=\"text-align: center;\">Max: <b>%.2f hPa</b></p>\
      </td></tr>\
    </table></center>\
    <p style=\"text-align: left;\">Od posledniho prijmu: <b>%02d:%02d:%02d</b></p>\
    <p style=\"text-align: left;\">Napeti akumulatoru: <b>%.2f V (%i%%)</b></p>\
    <p style=\"text-align: left;\">Datum spusteni: %i.%i.%i %02i:%02i</p>",

           Predpoved_rano, Predpoved_poledne, Predpoved_odpoledne, Predpoved_dest, aht_temp, aht_hum, co2, SD_ukazatel, bmpTeplota, dhtTeplota, bmpTlak, dhtVlhkost, rychlostVetru, narazVetru, minTeplota, maxTeplota, minVlhkost, maxVlhkost, minTlak, maxTlak, Dhr, Dmin % 60, Dsec % 60, akuVoltage, akuProcenta, denZ, mesicZ, rokZ, hodinaZ, minutaZ);
  
  server.sendHeader("Content-Type", "text/html");
  server.client().print(temp);
  server.client().print(obrazek);
}

void zpravaNeznamy() {
  // vytvoření zprávy s informací o neexistujícím odkazu
  // včetně metody a zadaného argumentu
  String zprava = "Neexistujici odkaz\n\n";
  zprava += "URI: ";
  zprava += server.uri();
  zprava += "\nMetoda: ";
  zprava += (server.method() == HTTP_GET) ? "GET" : "POST";
  zprava += "\nArgumenty: ";
  zprava += server.args();
  zprava += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    zprava += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  // vytištění zprávy se statusem 404 - Nenalezeno
  server.send(404, "text/plain", zprava);
}

void webserver_SETUP() {
  if (MDNS.begin("meteoserver")) {
    Serial.println("MDNS responder je zapnuty.");
  }

  server.on("/", zpravaHlavni);

  server.on("/ukazka", []() {
    String zprava = "Ukazka odkazu pro vice stranek.";
    server.send(200, "text/plain", zprava);
  });

  zpravaHlavni();

  server.onNotFound(zpravaNeznamy);

  server.begin();
  Serial.println("HTTP server je zapnuty.");

}

String fixCzechCharacters(String data) {
	data.replace("Ě", "E");
	data.replace("Š", "S");
	data.replace("Č", "C");
	data.replace("Ř", "R");
	data.replace("Ž", "Z");
	data.replace("Ý", "Y");
	data.replace("Á", "A");
	data.replace("Í", "I");
	data.replace("É", "E");
	data.replace("Ů", "U");
	data.replace("Ú", "U");
	data.replace("Ď", "D");
	data.replace("Ť", "T");
	data.replace("Ň", "N");
	data.replace("ě", "e");
	data.replace("š", "s");
	data.replace("č", "c");
	data.replace("ř", "r");
	data.replace("ž", "z");
	data.replace("ý", "y");
	data.replace("á", "a");
	data.replace("í", "i");
	data.replace("é", "e");
	data.replace("ů", "u");
	data.replace("ú", "u");
	data.replace("ď", "d");
	data.replace("ť", "t");
	data.replace("ň", "n");
  data.replace("°", "*");
	return data;
}

void Alojz() {

  String ZdrojKod;
  if ((WiFi.status() == WL_CONNECTED)) {

    HTTPClient http;

    http.begin("https://www.alojz.cz/melnik", root_ca);
    int httpCode = http.GET();

    if (httpCode > 0) {
      ZdrojKod = http.getString();
    }
    http.end();
  }

  int firstClosingBracket = ZdrojKod.indexOf("<h2 c");
  int lastClosingBracket = ZdrojKod.indexOf("</h2>", firstClosingBracket + 1 );

  Predpoved = (fixCzechCharacters(ZdrojKod.substring(firstClosingBracket+49, lastClosingBracket)));
//////////////////////////RANO////////////////////
  Predpoved_rano = (fixCzechCharacters(Predpoved.substring(1, 50)));
  firstClosingBracket = Predpoved_rano.indexOf("(");
  lastClosingBracket = Predpoved_rano.indexOf(")", firstClosingBracket + 1 );
  Predpoved_rano = (Predpoved_rano.substring(firstClosingBracket+1, lastClosingBracket));
//////////////////////////POLEDNE////////////////////
  Predpoved_poledne = (fixCzechCharacters(Predpoved.substring(50, 90)));
  firstClosingBracket = Predpoved_poledne.indexOf("(");
  lastClosingBracket = Predpoved_poledne.indexOf(")", firstClosingBracket + 1 );
  Predpoved_poledne = (Predpoved_poledne.substring(firstClosingBracket+1, lastClosingBracket));
//////////////////////////ODPOLEDNE////////////////////
  Predpoved_odpoledne = (fixCzechCharacters(Predpoved.substring(90)));
  firstClosingBracket = Predpoved_odpoledne.indexOf("(");
  lastClosingBracket = Predpoved_odpoledne.indexOf(")", firstClosingBracket + 1 );
  Predpoved_odpoledne = (Predpoved_odpoledne.substring(firstClosingBracket+1, lastClosingBracket));
//////////////////////////DEST////////////////////
  Predpoved_dest = (fixCzechCharacters(Predpoved.substring(90)));
  firstClosingBracket = Predpoved_dest.indexOf(".");
  lastClosingBracket = Predpoved_dest.indexOf(".", firstClosingBracket + 1 );
  Predpoved_dest = (Predpoved_dest.substring(firstClosingBracket+2, lastClosingBracket));
}

