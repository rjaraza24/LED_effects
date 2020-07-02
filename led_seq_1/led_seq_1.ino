#define bote 9
#define logo 3
#define frt 11


// This is a comment

/*
 * This is a paragraph
 * 
 * 
 * logo - bote - frt - off 
 * logo - bote - frt - off
 * fadein(all-var) - fadeoff(all-var)
 * repeat
 */

void setup() {
 Serial.begin(9600);
 pinMode(frt, OUTPUT);
  pinMode(logo,OUTPUT);
   pinMode(bote, OUTPUT);

 digitalWrite(frt, LOW);
 digitalWrite(logo, LOW);
 digitalWrite(bote, LOW);
 
 delay(500);
 Serial.println(" sequencer is ready ");
 
}

void loop() {
delay(200);


//// Seq 1 start
  digitalWrite(logo, HIGH);
  delay(500);
  digitalWrite(bote, HIGH);
  delay(500);
  digitalWrite(frt, HIGH);
  delay(2000);
  digitalWrite(logo, LOW);
//  delay(500);
  digitalWrite(bote, LOW);
//  delay(500);
  digitalWrite(frt, LOW);
  delay(1000);
  digitalWrite(logo, HIGH);
  delay(500);
  digitalWrite(bote, HIGH);
  delay(500);
  digitalWrite(frt, HIGH);
  delay(2000);  
  digitalWrite(logo, LOW);
//  delay(500);
  digitalWrite(bote, LOW);
//  delay(500);
  digitalWrite(frt, LOW);
delay(500);
/////// Seq 1 end

/// Seq 2 start

 for( int i = 0; i<=120 ; i+=1){
  analogWrite(frt, i);
  analogWrite(logo, i);
  analogWrite(bote, i);
  delay(20);
 }

// Seq 2 end

// Seq 3 start
 for( int i = 120; i>=0 ; i-=1){
  analogWrite(frt, i);
  analogWrite(logo, i);
  analogWrite(bote, i);
  delay(20);
 }
//seq 3 end

    
delay(1000);


}
