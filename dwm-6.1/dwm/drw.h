/* See LICENSE file for copyright and license details. */
#define DRW_FONT_CACHE_SIZE 32

typedef struct {
	unsigned long pix;
	XftColor rgb;
} Clr;

typedef struct {
	Cursor cursor;
} Cur;

typedef struct {
	Display *dpy;
	int ascent;
	int descent;
	unsigned int h;
	XftFont *xfont;
	FcPattern *pattern;
} Fnt;

typedef struct {
	Clr *fg;
	Clr *bg;
	Clr *border;
        Clr *floatborder;
	Clr *mark;
} ClrScheme;

typedef struct {
	unsigned int w, h;
	Display *dpy;
	int screen;
	Window root;
	Drawable drawable;
	Picture picture;
	GC gc;
	ClrScheme *scheme;
	size_t fontcount;
	Fnt *fonts[DRW_FONT_CACHE_SIZE];
} Drw;

typedef struct {
	unsigned int w;
	unsigned int h;
} Extnts;

/* Drawable abstraction */
Drw *drw_create(Display *, int, Window, unsigned int, unsigned int);
void drw_resize(Drw *, unsigned int, unsigned int);
void drw_free(Drw *);

/* Fnt abstraction */
Fnt *drw_font_create(Drw *, const char *);
void drw_load_fonts(Drw *, const char *[], size_t);
void drw_font_free(Fnt *);
void drw_font_getexts(Fnt *, const char *, unsigned int, Extnts *);
unsigned int drw_font_getexts_width(Fnt *, const char *, unsigned int);

/* Colour abstraction */
Clr *drw_clr_create(Drw *, const char *);
void drw_clr_free(Clr *);

/* Cursor abstraction */
Cur *drw_cur_create(Drw *, int);
void drw_cur_free(Drw *, Cur *);

/* Drawing context manipulation */
void drw_setfont(Drw *, Fnt *);
void drw_setscheme(Drw *, ClrScheme *);

Picture drw_picture_create_resized(Drw *drw, char *src, unsigned int src_w, unsigned int src_h, unsigned int dst_w, unsigned int dst_h);

/* Drawing functions */
void drw_rect(Drw *, int, int, unsigned int, unsigned int, int, int, int);
int drw_text(Drw *, int, int, unsigned int, unsigned int, const char *, int);
void drw_pic(Drw *drw, int x, int y, unsigned int w, unsigned int h, Picture pic);

/* Map functions */
void drw_map(Drw *, Window, int, int, unsigned int, unsigned int);
