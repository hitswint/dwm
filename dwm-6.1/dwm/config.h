/* See LICENSE file for copyright and license details. */

/* appearance */
static const char *fonts[]          = {"Sans:size=10"};
static const char dmenufont[]       = "Sans:size=10";
static const char normbordercolor[] = "#222222";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "deep sky blue";
static const char selbgcolor[]      = "#005577";
static const char selfgcolor[]      = "#eeeeee";
static const char normmarkcolor[]   = "dark green"; /*border color for marked client*/
static const char selmarkcolor[]    = "green"; /*border color for marked client on focus*/
static const char floatnormbordercolor[] = "dark red";
static const char floatselbordercolor[]  = "red";
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, 0: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

/* tagging */
static const char *tags[] = { "8", "9", "0" };

static const Rule rules[] = {
        /* xprop(1):
         *      WM_CLASS(STRING) = instance, class
         *      WM_NAME(STRING) = title
         */
        /* class      instance    title       tags mask     isfloating   monitor */
        { "Gimp",     NULL,       NULL,       0,            1,           -1 },
        { "Firefox",  NULL,       NULL,       1 << 2,       0,           -1 },
        { "Emacs",    NULL,       NULL,       1 << 1,       False,       -1 },
};

/* layout(s) */
static const int dirs[3]      = { DirHor, DirVer, DirVer }; /* tiling dirs */
static const float facts[3]   = { 1.1,    1.1,    1.1 };    /* tiling facts */
static const int nmaster      = 1;    /* number of clients in master area */
static const int resizehints  = 0;    /* 1 means respect size hints in tiled resizals */

#include "moveresize.c"
#include "maximize.c"
#include "zoomswap.c"
#include "exresize.c"

static const Layout layouts[] = {
        /* symbol     arrange function */
        { "[]=",      tile },    /* first entry is default */
        { "[M]",      monocle },
        { "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod1Mask
#define WINKEY Mod4Mask
#define TAGKEYS(KEY,TAG)                                                \
        { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
        { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
        { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
        { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

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
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]        = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *rofiruncmd[]      = { "rofi", "-show", "run", NULL };
static const char *rofidruncmd[]     = { "rofi", "-show", "drun", NULL };
static const char *rofiwincmd[]      = { "rofi", "-show", "window", NULL };
static const char *termcmd[]         = { "tabbed", "-c", "urxvt", "-embed", NULL };
static const char scratchpadname[]   = "scratchpad";
static const char *scratchpadcmd[]   = { "urxvt", "-name", scratchpadname, NULL };
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
static const char *scrotdesktopcmd[] = { "scrot.sh", NULL };
static const char *scrotbordercmd[]  = { "scrot.sh", "-bs", NULL };
static const char *scrotselectcmd[]  = { "scrot.sh", "-s", NULL };
static const char *emacscmd[]        = { "run-or-raise.sh", "emacs", "emacs", NULL };
static const char *wordcmd[]         = { "run-or-raise.sh", "WINWORD.EXE", "word.sh", NULL };
static const char *tccmd[]           = { "run-or-raise.sh", "TOTALCMD.EXE", "tc.sh", NULL };
static const char *firefoxcmd[]      = { "firefox", NULL, NULL, NULL, "Firefox-esr" };
static const char *switchcmd[]       = { "switcher", NULL };

static Key keys[] = {
        /* modifier                      key             function         argument */
        STACKKEYS(MODKEY,                                focus)
        STACKKEYS(MODKEY|ShiftMask,                      push)
        TILEKEYS(MODKEY,                                 1, 0, 0)
        TILEKEYS(MODKEY|ControlMask,                     0, 1, 0)
        TILEKEYS(MODKEY|ShiftMask,                       0, 0, 1)
        TILEKEYS(MODKEY|ControlMask|ShiftMask,           1, 1, 1)
        /* Sound cmd. */
        { 0,                             0x1008FF12,      spawn,          {.v = mutecmd } },
        { 0,                             0x1008FF11,      spawn,          {.v = volupcmd } },
        { 0,                             0x1008FF13,      spawn,          {.v = voldowncmd } },
        /* Set layout. */
        { WINKEY,                        XK_b,            togglebar,      {0} },
        { WINKEY,                        XK_space,        setlayout,      {0} },
        { WINKEY|ControlMask,            XK_space,        setlayout,      {.v = &layouts[0]} },
        { WINKEY|ShiftMask,              XK_space,        setlayout,      {.v = &layouts[1]} },
        { WINKEY|ControlMask|ShiftMask,  XK_space,        setlayout,      {.v = &layouts[2]} },
        { MODKEY|ControlMask,            XK_space,        togglefloating, {0} },
        { WINKEY,                        XK_minus,        incnmaster,     {.i = -1 } },
        { WINKEY,                        XK_equal,        incnmaster,     {.i = +1 } },
        { WINKEY,                        XK_1,            setnmaster,     {.i = 1} },
        { WINKEY,                        XK_2,            setnmaster,     {.i = 2} },
        { WINKEY,                        XK_3,            setnmaster,     {.i = 3} },
        { WINKEY,                        XK_4,            setnmaster,     {.i = 4} },
        { WINKEY,                        XK_5,            setnmaster,     {.i = 5} },
        { WINKEY,                        XK_6,            setnmaster,     {.i = 6} },
        { WINKEY,                        XK_7,            setnmaster,     {.i = 7} },
        { WINKEY,                        XK_8,            setnmaster,     {.i = 8} },
        { WINKEY,                        XK_9,            setnmaster,     {.i = 9} },
        { WINKEY,                        XK_0,            setnmaster,     {.i = 0} },
        { WINKEY,                        XK_backslash,    setdirs,        {.v = (int[]){ DirHor, DirVer, DirVer } } },
        { WINKEY|ShiftMask,              XK_backslash,    setdirs,        {.v = (int[]){ DirVer, DirHor, DirHor } } },
        /* Cmd. */
        { WINKEY,                        XK_s,            spawn,          {.v = slockcmd } },
        { WINKEY,                        XK_Return,       spawn,          {.v = termcmd } },
        { WINKEY,                        XK_r,            spawn,          {.v = rofiruncmd } },
        { WINKEY|ShiftMask,              XK_r,            spawn,          {.v = rofidruncmd } },
        { MODKEY|ControlMask,            XK_Tab,          spawn,          {.v = rofiwincmd } },
        { MODKEY|ControlMask,            XK_grave,        spawn,          {.v = switchcmd } },
        { WINKEY,                        XK_d,            spawn,          {.v = dmenucmd } },
        { MODKEY|ShiftMask,              XK_space,        spawn,          {.v = emacscmd } },
        { WINKEY,                        XK_w,            spawn,          {.v = wordcmd } },
        { WINKEY,                        XK_e,            spawn,          {.v = tccmd } },
        { WINKEY,                        XK_f,            runorraise,     {.v = firefoxcmd } },
        { WINKEY,                        XK_apostrophe,   togglescratch,  {.v = scratchpadcmd } },
        /* Maximize. */
        { WINKEY,                        XK_semicolon,    toggleverticalmax, {0} },
        { WINKEY|ShiftMask,              XK_semicolon,    togglehorizontalmax, {0} },
        { WINKEY|ControlMask,            XK_semicolon,    togglemaximize,      {0} },
        /* Moveresize. */
        { WINKEY,                        XK_k,            moveresize,     {.v = "0x -50y 0w 0h"} },
        { WINKEY,                        XK_j,            moveresize,     {.v = "0x 50y 0w 0h"} },
        { WINKEY,                        XK_h,            moveresize,     {.v = "-50x 0y 0w 0h"} },
        { WINKEY,                        XK_l,            moveresize,     {.v = "50x 0y 0w 0h"} },
        { WINKEY|ShiftMask,              XK_k,            moveresize,     {.v = "0x 0y 0w -50h"} },
        { WINKEY|ShiftMask,              XK_j,            moveresize,     {.v = "0x 0y 0w 50h"} },
        { WINKEY|ShiftMask,              XK_h,            moveresize,     {.v = "0x 0y -50w 0h"} },
        { WINKEY|ShiftMask,              XK_l,            moveresize,     {.v = "0x 0y 50w 0h"} },
        { WINKEY|ControlMask,            XK_k,            explace,        {.ui = EX_N  }},
        { WINKEY|ControlMask,            XK_j,            explace,        {.ui = EX_S  }},
        { WINKEY|ControlMask,            XK_h,            explace,        {.ui = EX_W  }},
        { WINKEY|ControlMask,            XK_l,            explace,        {.ui = EX_E  }},
        /* Exresize. */
        { WINKEY,                        XK_bracketleft,  exresize,       {.v = (int []){ -50,   0 }}},
        { WINKEY,                        XK_bracketright, exresize,       {.v = (int []){  50,   0 }}},
        { WINKEY|ShiftMask,              XK_bracketleft,  exresize,       {.v = (int []){   0, -50 }}},
        { WINKEY|ShiftMask,              XK_bracketright, exresize,       {.v = (int []){   0,  50 }}},
        /* Mark. */
        { WINKEY,                        XK_m,            swapfocus,      {0} },
        { WINKEY|ShiftMask,              XK_m,            swapclient,     {0} },
        { WINKEY|ControlMask,            XK_m,            togglemark,     {0} },
        /* View. */
        { MODKEY,                        XK_space,        zoom,           {0} },
        { MODKEY,                        XK_Escape,       view,           {0} },
        { MODKEY,                        XK_grave,        view,           {0} },
        { MODKEY,                        XK_F4,           killclient,     {0} },
        { WINKEY,                        XK_q,            killclient,     {0} },
        { WINKEY|ShiftMask,              XK_q,            killunsel,      {0} },
        { WINKEY|ControlMask,            XK_q,            quit,           {0} },
        { MODKEY,                        XK_7,            view,           {.ui = ~0 } },
        { MODKEY|ShiftMask,              XK_7,            tag,            {.ui = ~0 } },
        TAGKEYS(                         XK_8,            0)
        TAGKEYS(                         XK_9,            1)
        TAGKEYS(                         XK_0,            2)
        /* Focus adjacent tag. */
        { MODKEY,                        XK_minus,        viewtoleft,       {0} },
        { MODKEY,                        XK_equal,        viewtoright,      {0} },
        { MODKEY|ShiftMask,              XK_minus,        tagtoleft,        {0} },
        { MODKEY|ShiftMask,              XK_equal,        tagtoright,       {0} },
        /* Multi monitors. */
        { WINKEY,                        XK_grave,        focusmon,       {.i = -1 } },
        { WINKEY,                        XK_Tab,          focusmon,       {.i = +1 } },
        { WINKEY|ShiftMask,              XK_grave,        tagmon,         {.i = -1 } },
        { WINKEY|ShiftMask,              XK_Tab,          tagmon,         {.i = +1 } },
        /* Scrot. */
        { WINKEY,                        XK_p,            spawn,          {.v = scrotselectcmd } },
        { WINKEY|ShiftMask,              XK_p,            spawn,          {.v = scrotbordercmd } },
        { WINKEY|ControlMask,            XK_p,            spawn,          {.v = scrotdesktopcmd } },
        /* MPD setup. */
        { WINKEY,                        XK_slash,        spawn,          {.v = mpdtogglecmd } },
        { WINKEY,                        XK_comma,        spawn,          {.v = mpdseekminuscmd } },
        { WINKEY,                        XK_period,       spawn,          {.v = mpdseekpluscmd } },
        { WINKEY|ShiftMask,              XK_slash,        spawn,          {.v = mpdstopcmd } },
        { WINKEY|ShiftMask,              XK_comma,        spawn,          {.v = mpdprevcmd } },
        { WINKEY|ShiftMask,              XK_period,       spawn,          {.v = mpdnextcmd } },
        { WINKEY|ControlMask,            XK_comma,        spawn,          {.v = mpdvoldowncmd } },
        { WINKEY|ControlMask,            XK_period,       spawn,          {.v = mpdvolupcmd } },
        /* Mouse */
        { WINKEY|MODKEY,                 XK_h,            spawn,          SHCMD("exec xdotool mousemove_relative -- -15 0") },
        { WINKEY|MODKEY,                 XK_l,            spawn,          SHCMD("exec xdotool mousemove_relative 15 0") },
        { WINKEY|MODKEY,                 XK_k,            spawn,          SHCMD("exec xdotool mousemove_relative -- 0 -15") },
        { WINKEY|MODKEY,                 XK_j,            spawn,          SHCMD("exec xdotool mousemove_relative 0 15") },
        { WINKEY,                        XK_u,            spawn,          SHCMD("exec xdotool click 1") },
        { WINKEY,                        XK_i,            spawn,          SHCMD("exec xdotool click 2") },
        { WINKEY,                        XK_o,            spawn,          SHCMD("exec xdotool click 3") },
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
