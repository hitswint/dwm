/* See LICENSE file for copyright and license details. */

/* appearance */
static const char *fonts[]          = {"WenQuanYi Micro Hei:size=12"};
static const char dmenufont[]       = "WenQuanYi Micro Hei:size=12";
static const char normbordercolor[] = "#222222";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#dddddd";
static const char selbordercolor[]  = "DeepSkyBlue";
static const char selbgcolor[]      = "#005577";
static const char selfgcolor[]      = "#ffffff";
static const char normmarkcolor[]   = "dark green"; /*border color for marked client*/
static const char selmarkcolor[]    = "green"; /*border color for marked client on focus*/
static const char floatnormbordercolor[] = "dark red";
static const char floatselbordercolor[]  = "red";
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 1;   /* systray spacing */
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
        { "Wps",      NULL,       NULL,       0,            False,       -1 },
        { "Et",       NULL,       NULL,       0,            False,       -1 },
        { "Wpp",      NULL,       NULL,       0,            False,       -1 },
        { "Wine",     NULL,       NULL,       0,            False,       -1 },
        { "firefox",  NULL,       NULL,       1 << 2,       False,        1 },
        { "Emacs",    "emacs",    NULL,       1 << 1,       False,        0 },
        { "Emacs",    "ec_float", NULL,       0,            True,        -1 },
        { "copyq",    NULL,       NULL,       0,            True,        -1 },
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

#define TAGKEYS_MON(KEY,TAG)                                            \
        { WINKEY,                       KEY,      view_mon,       {.ui = 1 << TAG} }, \
        { WINKEY|ControlMask,           KEY,      toggleview_mon, {.ui = 1 << TAG} }, \
        { WINKEY|ShiftMask,             KEY,      tag_mon,        {.ui = 1 << TAG} }, \
        { WINKEY|ControlMask|ShiftMask, KEY,      toggletag_mon,  {.ui = 1 << TAG} },

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

#define STACKKEYS_MON(MOD,ACTION)                               \
        { MOD, XK_1,     ACTION##stack_mon, {.i = 0 } },        \
  { MOD, XK_2,     ACTION##stack_mon, {.i = 1 } },              \
  { MOD, XK_3,     ACTION##stack_mon, {.i = 2 } },              \
  { MOD, XK_4,     ACTION##stack_mon, {.i = 3 } },              \
  { MOD, XK_5,     ACTION##stack_mon, {.i = 4 } },              \
  { MOD, XK_6,     ACTION##stack_mon, {.i = -1 } },             \

#define TILEKEYS(MOD,G,M,S)                                             \
        { MOD, XK_h, setfacts, {.v = (float[]){ INC(G * -0.15), INC(M * -0.30), INC(S * -0.30) } } }, \
  { MOD, XK_l, setfacts, {.v = (float[]){ INC(G * +0.15), INC(M * +0.30), INC(S * +0.30) } } }, \
  { MOD, XK_backslash, setdirs,  {.v = (int[])  { INC(G * +1),   INC(M * +1),   INC(S * +1) } } }, \

#define TILEBUTTONS(MOD,G,M,S)                                          \
        { ClkWinTitle, MOD, Button1, setfacts, {.v = (float[]){ INC(G * -0.15), INC(M * -0.30), INC(S * -0.30) } } }, \
  { ClkWinTitle, MOD, Button3, setfacts, {.v = (float[]){ INC(G * +0.15), INC(M * +0.30), INC(S * +0.30) } } }, \
  { ClkWinTitle, MOD, Button2, setdirs,  {.v = (int[])  { INC(G * +1),   INC(M * +1),   INC(S * +1) } } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } } \

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]        = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *rofiruncmd[]      = { "rofi.sh", "rnow", "-run-command", "zsh -is eval '{cmd}'", "-show", "run", NULL };
static const char *rofidruncmd[]     = { "rofi.sh", "-show", "drun", NULL };
static const char *rofiwincmd[]      = { "rofi.sh", "-show", "window", NULL };
static const char *rofisshcmd[]      = { "rofi.sh", "-show", "ssh", NULL };
static const char *rofipowercmd[]    = { "rofi-power.sh", NULL };
static const char *roficlipcmd[]     = { "rofi-clip.sh", NULL };
static const char *roficalccmd[]     = { "rofi-calc.sh", NULL };
static const char *rofitranscmd[]    = { "rofi-trans.sh", NULL };
static const char *rofipasscmd[]     = { "rofi-pass.sh", NULL };
static const char *keynavcmd[]       = { "keynav.sh", NULL };
static const char *termcmd[]         = { "urxvt", NULL };
static const char scratchpadname[]   = "scratchpad";
static const char *scratchpadcmd[]   = { "urxvt", "-name", scratchpadname, NULL };
static const char *backlightdowncmd[]= { "backlight.sh", "down", NULL };
static const char *backlightupcmd[]  = { "backlight.sh", "up", NULL };
static const char *backlightoffcmd[] = { "backlight.sh", "off", NULL };
static const char *amixervoldowncmd[]= { "amixer", "-q", "sset", "Master", "5%-", "unmute", NULL };
static const char *amixervolupcmd[]  = { "amixer", "-q", "sset", "Master", "5%+", "unmute", NULL };
static const char *amixermutecmd[]   = { "amixer", "-q", "sset", "Master", "toggle", NULL };
static const char *pactlvoldowncmd[] = { "pactl", "set-sink-volume", "0", "-5%", NULL };
static const char *pactlvolupcmd[]   = { "pactl", "set-sink-volume", "0", "+5%", NULL };
static const char *pactlmutecmd[]    = { "pactl", "set-sink-mute", "0", "toggle", NULL };
static const char *pactlmicmutecmd[] = { "pactl", "set-source-mute", "1", "toggle", NULL };
static const char *mpdtogglecmd[]    = { "mpc", "toggle", NULL };
static const char *mpdstopcmd[]      = { "mpc", "stop", NULL };
static const char *mpdprevcmd[]      = { "mpc", "prev", NULL };
static const char *mpdnextcmd[]      = { "mpc", "next", NULL };
static const char *mpdseekpluscmd[]  = { "mpc", "seek", "+5%", NULL };
static const char *mpdseekminuscmd[] = { "mpc", "seek", "-5%", NULL };
static const char *mpdvolupcmd[]     = { "mpc", "volume", "+10", NULL };
static const char *mpdvoldowncmd[]   = { "mpc", "volume", "-10", NULL };
static const char *slockcmd[]        = { "slock", NULL };
static const char *ssdesktopcmd[]    = { "screenshot.sh", "d", NULL };
static const char *ssbordercmd[]     = { "screenshot.sh", "w", NULL };
static const char *ssselectcmd[]     = { "screenshot.sh", "s", NULL };
static const char *ssocrengcmd[]     = { "screenshot_ocr.sh", "eng", NULL };
static const char *ssocrchicmd[]     = { "screenshot_ocr.sh", "chi_sim", NULL };
static const char *emacsclientcmd[]  = { "run-or-raise.sh", "emacs", "ec.sh", NULL };
static const char *firefoxcmd[]      = { "firefox", NULL, NULL, NULL, "firefox" };
static const char *switchcmd[]       = { "switcher", NULL };
static const char *ec_dictcmd[]      = { "ec_dict.sh", NULL };
static const char *ec_examplecmd[]   = { "ec_example.sh", NULL };
static const char *ec_killringcmd[]  = { "ec_killring.sh", NULL };
static const char *ec_ffbmkscmd[]    = { "ec_ffbmks.sh", NULL };
static const char *ec_ffhistcmd[]    = { "ec_ffhist.sh", NULL };
static const char *ec_editcmd[]      = { "external-edit", "-e", "emacsclient", NULL };
static const char *ec_editselcmd[]   = { "external-edit", "-s", "-e", "emacsclient", NULL };
static const char *xclipcopycmd[]    = { "xclipcopy.sh", NULL };
static const char *xclippastecmd[]   = { "xclippaste.sh", NULL };
static const char *inputchinesecmd[] = { "inputchinese.sh", NULL };

static Key keys[] = {
        /* modifier                      key             function         argument */
        STACKKEYS(MODKEY,                                focus)
        STACKKEYS(MODKEY|ShiftMask,                      push)
        STACKKEYS_MON(WINKEY,                            focus)
        STACKKEYS_MON(WINKEY|ShiftMask,                  push)
        TILEKEYS(MODKEY,                                 1, 0, 0)
        TILEKEYS(MODKEY|ControlMask,                     0, 1, 0)
        TILEKEYS(MODKEY|ShiftMask,                       0, 0, 1)
        TILEKEYS(MODKEY|ControlMask|ShiftMask,           1, 1, 1)
        /* https://cgit.freedesktop.org/xorg/proto/x11proto/tree/XF86keysym.h */
        /* Sound cmd. */
        { 0,                             0x1008FF11,      spawn,          {.v = pactlvoldowncmd } },
        { 0,                             0x1008FF13,      spawn,          {.v = pactlvolupcmd } },
        { 0,                             0x1008FF12,      spawn,          {.v = pactlmutecmd } },
        { WINKEY,                        0x1008FF12,      spawn,          {.v = pactlmicmutecmd } },
        { ShiftMask,                     0x1008FF11,      spawn,          {.v = amixervoldowncmd } },
        { ShiftMask,                     0x1008FF13,      spawn,          {.v = amixervolupcmd } },
        { ShiftMask,                     0x1008FF12,      spawn,          {.v = amixermutecmd } },
        { WINKEY,                        XK_Page_Up,      spawn,          {.v = amixervolupcmd } },
        { WINKEY,                        XK_Page_Down,    spawn,          {.v = amixervoldowncmd } },
        { WINKEY,                        XK_Delete,       spawn,          {.v = amixermutecmd } },
        /* Backlight cmd. */
        { 0,                             0x1008FF03,      spawn,          {.v = backlightdowncmd } },
        { 0,                             0x1008FF02,      spawn,          {.v = backlightupcmd } },
        { WINKEY|ShiftMask,              XK_Page_Up,      spawn,          {.v = backlightupcmd } },
        { WINKEY|ShiftMask,              XK_Page_Down,    spawn,          {.v = backlightdowncmd } },
        { WINKEY|ShiftMask,              XK_Delete,       spawn,          {.v = backlightoffcmd } },
        /* Set layout. */
        { WINKEY,                        XK_b,            togglebar,      {0} },
        { WINKEY,                        XK_space,        setlayout,      {0} },
        { WINKEY|ControlMask,            XK_space,        setlayout,      {.v = &layouts[0] } },
        { WINKEY|ShiftMask,              XK_space,        setlayout,      {.v = &layouts[1] } },
        { WINKEY|ControlMask|ShiftMask,  XK_space,        setlayout,      {.v = &layouts[2] } },
        { MODKEY|ControlMask,            XK_space,        togglefloating, {0} },
        { WINKEY|ControlMask,            XK_minus,        incnmaster,     {.i = -1 } },
        { WINKEY|ControlMask,            XK_equal,        incnmaster,     {.i = +1 } },
        { WINKEY|ControlMask,            XK_BackSpace,    setnmaster,     {.i = 0 } },
        { WINKEY|ControlMask,            XK_1,            setnmaster,     {.i = 1 } },
        { WINKEY|ControlMask,            XK_2,            setnmaster,     {.i = 2 } },
        { WINKEY|ControlMask,            XK_3,            setnmaster,     {.i = 3 } },
        { WINKEY|ControlMask,            XK_4,            setnmaster,     {.i = 4 } },
        { WINKEY|ControlMask,            XK_5,            setnmaster,     {.i = 5 } },
        { WINKEY|ControlMask,            XK_6,            setnmaster,     {.i = 6 } },
        { WINKEY,                        XK_backslash,    setdirs,        {.v = (int[]){ DirHor, DirVer, DirVer } } },
        { WINKEY|ShiftMask,              XK_backslash,    setdirs,        {.v = (int[]){ DirVer, DirHor, DirHor } } },
        /* Cmd. */
        { WINKEY,                        XK_z,            spawn,          {.v = slockcmd } },
        { WINKEY,                        XK_Return,       spawn,          {.v = termcmd } },
        { WINKEY|ShiftMask,              XK_Return,       togglescratch,  {.v = scratchpadcmd } },
        { WINKEY,                        XK_apostrophe,   spawn,          {.v = ec_ffbmkscmd } },
        { WINKEY|ShiftMask,              XK_apostrophe,   spawn,          {.v = ec_ffhistcmd } },
        { WINKEY,                        XK_r,            spawn,          {.v = rofiruncmd } },
        { WINKEY|ShiftMask,              XK_r,            spawn,          {.v = rofidruncmd } },
        { WINKEY|ControlMask,            XK_r,            spawn,          {.v = dmenucmd } },
        { WINKEY,                        XK_Tab,          spawn,          {.v = rofiwincmd } },
        { WINKEY|ShiftMask,              XK_Tab,          spawn,          {.v = switchcmd } },
        { WINKEY,                        XK_a,            spawn,          {.v = rofisshcmd } },
        { WINKEY,                        XK_w,            spawn,          {.v = xclipcopycmd } },
        { WINKEY,                        XK_y,            spawn,          {.v = xclippastecmd } },
        { WINKEY|ShiftMask,              XK_y,            spawn,          {.v = ec_killringcmd } },
        { WINKEY|ControlMask,            XK_y,            spawn,          {.v = roficlipcmd } },
        { WINKEY,                        XK_c,            spawn,          {.v = roficalccmd } },
        { WINKEY,                        XK_p,            spawn,          {.v = rofipasscmd } },
        { WINKEY,                        XK_n,            spawn,          {.v = keynavcmd } },
        { WINKEY,                        XK_d,            spawn,          {.v = rofitranscmd } },
        { WINKEY|ShiftMask,              XK_d,            spawn,          {.v = ec_dictcmd } },
        { WINKEY|ControlMask,            XK_d,            spawn,          {.v = ec_examplecmd } },
        { WINKEY,                        XK_e,            spawn,          {.v = inputchinesecmd } },
        { WINKEY|ShiftMask,              XK_e,            spawn,          {.v = ec_editcmd } },
        { WINKEY|ControlMask,            XK_e,            spawn,          {.v = ec_editselcmd } },
        /* runorraise */
        /* 修改runorraise使其在不同屏幕间切换 */
        { MODKEY|ShiftMask,              XK_space,        spawn,          {.v = emacsclientcmd } },
        { WINKEY,                        XK_f,            runorraise,     {.v = firefoxcmd } },
        /* Maximize. */
        { WINKEY,                        XK_m,            toggleverticalmax, {0} },
        { WINKEY|ShiftMask,              XK_m,            togglehorizontalmax, {0} },
        { WINKEY|ControlMask,            XK_m,            togglemaximize, {0} },
        /* Moveresize. */
        { WINKEY|ShiftMask,              XK_k,            moveresize,     {.v = "0x 0y 0w -50h" } },
        { WINKEY|ShiftMask,              XK_j,            moveresize,     {.v = "0x 0y 0w 50h" } },
        { WINKEY|ShiftMask,              XK_h,            moveresize,     {.v = "0x 0y -50w 0h" } },
        { WINKEY|ShiftMask,              XK_l,            moveresize,     {.v = "0x 0y 50w 0h" } },
        { WINKEY|ControlMask,            XK_k,            moveresize,     {.v = "0x -50y 0w 0h" } },
        { WINKEY|ControlMask,            XK_j,            moveresize,     {.v = "0x 50y 0w 0h" } },
        { WINKEY|ControlMask,            XK_h,            moveresize,     {.v = "-50x 0y 0w 0h" } },
        { WINKEY|ControlMask,            XK_l,            moveresize,     {.v = "50x 0y 0w 0h" } },
        { WINKEY|ControlMask|ShiftMask,  XK_k,            explace,        {.ui = EX_N } },
        { WINKEY|ControlMask|ShiftMask,  XK_j,            explace,        {.ui = EX_S } },
        { WINKEY|ControlMask|ShiftMask,  XK_h,            explace,        {.ui = EX_W } },
        { WINKEY|ControlMask|ShiftMask,  XK_l,            explace,        {.ui = EX_E } },
        /* Exresize. */
        { WINKEY,                        XK_bracketleft,  exresize,       {.v = (int []){ -50,   0 } } },
        { WINKEY,                        XK_bracketright, exresize,       {.v = (int []){  50,   0 } } },
        { WINKEY|ShiftMask,              XK_bracketleft,  exresize,       {.v = (int []){   0, -50 } } },
        { WINKEY|ShiftMask,              XK_bracketright, exresize,       {.v = (int []){   0,  50 } } },
        /* Mark. */
        { WINKEY,                        XK_semicolon,    swapfocus,      {0} },
        { WINKEY|ShiftMask,              XK_semicolon,    swapclient,     {0} },
        { WINKEY|ControlMask,            XK_semicolon,    togglemark,     {0} },
        /* View. */
        { MODKEY,                        XK_space,        zoom,           {0} },
        { MODKEY,                        XK_grave,        view,           {0} },
        { MODKEY,                        XK_F4,           killclient,     {0} },
        { WINKEY,                        XK_q,            killclient,     {0} },
        { WINKEY|ShiftMask,              XK_q,            killunsel,      {0} },
        { WINKEY|ControlMask,            XK_q,            spawn,          {.v = rofipowercmd } },
        { WINKEY|ControlMask|ShiftMask,  XK_q,            quit,           {0} },
        { MODKEY,                        XK_7,            view,           {.ui = ~0 } },
        { MODKEY|ShiftMask,              XK_7,            tag,            {.ui = ~0 } },
        TAGKEYS(                         XK_8,            0)
        TAGKEYS(                         XK_9,            1)
        TAGKEYS(                         XK_0,            2)
        TAGKEYS_MON(                     XK_8,            0)
        TAGKEYS_MON(                     XK_9,            1)
        TAGKEYS_MON(                     XK_0,            2)
        /* Focus adjacent tag. */
        { MODKEY,                        XK_minus,        viewtoleft,     {0} },
        { MODKEY,                        XK_equal,        viewtoright,    {0} },
        { MODKEY|ShiftMask,              XK_minus,        tagtoleft,      {0} },
        { MODKEY|ShiftMask,              XK_equal,        tagtoright,     {0} },
        /* Multi monitors. */
        { WINKEY,                        XK_minus,        focusmon,       {.i = -1 } },
        { WINKEY,                        XK_equal,        focusmon,       {.i = +1 } },
        { WINKEY|ShiftMask,              XK_minus,        tagmon,         {.i = -1 } },
        { WINKEY|ShiftMask,              XK_equal,        tagmon,         {.i = +1 } },
        { WINKEY,                        XK_comma,        focusmonsp,     {.i = 0 } },
        { WINKEY,                        XK_period,       focusmonsp,     {.i = 1 } },
        { WINKEY,                        XK_slash,        focusmonsp,     {.i = 2 } },
        { WINKEY|ShiftMask,              XK_comma,        tagmonsp,       {.i = 0 } },
        { WINKEY|ShiftMask,              XK_period,       tagmonsp,       {.i = 1 } },
        { WINKEY|ShiftMask,              XK_slash,        tagmonsp,       {.i = 2 } },
        { WINKEY,                        XK_grave,        focusmonsp,     {.i = -1 } },
        { WINKEY|ShiftMask,              XK_grave,        tagmonsp,       {.i = -1 } },
        { WINKEY,                        XK_7,            viewallmon,     {.ui = ~0 } },
        /* Screenshot. */
        { WINKEY,                        XK_s,            spawn,          {.v = ssselectcmd } },
        { WINKEY|ShiftMask,              XK_s,            spawn,          {.v = ssbordercmd } },
        { WINKEY|ControlMask,            XK_s,            spawn,          {.v = ssdesktopcmd } },
        { WINKEY|ShiftMask,              XK_w,            spawn,          {.v = ssocrengcmd } },
        { WINKEY|ControlMask,            XK_w,            spawn,          {.v = ssocrchicmd } },
        /* MPD setup. */
        { WINKEY,                        XK_Left,         spawn,          {.v = mpdseekminuscmd } },
        { WINKEY,                        XK_Right,        spawn,          {.v = mpdseekpluscmd } },
        { WINKEY,                        XK_Down,         spawn,          {.v = mpdtogglecmd } },
        { WINKEY|ShiftMask,              XK_Down,         spawn,          {.v = mpdstopcmd } },
        { WINKEY|ShiftMask,              XK_Left,         spawn,          {.v = mpdprevcmd } },
        { WINKEY|ShiftMask,              XK_Right,        spawn,          {.v = mpdnextcmd } },
        { WINKEY|ControlMask,            XK_Up,           spawn,          {.v = mpdvoldowncmd } },
        { WINKEY|ControlMask,            XK_Down,         spawn,          {.v = mpdvolupcmd } },
        { 0,                             0x1008FF16,      spawn,          {.v = mpdseekminuscmd } },
        { 0,                             0x1008FF17,      spawn,          {.v = mpdseekpluscmd } },
        { 0,                             0x1008FF14,      spawn,          {.v = mpdtogglecmd } },
        { 0,                             0x1008FF15,      spawn,          {.v = mpdstopcmd } },
        { ShiftMask,                     0x1008FF16,      spawn,          {.v = mpdprevcmd } },
        { ShiftMask,                     0x1008FF17,      spawn,          {.v = mpdnextcmd } },
        /* Mouse */
        { WINKEY,                        XK_h,            spawn,          SHCMD("exec xdotool mousemove_relative -- -20 0" ) },
        { WINKEY,                        XK_l,            spawn,          SHCMD("exec xdotool mousemove_relative 20 0" ) },
        { WINKEY,                        XK_k,            spawn,          SHCMD("exec xdotool mousemove_relative -- 0 -20" ) },
        { WINKEY,                        XK_j,            spawn,          SHCMD("exec xdotool mousemove_relative 0 20" ) },
        { WINKEY,                        XK_u,            spawn,          SHCMD("exec xdotool click --clearmodifiers 1" ) },
        { WINKEY,                        XK_i,            spawn,          SHCMD("exec xdotool click --clearmodifiers 2" ) },
        { WINKEY,                        XK_o,            spawn,          SHCMD("exec xdotool click --clearmodifiers 3" ) },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
        /* click                event mask      button          function        argument */
        TILEBUTTONS(MODKEY,                                 1, 0, 0)
        TILEBUTTONS(MODKEY|ControlMask,                     0, 1, 0)
        TILEBUTTONS(MODKEY|ShiftMask,                       0, 0, 1)
        TILEBUTTONS(MODKEY|ControlMask|ShiftMask,           1, 1, 1)
        { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
        { ClkLtSymbol,          ShiftMask,      Button1,        setlayout,      {.v = &layouts[2] } },
        { ClkLtSymbol,          0,              Button2,        setdirs,        {.v = (int[]){ DirHor, DirVer, DirVer } } },
        { ClkLtSymbol,          ShiftMask,      Button2,        setdirs,        {.v = (int[]){ DirVer, DirHor, DirHor } } },
        { ClkLtSymbol,          0,              Button3,        incnmaster,     {.i = +1 } },
        { ClkLtSymbol,          ShiftMask,      Button3,        incnmaster,     {.i = -1 } },
        { ClkWinTitle,          0,              Button2,        zoom,           {0} },
        { ClkWinTitle,          ShiftMask,      Button2,        spawn,          {.v = emacsclientcmd } },
        { ClkWinTitle,          0,              Button1,        focusstack,     {.i = INC(-1) } },
        { ClkWinTitle,          ShiftMask,      Button1,        pushstack,      {.i = INC(-1) } },
        { ClkWinTitle,          0,              Button3,        focusstack,     {.i = INC(+1) } },
        { ClkWinTitle,          ShiftMask,      Button3,        pushstack,      {.i = INC(+1) } },
        { ClkStatusText,        0,              Button1,        spawn,          {.v = rofiruncmd } },
        { ClkStatusText,        ShiftMask,      Button1,        spawn,          {.v = dmenucmd } },
        { ClkStatusText,        0,              Button2,        killclient,     {0} },
        { ClkStatusText,        ShiftMask,      Button2,        quit,           {0} },
        { ClkStatusText,        0,              Button3,        spawn,          {.v = termcmd } },
        { ClkStatusText,        ShiftMask,      Button3,        togglescratch,  {.v = scratchpadcmd } },
        { ClkClientWin,         WINKEY|MODKEY,  Button1,        movemouse,      {0} },
        { ClkClientWin,         WINKEY|MODKEY,  Button2,        togglefloating, {0} },
        { ClkClientWin,         WINKEY|MODKEY,  Button3,        resizemouse,    {0} },
        { ClkTagBar,            0,              Button1,        view,           {0} },
        { ClkTagBar,            0,              Button3,        toggleview,     {0} },
        { ClkTagBar,            ShiftMask,      Button1,        tag,            {0} },
        { ClkTagBar,            ShiftMask,      Button3,        toggletag,      {0} },
};
