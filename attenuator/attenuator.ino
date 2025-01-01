// Definer pinnene for de parallell datautgangene
//const int DATA_PIN[6] = {2, 3, 4, 5, 6, 7}; // Bruker digital pinne 2-7
const int DATA_PIN[6] = {7, 6, 5, 4, 3, 2}; // Bruker digital pinne 2-7
const int NUM_BITS = 6; // Antall bits for dempeleddet

void setup() {
  // Sett opp datautganger som utgangspins
  for (int i = 0; i < NUM_BITS; i++) {
    pinMode(DATA_PIN[i], OUTPUT);
  }
  
  // Start UART kommunikasjon
  SerialUSB.begin(9600);
}

void loop() {
  // Sjekk om det er tilgjengelig data på UART
  if (SerialUSB.available() > 0) {
    // Les inn dataen fra UART
    String input = SerialUSB.readStringUntil('\n');
    
    // Sjekk om inputformatet er korrekt (dB:nn.n)
    if (input.startsWith("dB:")) {
      float dB_value = input.substring(3).toFloat();
      
      // Sjekk at verdien er innenfor gyldig område
      if (dB_value >= 0.0 && dB_value <= 31.5) {
        // Beregn binærverdien som skal sendes til dempeleddet
        int binaryValue = (int)(dB_value * 2); // Konverterer dB til 0.5dB steps
        
        // Skriv ut binærverdien til parallellutgangene
        for (int i = 0; i < NUM_BITS; i++) {
          digitalWrite(DATA_PIN[i], (binaryValue >> i) & 1);
        }

        // Send tilbake bekreftelse
        SerialUSB.print("Set attenuation to: ");
        SerialUSB.print(dB_value);
        SerialUSB.println(" dB");
      } else {
        SerialUSB.println("Error: Value out of range (0.0 - 31.5 dB)");
      }
    } else {
      SerialUSB.println("Error: Invalid format. Use 'dB:nn.n'");
    }
  }
}
