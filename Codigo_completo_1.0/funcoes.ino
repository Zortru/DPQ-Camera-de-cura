void zero_cross_detect() {  
  zero_cross = true; // set the boolean to true to tell our dimming function that a zero cross has occured
  y=0;
  digitalWrite(activate, LOW);// turn off TRIAC (and AC)
}                                 

// Turn on the TRIAC at the appropriate time

void dim_check() {          
  if(zero_cross == true) {              
    if(y>=dim) {                     
      digitalWrite(activate, HIGH); // turn on light       
      y=0;  // reset time step counter                         
      zero_cross = false; //reset zero cross detection
    } 
    else {
      y++; // increment time step counter                     
    }                                
  }                                  
}  

int potencia(float pot) {
  int resp;
  if (pot <= 28.968) { //9
    if (pot <= 4.049) { //5      
      if (pot <= 1.248) { //3       
        if (pot <= 0.413) { //2
          resp = 26.634*pot;
          }
        else { //2
          resp = 5.988*pot+85269;
          }
        }
        else { //3       
          if (pot <= 2.394) { //4
            resp = 3.4904*pot+11.644;
            }
          else { //4
            resp = 2.4169*pot+14.214;
            }
          }
        }
        else { //5      
          if (pot <= 12.518) { //7        
            if (pot <= 6.268) { // 6
              resp = 1.8026*pot+16.701;
              }
            else { //6
              resp = 1.28*pot+19.997;
              }
            }
          else { //7        
            if (pot <= 19.945) { //8
              resp = 0.9425*pot+24.202;
              }
            else { //8
              resp = 0.7758*pot+27.527;
              }
            }
          }
        } 
      else  { //9    
        if (pot <= 90.912) { //13      
          if (pot <= 75.071) { //11        
            if (pot <= 63.835) { //10
              resp = 0.6596*pot+30.891;
              }
            else { //10
              resp = 0.712*pot+27.55;
              }
            }
          else { //11        
            if (pot <= 84.513) { //12
              resp = 0.8473*pot+17.394;
              }
            else { // 12
              resp = 1.0939*pot-3.4505;
              }
            }
          }
        else { //13        
          if (pot <= 98.238) { //15          
            if (pot <= 95.451) { //14
              resp = 1.5422*pot-44.204;
              }
            else { //14
              resp = 2.5117*pot-136.74;
              }
            }
          else { //15          
            if (pot <= 99.688) { //16
              resp = 5.5172*pot-432;
              }
            else { //16
              resp = 32.051*pot-3077.1;
              }
            }
          }
         }    
      return resp;    
}
