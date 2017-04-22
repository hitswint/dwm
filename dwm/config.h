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

#include "maximize.c"
#include "exresize.c"

static const Layout layouts[] = {
        /* symbol     arrange function */
        { "[]=",      tile },    /* first entry is default */
        { "[M]",      monocle },
        { "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY1 Mod1Mask
#define MODKEY4 Mod4Mask

#define TAGKEYS(KEY,TAG)                                                \
        { MODKEY1,                 KEY,      view,           {.ui = 1 << TAG} }, \
  { MODKEY1|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
  { MODKEY1|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
  { MODKEY1|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \

#define STACKKEYS(MOD,ACTION)                                   \
        { MOD, XK_j,     ACTION##stack, {.i = INC(+1) } },      \
  { MOD, XK_k,     ACTION##stack, {.i = INC(-1) } },            \
  { MOD, XK_Tab,   ACTION##stack, {.i = PREVSEL } },            \
  { MOD, XK_1,     ACTION##stack, {.i = 0 } },                  \
  { MOD, XK_2,     ACTION##stack, {.i = 1 } },                  \
  { MOD, XK_3,     ACTION##stack, {.i = 2 } },                  \
  { MOD, XK_4,     ACTION##stack, {.i = 3 } },                  \
  { MOD, XK_5,     ACTION##stack, {.i = 4 } },                  \
  { MOD, XK_6,     ACTION##stack, {.i = -1 } },                 \

#define TILEKEYS(MOD,G,M,S)                                             \
        { MOD, XK_h, setfacts, {.v = (float[]){ INC(G * -0.15), INC(M * -0.30), INC(S * -0.30) } } }, \
  { MOD, XK_l, setfacts, {.v = (float[]){ INC(G * +0.15), INC(M * +0.30), INC(S * +0.30) } } }, \
  { MOD, XK_backslash, setdirs,  {.v = (int[])  { INC(G * +1),   INC(M * +1),   INC(S * +1) } } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[]        = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]         = { "tabbed", "-c", "urxvt", "-embed", NULL };
static const char *mutecmd[]         = { "amixer", "-q", "sset", "Master", "toggle", NULL };
static const char *volupcmd[]        = { "amixer", "-q", "sset", "Master", "5-", "unmute", NULL };
static const char *voldowncmd[]      = { "amixer", "-q", "sset", "Master", "5+", "unmute", NULL };
static const char *mpdtogglecmd[]    = { "mpc", "toggle", NULL };
static const char *mpdstopcmd[]      = { "mpc", "stop", NULL };
static const char *mpdprevcmd[]      = { "mpc", "prev", NULL };
static const char *mpdnextcmd[]      = { "mpc", "next", NULL };
static const char *mpdseekpluscmd[]  = { "mpc", "seek", "+5%", NULL };
static const char *mpdseekminuscmd[] = { "mpc", "seek", "-5%", NULL };
static const char *mpdvolupcmd[]     = { "mpc", "volume", "+10", NULL };
static const char *mpdvoldowncmd[]   = { "mpc", "volume", "-10", NULL };
static const char *slockcmd[]        = { "slock", NULL };
static const char *emacscmd[]        = { "run-or-raise.sh", "emacs", "emacs", NULL };
static const char *wordcmd[]         = { "run-or-raise.sh", "WINWORD.EXE", "word.sh", NULL };
static const char *tccmd[]           = { "run-or-raise.sh", "TOTALCMD.EXE", "tc.sh", NULL };
static const char *firefoxcmd[]      = { "firefox-esr", NULL, NULL, NULL, "Firefox-esr" };
/* static const char *switchcmd[]       = { "switcher", NULL }; */

static Key keys[] = {
        /* modifier                      key              function          argument */
        STACKKEYS(MODKEY1,                                focus)
        STACKKEYS(MODKEY1|ShiftMask,                      push)
        TILEKEYS(MODKEY1,                                 1, 0, 0)
        TILEKEYS(MODKEY1|ControlMask,                     0, 1, 0)
        TILEKEYS(MODKEY1|ShiftMask,                       0, 0, 1)
        TILEKEYS(MODKEY1|ControlMask|ShiftMask,           1, 1, 1)
        /* Sound cmd. */
        { 0,                             0x1008FF12,      spawn,            {.v = mutecmd } },
        { 0,                             0x1008FF11,      spawn,            {.v = volupcmd } },
        { 0,                             0x1008FF13,      spawn,            {.v = voldowncmd } },
        /* Set layout. */
        { MODKEY4,                       XK_m,            togglebar,        {0} },
        { MODKEY4,                       XK_space,        setlayout,        {0} },
        { MODKEY4|ControlMask,           XK_space,        setlayout,        {.v = &layouts[0]} },
        { MODKEY4|ShiftMask,             XK_space,        setlayout,        {.v = &layouts[1]} },
        { MODKEY4|ControlMask|ShiftMask, XK_space,        setlayout,        {.v = &layouts[2]} },
        { MODKEY4,                       XK_n,            incnmaster,       {.i = +1 } },
        { MODKEY4,                       XK_p,            incnmaster,       {.i = -1 } },
        { MODKEY4,                       XK_backslash,    setdirs,          {.v = (int[]){ DirHor, DirVer, DirVer } } },
        { MODKEY4|ShiftMask,             XK_backslash,    setdirs,          {.v = (int[]){ DirVer, DirHor, DirHor } } },
        /* Cmd. */
        { MODKEY4,                       XK_s,            spawn,            {.v = slockcmd } },
        { MODKEY4,                       XK_Return,       spawn,            {.v = termcmd } },
        { MODKEY4,                       XK_d,            spawn,            {.v = dmenucmd } },
        { MODKEY4,                       XK_w,            spawn,            {.v = wordcmd } },
        { MODKEY4,                       XK_e,            spawn,            {.v = tccmd } },
        { MODKEY4,                       XK_f,            runorraise,       {.v = firefoxcmd } },
        /* View. */
        { MODKEY4,                       XK_q,            quit,             {0} },
        { MODKEY1,                       XK_grave,        view,             {0} },
        { MODKEY1,                       XK_Escape,       view,             {0} },
        { MODKEY1,                       XK_space,        zoom,             {0} },
        { MODKEY1|ShiftMask,             XK_space,        spawn,            {.v = emacscmd } },
        { MODKEY1,                       XK_q,            killclient,       {0} },
        { MODKEY1,                       XK_F4,           killclient,       {0} },
        { MODKEY1,                       XK_7,            view,             {.ui = ~0 } },
        { MODKEY1|ShiftMask,             XK_7,            tag,              {.ui = ~0 } },
        /* Focus adjacent tag. */
        /* { MODKEY4,                       XK_k,            viewtoleft,       {0} }, */
        /* { MODKEY4,                       XK_j,            viewtoright,      {0} }, */
        /* { MODKEY4|ShiftMask,             XK_k,            tagtoleft,        {0} }, */
        /* { MODKEY4|ShiftMask,             XK_j,            tagtoright,       {0} }, */
        /* Multi monitors. */
        { MODKEY4,                       XK_grave,        focusmon,         {.i = -1 } },
        { MODKEY4,                       XK_Tab,          focusmon,         {.i = +1 } },
        { MODKEY4|ShiftMask,             XK_grave,        tagmon,           {.i = -1 } },
        { MODKEY4|ShiftMask,             XK_Tab,          tagmon,           {.i = +1 } },
        TAGKEYS(                         XK_8,            0)
        TAGKEYS(                         XK_9,            1)
        TAGKEYS(                         XK_0,            2)
        /* MPD setup. */
        { MODKEY4,                       XK_slash,        spawn,            {.v = mpdtogglecmd } },
        { MODKEY4,                       XK_comma,        spawn,            {.v = mpdseekminuscmd } },
        { MODKEY4,                       XK_period,       spawn,            {.v = mpdseekpluscmd } },
        { MODKEY4|ShiftMask,             XK_slash,        spawn,            {.v = mpdstopcmd } },
        { MODKEY4|ShiftMask,             XK_comma,        spawn,            {.v = mpdprevcmd } },
        { MODKEY4|ShiftMask,             XK_period,       spawn,            {.v = mpdnextcmd } },
        { MODKEY4|ControlMask,           XK_comma,        spawn,            {.v = mpdvoldowncmd } },
        { MODKEY4|ControlMask,           XK_period,       spawn,            {.v = mpdvolupcmd } },
        /* Maximize. */
        { MODKEY1|ControlMask,           XK_space,        toggleverticalmax, {0} },
        { MODKEY1|ControlMask|ShiftMask, XK_space,        togglehorizontalmax, {0} },
        /* Exresize. */
        { MODKEY4|ControlMask,           XK_h,            exresize,         {.v = (int []){ -50,   0 }}},
        { MODKEY4|ControlMask,           XK_l,            exresize,         {.v = (int []){  50,   0 }}},
        { MODKEY4|ControlMask,           XK_k,            exresize,         {.v = (int []){   0, -50 }}},
        { MODKEY4|ControlMask,           XK_j,            exresize,         {.v = (int []){   0,  50 }}},
        /* { MODKEY4,                       XK_bracketleft,  exresize,         {.v = (int []){ -25, -25 }}}, */
        /* { MODKEY4,                       XK_bracketright, exresize,         {.v = (int []){  25,  25 }}}, */
        { MODKEY4,                       XK_k,            explace,          {.ui = EX_N  }},
        { MODKEY4,                       XK_j,            explace,          {.ui = EX_S  }},
        { MODKEY4,                       XK_h,            explace,          {.ui = EX_W  }},
        { MODKEY4,                       XK_l,            explace,          {.ui = EX_E  }},
        { MODKEY4|ShiftMask,             XK_k,            explace,          {.ui = EX_NW }},
        { MODKEY4|ShiftMask,             XK_j,            explace,          {.ui = EX_SE }},
        { MODKEY4|ShiftMask,             XK_h,            explace,          {.ui = EX_SW }},
        { MODKEY4|ShiftMask,             XK_l,            explace,          {.ui = EX_NE }},
        /* { MODKEY1|MODKEY4,                       XK_5,   explace,                {.ui = EX_C  }}, */
        /* exresize里的maximize功能不好，在exresize.c中注释掉。 */
        /* { MODKEY4,      XK_,              togglehorizontalexpand, {.i = +1} }, */
        /* { MODKEY4,      XK_,              togglehorizontalexpand, {.i =  0} }, */
        /* { MODKEY4,      XK_,              togglehorizontalexpand, {.i = -1} }, */
        /* { MODKEY4,      XK_,              toggleverticalexpand,   {.i = +1} }, */
        /* { MODKEY4,      XK_,              toggleverticalexpand,   {.i =  0} }, */
        /* { MODKEY4,      XK_,              toggleverticalexpand,   {.i = -1} }, */
        /* { MODKEY4,      XK_,              togglemaximize,         {.i = -1} }, */
        /* { MODKEY4,      XK_,              togglemaximize,         {.i = +1} }, */
        /* { MODKEY4,      XK_,              togglemaximize,         {.i =  0} }, */
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
        /* click                event mask      button          function        argument */
        { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
        { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
        { ClkWinTitle,          0,              Button2,        zoom,           {0} },
        { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
        { ClkClientWin,         MODKEY1,        Button1,        movemouse,      {0} },
        { ClkClientWin,         MODKEY1,        Button2,        togglefloating, {0} },
        { ClkClientWin,         MODKEY1,        Button3,        resizemouse,    {0} },
        { ClkTagBar,            0,              Button1,        view,           {0} },
        { ClkTagBar,            0,              Button3,        toggleview,     {0} },
        { ClkTagBar,            MODKEY1,        Button1,        tag,            {0} },
        { ClkTagBar,            MODKEY1,        Button3,        toggletag,      {0} },
};
