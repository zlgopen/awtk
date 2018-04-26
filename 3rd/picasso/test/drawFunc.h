

#ifndef _DRAW_FUNC_H
#define _DRAW_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void init_context(ps_context* gc, ps_canvas* cs, unsigned char*);
void dini_context(ps_context* gc);
void draw_test(int id, ps_context* gc);
void set_image_data(unsigned char* data, ps_color_format fmt, int w, int h, int p);
void set_pattern_data(unsigned char* data, ps_color_format fmt, int w, int h, int p);
void timer_action(ps_context* gc);

int argc(void);
const char** argv(void);

extern int thr_run1;
extern int thr_run2;

void start_worker_threads(void*);
void stop_worker_threads(void);
void thread_func1(void*);
void thread_func2(void*);
void loop_wait(int ms);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
