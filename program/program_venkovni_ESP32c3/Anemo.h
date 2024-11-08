
#define ANEMO_PIN 19

#define READING_TIME 60     // s

float rychlostVetru;
float narazVetru;

float konstanta = 2.7;


void ReadWindSpeed() {

  float polomer = 0.06;   //m
  float delkaPulotacky = 0.188495559;     //m

  bool hrana = false;

  unsigned int casZacatkuPulzu;
  float casPulotacky;
  float pocetPulotacek = -1;

  unsigned int zacatek = millis();
  narazVetru = 0;

  while((millis() - zacatek) <= (READING_TIME*1000)) {
    
    if((digitalRead(ANEMO_PIN) == 0) && (hrana == false)) {
      pocetPulotacek += 1;
      if(pocetPulotacek == 0) {
        zacatek = millis();
        casZacatkuPulzu = millis();
        casPulotacky = 0;
        hrana = true;
      }
      else {
        casPulotacky = millis() - casZacatkuPulzu;
        casZacatkuPulzu = millis();
        hrana = true;

        rychlostVetru = (delkaPulotacky / (casPulotacky/1000)) * konstanta;      /// m/s
        if(rychlostVetru > narazVetru) narazVetru = rychlostVetru;
      }
    }
    if((millis() - zacatek >= 30000) && (pocetPulotacek < 1)) break;

    if(digitalRead(ANEMO_PIN) == 1) hrana = false;
  }

  rychlostVetru = ((delkaPulotacky * pocetPulotacek) / READING_TIME) * 3;
  if(rychlostVetru < 0) rychlostVetru = 0;

}