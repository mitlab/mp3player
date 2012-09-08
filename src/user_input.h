#if !defined(USER_INPUT_H_)
#define USER_INPUT_H_

#define PHASE_A     (PINA & 1<<PA3)
#define PHASE_B     (PINA & 1<<PA4)
 
void encode_init( void );
int8_t encode_read1( void );
int8_t encode_read2( void );
int8_t encode_read4( void );

#endif // USER_INPUT_H_