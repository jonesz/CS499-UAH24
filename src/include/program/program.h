#ifndef PROGRAM_PROGRAM_H
#define PROGRAM_PROGRAM_H
#define DUMB_FS_PID 0x2

int echo_main(int argc, char **argv);
int overflow_main(int argc, char **argv);
int shell_main(int argc, char **argv);
int cat_main(int argc, char **argv);
void dumb_fs(int argc, char **argv);
void ping_main(int argc, char **argv);
void pong_main(int argc, char **argv);

#endif // PROGRAM_PROGRAM_H
