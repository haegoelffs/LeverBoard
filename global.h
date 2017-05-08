  #ifdef GLOB
  #define EXTERN1
  #else
  #define EXTERN2 extern
  #endif
  EXTERN1 char phaseState;
  
  EXTERN2 uint16_t delta_time;
