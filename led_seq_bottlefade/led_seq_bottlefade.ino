 int led1 = 3; // Apple
int led2 = 8;  // Bottle
int led3 = 12;  // Branding

#define fadeTime1 50 // for the wait in the increase / decrease of luminosity 
#define fadeTime2 500  // for the time which is kept each color 

void setup() {
  // put your setup code here, to run once:

  pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
  int r, g, b; 
  
analogWrite (led3, 0); 
analogWrite (led1, 0); 
analogWrite (led2, 0);


analogWrite (led3, 0);
delay(500); 
analogWrite (led3, 120); 
delay(500); 
analogWrite (led3, 0);
delay(500); 
analogWrite (led3, 120);
delay(500); 
analogWrite (led3, 0);
delay(500);
analogWrite (led3, 120);
delay(500); 
analogWrite (led3, 0);
delay(500);  
for (r = 0; r<120; r+=4) 
{ 
analogWrite (led3, r); 
delay (fadeTime1); 
} 
delay(200);
for (r = 0; r<80; r+=4) 
{ 
analogWrite (led2, r); 
delay (fadeTime1); 
} 
delay (fadeTime2); 

analogWrite (led1, 0);
delay(50);
analogWrite (led1, 120); 
delay(100); 
analogWrite (led1, 0);
delay(50);
analogWrite (led1, 120); 
delay(100); 
analogWrite (led1, 0);
delay(50); 
analogWrite (led1, 120); 
delay(100); 
analogWrite (led1, 0);
delay(50); 
analogWrite (led1, 120);
delay(100); 
analogWrite (led1, 0);
delay(50); 
analogWrite (led1, 120);
delay(100); 
analogWrite (led1, 0);
delay(50); 
analogWrite (led1, 120);
delay(100); 
analogWrite (led1, 0);
delay(50); 
analogWrite (led1, 120);
delay(100); 
analogWrite (led1, 0);

delay(500); 
//analogWrite (led1, 120);
//delay(500);

for (r = 0; r<120; r+=3) 
{ 
analogWrite (led1, r); 
delay (fadeTime1); 
} 

delay(2000);

//analogWrite (led1, 0); 
//delay(500);
//analogWrite (led2, 0); 
//delay(500);
//analogWrite (led3, 0);
//delay(500);



// from violet to red 
//for (b = 0;b<120; b+=3) 
//{ 
//analogWrite (led2, b); 
//analogWrite (led1, 0); 
//analogWrite (led3, 0); 
//delay (fadeTime1); 
//} 
//delay (fadeTime2); 
// from red to yellow 
//for (g = 0; g<120; g+=3) 
//{ 
//analogWrite (led3, g); 
//analogWrite (led1, 0); 
//analogWrite (led2, 0); 
//delay (fadeTime1); 
//} 
//delay (fadeTime2); 

//// from yellow to green 
for (r = 120; r>=0; r-=4) 
{ 
analogWrite (led1, r); 
delay (fadeTime1); 
} 
delay(200);
for (r = 80; r>=0; r-=4) 
{ 
analogWrite (led2, r); 
delay (fadeTime1); 
} 
delay(200);
for (r = 120; r>=0; r-=4) 
{ 
analogWrite (led3, r); 
delay (fadeTime1); 
} 
delay(200);
//delay (fadeTime2); 
  
}
