/* Terminator - A simple terminal graphics system */
/* Standard includes */
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
//mine
#include "timing.h"
#include "screen.h"
#include "init.h"
#include "input.h"

int times = 0;
int64_t total = 0;
int64_t min_time = INT64_MAX;
int64_t max_time = 0;

/* Constants and macros */
#define CTRL_KEY(k) ((k) & 0x1f)

bool RUNNING = true;

int io_index = 0;
int render_index = 0;

long rps = 0;
/* Writing */
void*
thread_write() {
    long start = get_ns();
    while(RUNNING){
        if (screen.frames[io_index].state == IO){
            if (screen.frames[io_index].used != 0){
                write_to_terminal(&screen.frames[io_index]);
            }

            screen.frames[io_index].used = 0;
            screen.frames[io_index].state = RENDER;
            io_index = (io_index + 1) % num_frames;

            // Calculate and display timing
            int64_t current = get_ns();
            int64_t elapsed_ns = time_diff_ns(start, current);

            if (elapsed_ns > 0) {
                total += elapsed_ns;
                min_time = (elapsed_ns < min_time) ? elapsed_ns : min_time;
                max_time = (elapsed_ns > max_time) ? elapsed_ns : max_time;
                ++times;
            }

            start = get_ns();
        }
    }
    return NULL;
}

/* Rendering */
void* thread_render() {
    long start = get_us();
    while(RUNNING) {
        if (screen.frames[render_index].state == RENDER) {
            /*
            if(render_index & 1){
                clean_square(&screen.frames[render_index], 10, 10, 40, 20, ' ');
            }
            else {
                draw_pixel(&screen.frames[render_index], 20, 20, 'x');
            }
            */
            screen.frames[render_index].state = IO;
            render_index = (render_index + 1) % num_frames;

            int64_t end = get_us();
            rps = time_diff_us(start, end);
            start = get_us();
}
    }
    return NULL;
}

/* Main entry point */
int main() {
    screen_init();
    enable_raw_mode();

    draw_blank();

    long cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t threads[cpu_num];

    pthread_create(&threads[0], NULL, thread_render, NULL);
    pthread_create(&threads[1], NULL, thread_write, NULL);

    /*
    while (last_key != CTRL_KEY('q')) {
        last_key = editor_read_key();
    }
    */

    while (times < 100000000){

    }

    RUNNING = false;

    for(int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n\r\x1b[K");
    printf("Performance Stats:\n\x1b[K\r");

    if (times > 0) {
        double avg_time = total / (double)times;
        //avg_time = (elasped - main_start) / (double)times;
        printf("Average frame time: %.2f ns (%.2f FPS)\n\x1b[K\r",
               avg_time, ns_to_fps(avg_time));
        printf("Best frame time: %lld ns (%.2f FPS)\n\x1b[K\r", 
               min_time, ns_to_fps(min_time));
        printf("Worst frame time: %lld ns (%.2f FPS)\n\x1b[K\r", 
               max_time, ns_to_fps(max_time));
    }

    disable_raw_mode();
    free_screen();
    return 0;
}
