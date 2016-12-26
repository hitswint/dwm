/* See LICENSE file for copyright and license details. */

/* appearance */
/* static const char font[]            = "DejaVu Sans Mono 10"; */
static const char font[]            = "Sans 10";
static const char normbordercolor[] = "#222222";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#aaaaaa";
static const char selbordercolor[]  = "deep sky blue";
static const char selbgcolor[]      = "#222222";
static const char selfgcolor[]      = "white";
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const Bool showsystray       = True;     /* False means no systray */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
static const char *tags[] = { "8", "9", "0" };

static const Rule rules[] = {
  /* class      instance    title       tags mask     isfloating   monitor */
  { "Firefox",  NULL,       NULL,     1 << 2,       False,       -1 },
  { "Emacs",    NULL,       NULL,     1 << 1,       False,       -1 },
};

/* layout(s) */
static const int dirs[3]      = { DirHor, DirVer, DirVer }; /* tiling dirs */
/* static const float facts[3]   = { 1.1,    1.1,    1.1 };    /\* tiling facts *\/ */
static const float facts[3]   = { 1.1,    1.1,    1.1 };    /* tiling facts */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

#include <maximize.c>
#include <exresize.c>

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "[]=",      tile },    /* first entry is default */
  { "[M]",      monocle },
  { "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod1Mask
#define MOD4 Mod4Mask
#define TAGKEYS(KEY,TAG)                                                \
  { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
  { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define STACKKEYS(MOD,ACTION)                                   \
  { MOD, XK_j,     ACTION##stack, {.i = INC(+1) } },            \
        { MOD, XK_k,     ACTION##stack, {.i = INC(-1) } },      \
        { MOD, XK_Tab,   ACTION##stack, {.i = PREVSEL } },      \
        { MOD, XK_1,     ACTION##stack, {.i = 0 } },            \
        { MOD, XK_2,     ACTION##stack, {.i = 1 } },            \
        { MOD, XK_3,     ACTION##stack, {.i = 2 } },            \
        { MOD, XK_4,     ACTION##stack, {.i = 3 } },            \
        { MOD, XK_5,     ACTION##stack, {.i = 4 } },            \
        { MOD, XK_6,     ACTION##stack, {.i = -1 } },           \

#define TILEKEYS(MOD,G,M,S)                                             \
  { MOD, XK_h, setfacts, {.v = (float[]){ INC(G * -0.15), INC(M * -0.30), INC(S * -0.30) } } }, \
  { MOD, XK_l, setfacts, {.v = (float[]){ INC(G * +0.15), INC(M * +0.30), INC(S * +0.30) } } }, \
  { MOD, XK_r, setdirs,  {.v = (int[])  { INC(G * +1),   INC(M * +1),   INC(S * +1) } } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[]            = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]             = { "tabbed", "-c", "urxvt", "-embed", NULL };
static const char *mutecmd[]             = { "amixer", "-q", "sset", "Master", "toggle", NULL };
static const char *volupcmd[]            = { "amixer", "-q", "sset", "Master", "5-", "unmute", NULL };
static const char *voldowncmd[]          = { "amixer", "-q", "sset", "Master", "5+", "unmute", NULL };
static const char *mpdtogglecmd[]        = { "mpc", "toggle", NULL };
static const char *mpdstopcmd[]          = { "mpc", "stop", NULL };
static const char *mpdprevcmd[]          = { "mpc", "prev", NULL };
static const char *mpdnextcmd[]          = { "mpc", "next", NULL };
static const char *mpdseekpluscmd[]      = { "mpc", "seek", "+5%", NULL };
static const char *mpdseekminuscmd[]     = { "mpc", "seek", "-5%", NULL };
static const char *mpdvolupcmd[]         = { "mpc", "volume", "+10", NULL };
static const char *mpdvoldowncmd[]       = { "mpc", "volume", "-10", NULL };
/* static const char *switchcmd[]           = { "switcher", NULL }; */
static const char *zoomtoemacscmd[]      = { "xdotool", "search", "--onlyvisible", "--classname", "emacs", "windowactivate", NULL };
static const char *slockcmd[]            = { "slock", NULL };
static const char *emacs[]               = { "emacs", NULL, NULL, NULL, "Emacs" };
static const char *firefox[]             = { "firefox", NULL, NULL, NULL, "Firefox" };
static const char *wps[]                 = { "wps", NULL, NULL, NULL, "Wps" };

static Key keys[] = {
  /* modifier                     key        function        argument */
  { 0,                      0x1008FF12,      spawn,          {.v = mutecmd } },
  { 0,                      0x1008FF11,      spawn,          {.v = volupcmd } },
  { 0,                      0x1008FF13,      spawn,          {.v = voldowncmd } },
  STACKKEYS(MODKEY,                          focus)
  STACKKEYS(MODKEY|ShiftMask,                push)
  TILEKEYS(MODKEY,                           1, 0, 0)
  TILEKEYS(MODKEY|ControlMask,               0, 1, 0)
  TILEKEYS(MODKEY|ShiftMask,                 0, 0, 1)
  TILEKEYS(MODKEY|ControlMask|ShiftMask,     1, 1, 1)
  { MOD4,                         XK_b,      togglebar,      {0} },
  { MOD4,                         XK_p,      setlayout,      {.v = &layouts[0]} },
  { MOD4,                         XK_m,      setlayout,      {.v = &layouts[1]} },
  { MOD4,                         XK_n,      setlayout,      {.v = &layouts[2]} },
  { MODKEY,                       XK_space,  setlayout,      {0} },
  { MOD4,                         XK_i,      incnmaster,     {.i = +1 } },
  { MOD4,                         XK_o,      incnmaster,     {.i = -1 } },
  { MOD4,                         XK_comma,  setdirs,        {.v = (int[]){ DirHor, DirVer, DirVer } } },
  { MOD4,                         XK_period, setdirs,        {.v = (int[]){ DirVer, DirHor, DirHor } } },
  { MOD4,                         XK_l,      spawn,          {.v = slockcmd } },
  { MOD4,                         XK_Return, spawn,          {.v = termcmd } },
  { MOD4,                         XK_r,      spawn,          {.v = dmenucmd } },
  { MOD4,                         XK_e,      runorraise,     {.v = emacs } },
  { MOD4,                         XK_f,      runorraise,     {.v = firefox } },
  { MOD4,                         XK_w,      runorraise,     {.v = wps } },
  { MOD4,                         XK_q,      quit,           {0} },
  { MODKEY,                       XK_grave,  view,           {0} },
  /* { MOD4|ControlMask,             XK_m,      spawn,          {.v = switchcmd } }, */
  { MODKEY,                       XK_o,      zoom,           {0} },
  { MODKEY|ShiftMask,             XK_o,      spawn,          {.v = zoomtoemacscmd } },
  { MODKEY,                       XK_F4,     killclient,     {0} },
  { MODKEY,                       XK_7,      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_7,      tag,            {.ui = ~0 } },
  { MOD4,                         XK_k,      viewtoleft,     {0} },
  { MOD4,                         XK_j,      viewtoright,    {0} },
  { MOD4|ShiftMask,               XK_k,      tagtoleft,      {0} },
  { MOD4|ShiftMask,               XK_j,      tagtoright,     {0} },
  { MOD4,                         XK_grave,  focusmon,       {.i = -1 } },
  { MOD4,                         XK_Tab,    focusmon,       {.i = +1 } },
  { MOD4|ShiftMask,               XK_grave,  tagmon,         {.i = -1 } },
  { MOD4|ShiftMask,               XK_Tab,    tagmon,         {.i = +1 } },
  TAGKEYS(                        XK_8,                      0)
  TAGKEYS(                        XK_9,                      1)
  TAGKEYS(                        XK_0,                      2)
  /* MPD setup */
  { MODKEY|ControlMask,           XK_space,  spawn,          {.v = mpdtogglecmd } },
  { MODKEY|ControlMask,           XK_Left,   spawn,          {.v = mpdseekminuscmd } },
  { MODKEY|ControlMask,           XK_Right,  spawn,          {.v = mpdseekpluscmd } },
  { MODKEY|ControlMask,           XK_Up,     spawn,          {.v = mpdvolupcmd } },
  { MODKEY|ControlMask,           XK_Down,   spawn,          {.v = mpdvoldowncmd } },
  { MOD4|ControlMask,             XK_Left,   spawn,          {.v = mpdprevcmd } },
  { MOD4|ControlMask,             XK_Right,  spawn,          {.v = mpdnextcmd } },
  { MOD4|ControlMask,             XK_space,  spawn,          {.v = mpdstopcmd } },
  /* maximize */
  { MODKEY|ShiftMask,             XK_i,      togglehorizontalmax, {0} },
  { MODKEY,                       XK_i,      toggleverticalmax, {0} },
  /* exresize */
  { MODKEY,                       XK_equal,  exresize,       {.v = (int []){  50,   0 }}},
  { MODKEY,                       XK_minus,  exresize,       {.v = (int []){ -50,   0 }}},
  { MODKEY|ShiftMask,             XK_equal,  exresize,       {.v = (int []){   0,  50 }}},
  { MODKEY|ShiftMask,             XK_minus,  exresize,       {.v = (int []){   0, -50 }}},
  /* { MODKEY|MOD4|ShiftMask,             XK_5,   exresize,               {.v = (int []){  25,  25 }}}, */
  /* { MODKEY|MOD4|ShiftMask|ControlMask, XK_5,   exresize,               {.v = (int []){ -25, -25 }}}, */
  { MOD4,                         XK_Up,     explace,        {.ui = EX_N  }},
  { MOD4,                         XK_Down,   explace,        {.ui = EX_S  }},
  { MOD4,                         XK_Left,   explace,        {.ui = EX_W  }},
  { MOD4,                         XK_Right,  explace,        {.ui = EX_E  }},
  { MOD4|ShiftMask,               XK_Up,     explace,        {.ui = EX_NW }},
  { MOD4|ShiftMask,               XK_Down,   explace,        {.ui = EX_SE }},
  { MOD4|ShiftMask,               XK_Left,   explace,        {.ui = EX_SW }},
  { MOD4|ShiftMask,               XK_Right,  explace,        {.ui = EX_NE }},
  /* { MODKEY|MOD4,                       XK_5,   explace,                {.ui = EX_C  }}, */
  /* exresize里的maximize功能不好，在exresize.c中注释掉 */
  /* { MODKEY|MOD4|ControlMask,      XK_1,              togglehorizontalexpand, {.i = +1} }, */
  /* { MODKEY|MOD4|ControlMask,      XK_comma,          togglehorizontalexpand, {.i =  0} }, */
  /* { MODKEY|MOD4|ControlMask,      XK_2,              togglehorizontalexpand, {.i = -1} }, */
  /* { MODKEY|MOD4|ControlMask,      XK_3,              toggleverticalexpand,   {.i = +1} }, */
  /* { MODKEY|MOD4|ControlMask,      XK_period,         toggleverticalexpand,   {.i =  0} }, */
  /* { MODKEY|MOD4|ControlMask,      XK_4,              toggleverticalexpand,   {.i = -1} }, */
  /* { MODKEY|MOD4|ControlMask,      XK_9,              togglemaximize,         {.i = -1} }, */
  /* { MODKEY|MOD4|ControlMask,      XK_7,              togglemaximize,         {.i = +1} }, */
  /* { MODKEY|MOD4|ControlMask,      XK_5,              togglemaximize,         {.i =  0} }, */
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
  { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,          0,              Button2,        zoom,           {0} },
  { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
