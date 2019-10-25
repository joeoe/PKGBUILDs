/* for XF86 Media Keys */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 8;        /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
/* static const char *fonts[]          = { "Ubuntu:size=10:style=medium", "Hack Nerd Font Mono Bold:size=11", "monospace:size=10" }; */
static const char *fonts[]          = { "Ubuntu Nerd Font:size=10:style=medium", "Ubuntu:size=10:style=medium", "Hack Nerd Font Mono Bold:size=11", "monospace:size=10" };
static const char dmenufont[]       = "Hack Nerd Font Mono Bold:size=11";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class       instance    title           tags mask     isfloating   monitor */
	{ "MediaElch", NULL,       "Settings",     0,            1,           -1 },
	{ "Steam",     NULL,       "Friends List", 0,            1,           -1 },
	{ "Steam",     NULL,       NULL,           1 << 5,       0,           -1 },
	{ "Kodi",      NULL,       NULL,           0,            1,           -1 },
	{ "TelegramDesktop", NULL, NULL,           0,            1,           -1 },
	{ "Filezilla", NULL,       NULL,           1 << 7,       0,           -1 },
	{ "Deluge",    NULL,       NULL,           1 << 7,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	/* { "><>",      NULL },    /1* no layout function means floating behavior *1/ */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
/* static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL }; */
/* static const char *passmenucmd[] = { "passmenu", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selbordercolor, NULL }; */
/* static const char *term_default[]  = { "st", "-e", "tmuxinator", "start", "default", NULL }; */
/* static const char *term_mpsyt[]  = { "st", "-e", "tmux", "start", "mpsyt", NULL }; */
/* static const char *term_notes[]  = { "st", "-e", "tmuxinator", "start", "notes", NULL }; */
/* static const char *term_rc[]  = { "st", "-e", "tmuxinator", "start", "rc", NULL }; */
static const char *rofi_pass[] = { "rofi-pass" };
static const char *rofi_tmux[] = { "rofi-tmux" };
static const char *rofi_book[] = { "rofi-books" };
static const char *dmenucmd[] =  { "rofi", "-show", "run", NULL};
static const char *termcmd[] =   { "st", NULL };
static const char *term_tmux[] = { "st", "-e", "tmux", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };
static const char *media_next[] =  { "media-global-next", NULL};
static const char *media_prev[] =  { "media-global-prev", NULL};
static const char *media_play[] =  { "media-global-play", NULL};
static const char *vol_up[] =    { "change-volume", "5%+", NULL};
static const char *vol_down[] =  { "change-volume", "5%-", NULL};
static const char *vol_mute[] =  { "change-volume", "toggle", NULL};
/* static const char *screenshot_sel[] = { "sleep", "0.2;", "scrot", "'%Y-%m-%d_%T_$wx$h_scr.png'", "-s", "-e", "'mv", "$f", "~/images/shots/'", NULL}; */
static const char *screenshot_win[] = { "scrot", "%Y-%m-%d_%T_$wx$h_scr.png", "-u", "-e", "mv $f ~/images/shots/", NULL};
static const char *screenshot_mon[] = { "scrot", "%Y-%m-%d_%T_$wx$h_scr.png", "-u", "-e", "mv $f ~/images/shots/", NULL};
static const char *browser[] = { "firefox", NULL};
static const char *browser_alt[] = { "qutebrowser", NULL};

static Key keys[] = {
	/* modifier            key                        function        argument */
	{ MODKEY,              XK_d,                      spawn,          {.v = dmenucmd } },
	{ MODKEY,              XK_x,                      spawn,          {.v = dmenucmd } },
	{ MODKEY,              XK_p,                      spawn,          {.v = rofi_pass } },
	{ MODKEY,              XK_t,                      spawn,          {.v = rofi_tmux } },
	{ MODKEY,              XK_F2,                     spawn,          {.v = rofi_book } },
	{ MODKEY|ShiftMask,    XK_Return,                 spawn,          {.v = term_tmux } },
	{ MODKEY|ControlMask,  XK_Return,                 spawn,          {.v = termcmd } },
	{ MODKEY,              XK_apostrophe,             togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,              XK_b,                      spawn,          {.v = browser } },
	{ MODKEY|ShiftMask,    XK_b,                      spawn,          {.v = browser_alt } },
	{ MODKEY|ControlMask,  XK_b,                      togglebar,      {0} },
	{ MODKEY,              XK_a,                      focusurgent,    {0} },
	{ MODKEY,              XK_j,                      focusstack,     {.i = +1 } },
	{ MODKEY,              XK_k,                      focusstack,     {.i = -1 } },
	{ MODKEY,              XK_i,                      incnmaster,     {.i = +1 } },
	{ MODKEY,              XK_y,                      incnmaster,     {.i = -1 } },
	{ MODKEY,              XK_h,                      setmfact,       {.f = -0.05} },
	{ MODKEY,              XK_l,                      setmfact,       {.f = +0.05} },
	{ MODKEY,              XK_equal,                  setmfact,       {.f = 1.50} },
	{ MODKEY,              XK_Return,                 zoom,           {0} },
	{ MODKEY,              XK_Tab,                    view,           {0} },
	{ MODKEY|ShiftMask,    XK_c,                      killclient,     {0} },
	/* { MODKEY,              XK_t,      setlayout,      {.v = &layouts[0]} }, */
	/* { MODKEY,              XK_f,      setlayout,      {.v = &layouts[1]} }, */
	/* { MODKEY,              XK_m,      setlayout,      {.v = &layouts[2]} }, */
	{ MODKEY,              XK_space,                  setlayout,      {0} },
    { MODKEY,              XK_f,                      fullscreen,     {0} },
    { MODKEY|ShiftMask,    XK_f,                      togglefloating, {0} },
	{ MODKEY,              XK_0,                      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,    XK_0,                      tag,            {.ui = ~0 } },
	{ MODKEY,              XK_comma,                  focusmon,       {.i = -1 } },
	{ MODKEY,              XK_period,                 focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_comma,                  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_period,                 tagmon,         {.i = +1 } },
	{ ShiftMask,           XK_Print,                  spawn,          {.v = screenshot_win } },
	{ 0,                   XK_Print,                  spawn,          {.v = screenshot_mon } },
	{ MODKEY,              XK_F5,                     xrdb,           {.v = NULL } },
	TAGKEYS(               XK_1,                      0)
	TAGKEYS(               XK_2,                      1)
	TAGKEYS(               XK_3,                      2)
	TAGKEYS(               XK_4,                      3)
	TAGKEYS(               XK_5,                      4)
	TAGKEYS(               XK_6,                      5)
	TAGKEYS(               XK_7,                      6)
	TAGKEYS(               XK_8,                      7)
	TAGKEYS(               XK_9,                      8)
	{ 0,                   XF86XK_AudioRaiseVolume,   spawn,  {.v = vol_up} },
	{ 0,                   XF86XK_AudioLowerVolume,   spawn,  {.v = vol_down} },
	{ 0,                   XF86XK_AudioMute,          spawn,  {.v = vol_mute} },
	{ 0,                   XF86XK_AudioPlay,          spawn,  {.v = media_play} },
	{ 0,                   XF86XK_AudioNext,          spawn,  {.v = media_next} },
	{ 0,                   XF86XK_AudioPrev,          spawn,  {.v = media_prev} },
	{ MODKEY|ShiftMask,    XK_q,                      quit,           {1} },
	{ MODKEY|ControlMask,  XK_q,                      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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

