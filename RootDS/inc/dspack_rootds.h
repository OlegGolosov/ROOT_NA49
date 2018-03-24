#ifndef _DSPACK_H_INCLUDED_
#define _DSPACK_H_INCLUDED_

/* DSPACK extern declarations: */

extern "C" int init_dspack_db(int, char *, char **, int *);
extern "C" int init_dspack(int, char *, char **,int);
extern "C" int open_infile(char *);
extern "C" int open_outfile(char *);
extern "C" int mark_objects(int, char **, int);
extern "C" int clear_section(int);
extern "C" int read_file(int);
extern "C" int write_file(int);
extern "C" int close_infile(int);
extern "C" int close_outfile(int);
extern "C" void *get_objects(char *, int *);
extern "C" void *create_objects(char *, int);
extern "C" void read_definitions(char *);
extern "C" int purge_linked_list(char *, void *, int *);

/* for T49Trkstep */
extern "C" void trkstep_ (float *q, float *xfrom, float *pfrom, float *xto, float *pto,
                          int *fixstep, float *fixdis, int *check, int *ierr);
extern "C" char init_na49db (char *dspack_server, char **argv, int *ierr);           

/* for T49Padtrans */
extern "C" int    pad2int (int detector, int sector,
                           int row, double pad, double time,
                           double *xint, double *yint, double *zint);
extern "C" int    int2pad (int detector, double xint, double yint, double zint,
                           int *sector, int *row, double *pad, double *time);
extern "C" int    int2ext (int detector, double xint, double yint, double zint,
                           double *xext, double *yext, double *zext);
extern "C" int    ext2int (int detector, double xext, double yext, double zext,
                           double *xint, double *yint, double *zint);
extern "C" int    int2sr  (int detector, double xint, double yint, double zint,
                           int *sector, int *row);
extern "C" int    int2drow (int detector, double xint, double yint, double zint,
                            int *sector, double *row, double *pad, double *time);
extern "C" int    drow2int (int detector, int sector,
                            double row, double pad, double time,
                            double *xint, double *yint, double *zint);
extern "C" double pad2channel (int detector, int sector, int row, double pad);
extern "C" double channel2pad (int detector, int sector, int row, double channel);
extern "C" int    ext2beam (double xext, double yext, double zext,
                            double *xbeam, double *ybeam, double *zbeam);
extern "C" int    beam2ext (double xbeam, double ybeam, double zbeam,
                            double *xext, double *yext, double *zext);
extern "C" void   set_padtrans_verbose (int print_on);        

#endif
