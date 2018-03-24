#ifndef DDSProt
#define DDSProt

#define DSPACK_VAR_SECTION 1
#define DSPACK_CON_SECTION 2

#include "ds_object.h"
#include "dspack_z.h"

#ifdef __cplusplus
#define CC_P "C"
#else
#define CC_P
#endif

/* Initialization */

extern CC_P int ds_carg(char **argv);

extern CC_P int ds_init(const char*, int*);
extern CC_P int ds_reconnect( char*, int*);
extern CC_P int dsinit(const int, const char*, const int, const int, const int,  int*);
extern CC_P int ds_install(const int, const int (*)(void));
extern CC_P int ids_install_command(const char*, const int (*)(void));
extern CC_P int ids_done(int*);
extern CC_P int ds_reg_reconnect_p(void**);
extern CC_P int ds_clear_reconnect();

/* access to data */
/* getting data */

extern CC_P int dsget(const char*, int*, const int, int*, int*, int*);
extern CC_P int ids_wait(const char*, int*, const int);
extern CC_P void *idsget(const char*, int*, const int, int*, int*, const int, int*, int*, int*);
extern CC_P void *idspnt(const char*, int*, const int, int*, int*);
extern CC_P void *idsget_ds(const char*, int*, int*);
extern CC_P void *idsget_ds_nc(const char*, int*, int*);
extern CC_P void *idsget_ds_n(const char*, int*, int);
extern CC_P void *idsget_ds_n_nc(const char*, int*, int);

/* storing data */

extern CC_P int dsput(const char*, int*, const int, void*, int*);
extern CC_P int ds_putt(const char*, int*);
extern CC_P int dsbook(const char*, int*, const int, int*);
extern CC_P int dsadde(const char*, int*, const int, int*, void*, int*);
extern CC_P int dsrepl(const char*, int*, const int, const int, void*, int*);
extern CC_P int dsset(const char*, int*, const int, const int, const int, int*);
extern CC_P void *idsput_ds(const char*, int*,int nent);
extern CC_P void *idsrebook(const char*, int*, const int, int*);
extern CC_P void *ids_add_entry(const char*, int*, const int, const int, void*, int*);
extern CC_P void *idsadd(const char*, int*, const int, int*, int*, int*, int*);

/* Manipulations of objects */

extern CC_P int ds_update(const char*, int*, int*);
extern CC_P int ds_update_ns(const char*, int*, int*);
extern CC_P int ds_upd_wait(const char*, int*, const char*, int*, int*);
extern CC_P int ds_clear(void);
extern CC_P int dsnclr(const int);
extern CC_P int dsclr(const char*, int*, const int, int*);
extern CC_P int ds_resize(const char*, int*, const int, int*);
extern CC_P int dscutl(const int, const int, int*);
extern CC_P int dsdrop(const char*, int*, int*);
extern CC_P int ids_delete(const char*, int*, int*);
extern CC_P int dshide(const char*, int*, int*, int*, int*);
extern CC_P int dsrest(const int, const int, const char*, int*);
extern CC_P int dssel(const char*, int*, const int, int*, int*, int*);
extern CC_P int ds_clean_ll(const char*, int*, void*, int*, int*);
extern CC_P int ds_ll(const char*, int*);
extern CC_P int ds_map_mem(const char*, int*, void*, int*);
extern CC_P int ds_map_object(const char*, int*, const char*, int*, int*);
extern CC_P int ds_cp_object(const char*, int*, const char*, int*, const int, int*);
extern CC_P int ds_cp(const char*, int*, const char*, int*, const int, int*);
extern CC_P int ds_append(const char*, int*, const char*, int*, int*);
extern CC_P int ds_mv_object(const char*, int*, const char*, int*, int*);
extern CC_P int ds_rm_object(const char*, int*, int*);
extern CC_P int ids_pnt2h(const void*, int*);
extern CC_P int ids_segm2h(const int);
extern CC_P int ds_delete_priv(ds_ext_obj*, int*);
extern CC_P int ids_release(ds_ext_obj*);
extern CC_P int ids_get_external(const char*, const char*, ds_ext_obj*, int*);
extern CC_P int ids_free_memory(const void*); 
extern CC_P void *ids_h2pnt(const int, const int);
extern CC_P void *ids_create_priv(const char*, const int, ds_ext_obj*, int*);
extern CC_P void *ids_memory(const int, const int); 
extern CC_P void *ids_resize_memory(const void*, const int); 

/* I/O */

extern CC_P int fsopin(const int, const char*, const char*, int*);
extern CC_P int fsopou(const int, const char*, const char*, int*);
extern CC_P int fsmark(const int, const char*, int*, int*);
extern CC_P int fsmark_s(const int, const int, int*, int*);
extern CC_P int fsumrk(const int, const char*, int*, int*);
extern CC_P int fsoutv(const int, const int, int*, int*);
extern CC_P int fdsin(const int, int*, int*);
extern CC_P int fsendo(const int);
extern CC_P int fsendi(const int);
extern CC_P int fsclr(const int, const int, int*);
extern CC_P int fscopy(const int, const int, int*);
extern CC_P int fsvers(float*, float*, int*, int*);
extern CC_P int fsshve(const int);
extern CC_P int ds_write(const int, int*);
extern CC_P int ds_read(int*);
extern CC_P int ds_read_dsfile(char*, int*);
extern CC_P int ds_close_out(void);
extern CC_P int ds_close_in(void);
extern CC_P char *ds_io_lst(const int, const int, char*, int*, int*);
extern CC_P char *ds_oo_lst(const int, const int, char*, int*, int*);
extern CC_P int ds_lock(const char*, int*, int*);
extern CC_P int ds_unlock(const char*, int*, int*);
extern CC_P int ifs_iunit(int*);
extern CC_P int ifs_ounit(int*);
extern CC_P int ds_getfile(char*, char*, int*);
extern CC_P int ds_putfile(char*, char*, char*, int*);
extern CC_P int fs_ext_io(int, char*, void(*)(), void(*)(), void(*)(), int*, int);
extern CC_P int fs_seek(int, int);
extern CC_P int fs_tell(int, int*, int*, int*, int*);

/* Pointers */

extern CC_P int ds_topnt(const char*, int*, int*);
extern CC_P int ds_toind(const char*, int*, int*);
extern CC_P int ids_pnt_conv(const int);
extern CC_P int ids_pnc_get(const int);
extern CC_P int ds_bind(const void**, const char*, int*);
extern CC_P int ids_update_signals(int);

/* Resources */

extern CC_P void *ids_add_res(const char*, int*, const char*, const int, void*, int*);
extern CC_P void *ids_put_res(const char*, int*, const char*, const int, void*, int*);
extern CC_P void *ids_get_res(const char*, int*, const char*, int*, int*, int*);
extern CC_P void *ids_del_res(const char*, int*, const char*, int*);
extern CC_P void *ids_rpl_res(void *ipnt, const int, void*, int*);
extern CC_P void *ids_add_res_d(const char*, int*, const char*, const int, void*, int*);
extern CC_P void *ids_put_res_d(const char*, int*, const char*, const int, void*, int*);
extern CC_P void *ids_get_res_d(const char*, int*, const char*, int*, int*, int*);
extern CC_P void *ids_del_res_d(const char*, int*, const char*, int*);
extern CC_P void *ids_rpl_res_d(void *ipnt, const int, void*, int*);
extern CC_P void *ids_pntt_res(const char*, int*, int*, int*);
extern CC_P void *ids_pntd_res(const char*, int*, int*, int*);
extern CC_P int dsadco(const char*, int*, const char*, int*);
extern CC_P int dsgtco(const char*, int*, int*, char*, int*);

/* Defining objects */

extern CC_P int dsdef(const char*, const char*, const int, const int, const char*, int*, int*);
extern CC_P int ds_cp_def(const char*, int*, const char*, int*, const int, int*);
extern CC_P int dslike(const char*, const char*, const int, int*, int*);
extern CC_P int dsdefl(const char*, int*, const int, const char*, int*);

/* Object info */

extern CC_P int dschek(const char*, int*, int*);
extern CC_P int ids_handle(const char*);
extern CC_P int ids_handle_nt(const char*);
extern CC_P int ids_handle_abs(const char*, int*);
extern CC_P int dsiv(const char*, int*, int*);
extern CC_P int dsivid(const char*, const char*, int*, int*, int*);
extern CC_P int ids_size(const char*, int*);
extern CC_P int ids_room(const char*, int*, int*, int*);
extern CC_P int ids_root(const char*);
extern CC_P int ids_nent(const char*, int*);
extern CC_P int ds_ainfo(const char*, int*, int*, int*);
extern CC_P int dsshow(const char*, const int, const int, int*);
extern CC_P int dsshow_obj(const char*, int*, const int, const int, int*);
extern CC_P int dsshdf(const char*, int*, const int, int*);
extern CC_P int dsshdc(const char*, int*, int*, const int, const int, int*);
extern CC_P int dsshdcc(const char*, int*, int*, const int, int*);
extern CC_P int dsinpc(const int, const int);
extern CC_P int dsinpcc(const int, const int);
extern CC_P int dsallc(int);
extern CC_P int dsallcc(int);
extern CC_P int dsallh(int);
extern CC_P int dsouph(const int, const int);
extern CC_P int dsfhdr(const char*, int*, const int, int*);
extern CC_P int dsglnk(const char*, int*, const int, char*, char*, int*, int*, int*);
extern CC_P int dsglnn(const char*, int*, char*, char*, int*, int*, int*, int*);
extern CC_P int dsgtrg(const char*, const char*, int*, int*);
extern CC_P int ds_show_header(const char*, int*, const int, const char*, int*);
extern CC_P int dssidf(const char*, const int, int*);
extern CC_P int dshdrf(const char*, int*, int*, const int, const int, int*);
extern CC_P int ds_name(const int, char*, int*);
extern CC_P int ds_show_help(const char*, int*, const int, int*);
extern CC_P int ds_rep_find(const char*, int*, int*);
extern CC_P int ids_rep_onoff(const int);
extern CC_P ds_object *ids_get_def(const char* );
extern CC_P ds_object *ids_get_def_f(const char*, int*, int* );
extern CC_P ds_object **ids_get_alldef(int*);

/* Directories */

extern CC_P int ids_mkdir(const char*, int*);
extern CC_P int ds_ln(const char*, int*, int*);
extern CC_P int ds_unlink(const char*, int*, int*);
extern CC_P int ids_cd(const char*, int*, int*);
extern CC_P int ids_cdn(const int);
extern CC_P int ids_dir_parse(const char*, char*, int*, int*);
extern CC_P int ids_pwd(char*);
extern CC_P int ids_is_dir(const char*, int*, int*);
extern CC_P int ids_parent(const int, char*, const int);

/* Interactive */

extern CC_P int ds_examine(char*);
extern CC_P void *ids_int_alias(const char*, const char*);

/* Error handling */

extern CC_P int ds_error(const char*, const char*, const int, int*);
extern CC_P int ds_print(const char*, const int);

/* Utilities */

extern CC_P int ds_print_line(const int fd, const char *text);
extern CC_P int ds_print_ncr(const int fd, const char *text);
extern CC_P int ds_namtok(const char*, const char*, const char*, int*, char*, int*, int*);
extern CC_P int ds_rdline(int, char*, int*, const int, int*);
extern CC_P int ldsexn(const char*, int*);
extern CC_P int idslen(char*);
extern CC_P int idsqna(char*);
extern CC_P int ds_get_nd(const char*, char*, char*, int*);
extern CC_P int dsshve(const char*, void (*)(float*, float*), const int);
extern CC_P int dswidg(const char*, const char*, const int, int*);
extern CC_P int ds_cl_rpn(char*);
extern CC_P int ds_dump(void*, void*, const int);
extern CC_P int ds_getarg(const int, char*);
extern CC_P int ds_int_edit(const char*, const int, const int, const char*, int*);
extern CC_P int ids_tapectl(const int, const char*, int*);
extern CC_P int dsfndl(const char*, char*, int*);
extern CC_P char *dsexna(char*);
extern CC_P char *ds_term_text(const char* ,const int);
extern CC_P float dsexnu(const char*, int*);

/* Object definition files */

extern CC_P int ds_ascii(const char*, const int, int*);


/* Classes */

extern CC_P int ds_setcl(const char*, int*, const char*, const int, int*);
extern CC_P int ds_getcl(const char*, int*, int*, int*, int*);
extern CC_P int ids_ndrivers(int*);
extern CC_P int ds_ident_driver(const int, char*, int*);
extern CC_P int ds_obj_mess(const char*, const char*, int*, int*);
extern CC_P int ids_cls_memb(char* DSclass, int*, const char*);
extern CC_P int ids_drcall(const char*, void(*)(const char*, int*, void*, int*), int*, void*);
extern CC_P int dscprg(const char*, int*, int*);
extern CC_P int ds_brdcast(const char*, const int);
extern CC_P int ids_nclass(char*);
extern CC_P int ds_action(const char*, const char*, int*, int*);
extern CC_P void *ids_draddr(const int);
extern CC_P void *ids_draddrc(char*);

/* DSPACK info */

extern CC_P int dsvers(double, double);
extern CC_P int ids_valid_address(void*, void*, void*, const int);

/* User routines */

extern CC_P int ds_user_widget(const char*, const char*, const int);

#endif
