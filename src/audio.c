#include <stdint.h>

#include <tusb.h>
#include <pico/audio_i2s.h>
#include "audio.h"

extern audio_buffer_pool_t *audio_pool;

void audio_task(void) {
    if (tud_audio_available()) {
        // Get a buffer from the audio pool
        audio_buffer_t *buffer = take_audio_buffer(audio_pool, true);
        if (!buffer) return;

        // Read audio data from USB into the buffer
        int bytes_read = tud_audio_read(buffer->buffer, buffer->max_sample_count * 4); // 4 bytes = 2ch × 16-bit
        if (bytes_read > 0) {
            buffer->sample_count = bytes_read / 4;
            give_audio_buffer(audio_pool, buffer);
        } else {
            // Return the buffer unused
            give_audio_buffer(audio_pool, buffer);
        }
    }
}