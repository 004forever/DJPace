//
//  Sound.h
//  final
//
//  Created by Sam Vercauteren on 4/23/15.
//
//

#include <avr/io.h>
#include <util/delay.h>


#define PLAY_PAUSE    0xfffe
#define STOP          0xffff


void init_audio(void);
void audio_reset(void);
void send_audio_data(uint16_t);