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
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
static const char *tags[] = { "8", "9", "0"};

static const Rule rules[] = {
  /* class      instance    title       tags mask     isfloating   monitor */
  { "Firefox",  NULL,       NULL,     1 << 1,       False,       -1 },
  { "Emacs",    NULL,       NULL,     1 << 2,       False,       -1 },
};

/* layout(s) */
static const int dirs[3]      = { DirHor, DirVer, DirVer }; /* tiling dirs */
/* static const float facts[3]   = { 1.1,    1.1,    1.1 };    /\* tiling facts *\/ */
static const float facts[3]   = { 1.1,    1.1,    1.1 };    /* tiling facts */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "[]=",      tile },    /* first entry is default */
  { "[M]",      monocle },
  { "><>",      NULL },    /* no layout function means floating behavior */
};

#include <maximize.c>
#include <moveresize.c>

/* key definitions */
#define MODKEY Mod1Mask
#define MOD4 Mod4Mask
#define TAGKEYS(KEY,TAG)                                                \
  { MODKEY,                    KEY,      view,           {.ui = 1 << TAG} }, \
        { MOD4|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
        { MOD4,                       KEY,      tag,            {.ui = 1 << TAG} }, \
        { MOD4|ShiftMask,             KEY,      toggletag,      {.ui = 1 << TAG} },

#define STACKKEYS(MOD,ACTION)                                   \
  { MOD, XK_j,     ACTION##stack, {.i = INC(+1) } },            \
        { MOD, XK_k,     ACTION##stack, {.i = INC(-1) } },      \
        { MOD, XK_1,     ACTION##stack, {.i = 0 } },            \
        { MOD, XK_2,     ACTION##stack, {.i = 1 } },            \
        { MOD, XK_3,     ACTION##stack, {.i = 2 } },            \
        { MOD, XK_4,     ACTION##stack, {.i = 3 } },            \
        { MOD, XK_5,     ACTION##stack, {.i = 4 } },            \
        { MOD, XK_6,     ACTION##stack, {.i = -1 } },

#define STACKKEYS1(MOD,ACTION)                          \
  { MOD, XK_i,     ACTION##stack, {.i = PREVSEL } },

#define TILEKEYS(MOD,G,M,S)                                             \
  { MOD, XK_h, setfacts, {.v = (float[]){ INC(G * -0.15), INC(M * -0.30), INC(S * -0.30) } } }, \
  { MOD, XK_l, setfacts, {.v = (float[]){ INC(G * +0.15), INC(M * +0.30), INC(S * +0.30) } } }, \
  { MOD, XK_r, setdirs,  {.v = (int[])  { INC(G * +1),   INC(M * +1),   INC(S * +1) } } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[]            = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]             = { "urxvt", NULL };
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
/* static const char *switchtoemacscmd[]    = { "xdotool", "search", "--onlyvisible", "--classname", "emacs", "windowactivate", NULL }; */
static const char *lockcmd[]             = { "slock", NULL };
static const char *emacs[]               = { "emacs", NULL, NULL, NULL, "Emacs" };
static const char *firefox[]             = { "firefox", NULL, NULL, NULL, "Firefox" };

static Key keys[] = {
  /* modifier                     key        function        argument */
  { 0,                      0x1008FF12,      spawn,          {.v = mutecmd } },
  { 0,                      0x1008FF11,      spawn,          {.v = volupcmd } },
  { 0,                      0x1008FF13,      spawn,          {.v = voldowncmd } },
  STACKKEYS(MODKEY,                          focus)
  STACKKEYS(MOD4,                            push)
  STACKKEYS1(MODKEY,                         focus)
  /* STACKKEYS1(MOD4,                            push) */
  TILEKEYS(MODKEY,                                           1, 0, 0)
  TILEKEYS(MODKEY|ControlMask,                               0, 1, 0)
  TILEKEYS(MODKEY|ShiftMask,                                 0, 0, 1)
  TILEKEYS(MODKEY|ControlMask|ShiftMask,                     1, 1, 1)
  { MOD4,                         XK_b,      togglebar,      {0} },
  { MOD4,                         XK_p,      setlayout,      {.v = &layouts[0]} },
  { MOD4,                         XK_m,      setlayout,      {.v = &layouts[1]} },
  { MOD4,                         XK_n,      setlayout,      {.v = &layouts[2]} },
  { MODKEY,                       XK_space,  setlayout,      {0} },
  { MOD4,                         XK_i,      incnmaster,     {.i = +1 } },
  { MOD4,                         XK_o,      incnmaster,     {.i = -1 } },
  /* { MOD4|ControlMask,             XK_comma,  setdirs,        {.v = (int[]){ DirHor, DirVer, DirVer } } }, */
  /* { MOD4|ControlMask,             XK_period, setdirs,        {.v = (int[]){ DirVer, DirHor, DirHor } } }, */
  { MOD4,                         XK_l,      spawn,          {.v = lockcmd } },
  { MOD4,                         XK_Return, spawn,          {.v = termcmd } },
  { MOD4,                         XK_r,      spawn,          {.v = dmenucmd } },
  { MOD4,                         XK_e,      runorraise,     {.v = emacs } },
  { MOD4,                         XK_f,      runorraise,     {.v = firefox } },
  { MOD4,                         XK_q,      quit,           {0} },
  /* { MODKEY,                       XK_m,      spawn,          {.v = switchcmd } }, */
  /* { MODKEY,                       XK_i,      spawn,          {.v = switchtoemacscmd } }, */
  { MODKEY,                       XK_o,      zoom,           {0} },
  { MODKEY,                       XK_Tab,    view,           {0} },
  { MODKEY,                       XK_F4,     killclient,     {0} },
  { MODKEY,                       XK_7,      view,           {.ui = ~0 } },
  { MOD4,                         XK_7,      tag,            {.ui = ~0 } },
  { MODKEY,                       XK_grave,  focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,             XK_grave,  focusmon,       {.i = -1 } },
  { MOD4,                         XK_grave,  tagmon,         {.i = +1 } },
  { MOD4|ShiftMask,               XK_grave,  tagmon,         {.i = -1 } },
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
  { MOD4,                         XK_comma,  togglehorizontalmax, {0} },
  { MOD4,                         XK_period, toggleverticalmax, {0} },
  /* moveresize */
  /* 可能是因为xtile或者pertag的原因，并不成功 */
  { MOD4,                         XK_equal,    moveresize,                {.v = (int []){ 0, 25, 0, 0 }}},
  { MOD4,                         XK_minus,    moveresize,                {.v = (int []){ 0, -25, 0, 0 }}},
  { MOD4|ControlMask,             XK_equal,    moveresize,                {.v = (int []){ 25, 0, 0, 0 }}},
  { MOD4|ControlMask,             XK_minus,    moveresize,                {.v = (int []){ -25, 0, 0, 0 }}},
  { MOD4,                         XK_bracketleft,    moveresize,          {.v = (int []){ 0, 0, 0, 25 }}},
  { MOD4,                         XK_bracketright,   moveresize,          {.v = (int []){ 0, 0, 0, -25 }}},
  { MOD4|ControlMask,             XK_bracketleft,    moveresize,          {.v = (int []){ 0, 0, 25, 0 }}},
  { MOD4|ControlMask,             XK_bracketright,   moveresize,          {.v = (int []){ 0, 0, -25, 0 }}},
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
