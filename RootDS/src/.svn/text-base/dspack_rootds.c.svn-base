/************************************************/
/*                                              */
/* C++ interface to DSPACK                      */
/*                                              */
/* needed to avoid problems with dsc.h in C++   */
/* and to only declare extern "C" if necessary  */
/*                                              */
/* With the new DSPACK version these problems   */
/* are probably fixed, but you know:            */
/* temporary solutions (e.g.. fixes) are the    */
/* most stable ones.                            */
/************************************************/
  
#include <stdio.h>

#define FALSE 0
#define TRUE 1

#include "dsc_prot.h"

/* 
int init_dspack_db(int mode, char *server, char **argv, int *ierr) { 

    *ierr = 0;  
    return init_na49db(server, argv, ierr); 

 } 
*/

int init_dspack(int mode, char *name, char **argv,int db) {

    int ierr = 0;
    
    ds_carg(argv);
#ifdef HP
    if(!db)
        return dsinit(mode,name,6,0,0, &ierr);
    else
        return init_na49db(" ",argv,&ierr);
#else
        return dsinit(mode,name,6,0,0, &ierr);
#endif

}

int open_infile(char *filename) {

    int ierr = 0;
    int ktype = 0;
    int id_in = 0;
    
    id_in = ifs_iunit(&ierr);
    if (ierr) {
        fprintf(stderr,"ERROR: No free DSPACK input unit!\n");
        return(-1);
    }
    fsopin(id_in, filename, " ",&ierr);
    if (ierr) {
        fprintf(stderr,"ERROR: file %s not found!\n",filename);
        return(-1);
    }
    fdsin (id_in, &ktype, &ierr);

    return id_in;
}

int open_outfile(char *filename) {

    int ierr = 0;
    int id_out = 0;
    
    id_out = ifs_ounit(&ierr);
    if (ierr) {
        fprintf(stderr,"ERROR: No free DSPACK output unit!\n");
        return(-1);
    }
    fsopou(id_out, filename, " ",&ierr);
    if (ierr) {
        fprintf(stderr,"ERROR: file %s not found!\n",filename);
        return(-1);
    }

    return id_out;
}

int mark_objects(int id_out, char **objects, int nobj) {

    int nout = 0;
    int i;
    int ierr = 0;
    
    for(i=0; i<nobj ;i++) {
        fsmark(id_out, objects[i], &nout, &ierr);
    }
    fsoutv(id_out, 2, &nout, &ierr);

    return TRUE;

}

int clear_section(int section) {

    return dsnclr(section);
    
}

int read_file(int file_id) {

    int ktyp = 0;
    int ierr = 0;

    do {

        fdsin(file_id, &ktyp, &ierr);
        if (ierr) {
            if (ierr == 2) {
                fprintf(stderr,"End of input file!\n");
            } else {
                fprintf(stderr,"ERROR while reading event!\n");
            }
            return FALSE;
        }
        
    } while (ktyp != 1);
    
    return TRUE;

}

int write_file(int file_id) {

    int ierr = 0;
    int nout = 0;

    return fsoutv(file_id, 1, &nout, &ierr);
    
}


int close_infile(int file_id) {

    return fsendi(file_id);
    
}

int close_outfile(int file_id) {

    return fsendo(file_id);
    
}

void *get_objects(char *name, int *num) {

    int handle = 0;
    
    *num = 0;
    return (void *)idsget_ds(name, &handle, num);

}

void *create_objects(char *name, int num) {

    int handle = 0;
    
    return (void *)idsput_ds(name, &handle, num);

}

void read_definitions(char *filename) {

    int ierr = 0;
    
    ds_ascii(dsexna(filename), 0, &ierr);

}

int purge_linked_list(char *name, void *start, int *nleft) {

    int ierr = 0;
    int handle = 0;
    
    return ds_clean_ll(name, &handle, start, nleft, &ierr);

}

#ifndef HP

/* int trkstep_(float* x1,float* x2,float* x3,float* x4,float* x5,float* x6,char* x7,char* x8,char* x9,char* x10,int* x11) */
/* { */
/*     return 0; */
/* } */
#endif
