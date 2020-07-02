#define pic 6
#define pic2 5
#define pic3 4
#define pic4 2
#define pic5 3
#define pic6 7
#define pic7 10
 

int dly = 3000;
int dly2 = 5000;

int val = 200;

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
 pinMode(pic, OUTPUT);
 pinMode(pic2,OUTPUT);
 pinMode(pic3, OUTPUT);
 pinMode(pic4, OUTPUT);
 pinMode(pic5,OUTPUT);
 pinMode(pic6, OUTPUT); 
 pinMode(pic7, OUTPUT);
   

 digitalWrite(pic, HIGH);
 digitalWrite(pic2, HIGH);
 digitalWrite(pic3, HIGH);
 digitalWrite(pic4, HIGH);
 digitalWrite(pic5, HIGH);
 digitalWrite(pic6, HIGH);
 digitalWrite(pic7, HIGH);
 
 delay(500);
 Serial.println(" sequencer is ready ");
 
}

void loop() {
// enf1(); ///3secs 7 sequence
 enf2(); ///5secs   6 sequence

}


void enf1(){
 digitalWrite(pic, HIGH);
 digitalWrite(pic2, HIGH);
 digitalWrite(pic3, HIGH);
 digitalWrite(pic4, HIGH);
 digitalWrite(pic5, HIGH);
 digitalWrite(pic6, HIGH);
 digitalWrite(pic7, HIGH);
 delay(dly);
 digitalWrite(pic, LOW);
 delay(dly);
 digitalWrite(pic2, LOW);
 delay(dly);
 digitalWrite(pic3, LOW);
 delay(dly);
 digitalWrite(pic4, LOW);
 delay(dly);
 digitalWrite(pic5, LOW);
 delay(dly);
 digitalWrite(pic6, LOW);
 delay(dly);
 digitalWrite(pic7, LOW);
 delay(dly);


}




void enf2(){
 digitalWrite(pic, HIGH);
 digitalWrite(pic2, HIGH);
 digitalWrite(pic3, HIGH);
 digitalWrite(pic4, HIGH);
 digitalWrite(pic5, HIGH);
 digitalWrite(pic6, HIGH);
 digitalWrite(pic7, HIGH);
 delay(dly);
 digitalWrite(pic, LOW);
 delay(dly);
 digitalWrite(pic2, LOW);
 delay(dly);
 digitalWrite(pic3, LOW);
 delay(dly);
 digitalWrite(pic4, LOW);
 delay(dly);
 digitalWrite(pic5, LOW);
 delay(dly2);
 digitalWrite(pic6, LOW);
 delay(dly2);



}
