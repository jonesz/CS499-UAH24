#ifndef PROGRAM_PROGRAM_H
#define PROGRAM_PROGRAM_H
#define DUMB_FS_PID 0x2

int echo_main(int argc, char **argv);
int shell_main(int argc, char **argv);
int cat_main(int argc, char **argv);
void dumb_fs();

#endif // PROGRAM_PROGRAM_H
