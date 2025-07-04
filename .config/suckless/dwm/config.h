/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
#include "exitdwm.c"
#define SESSION_FILE "/tmp/dwm-session"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "terminus:size=10" };
static const char dmenufont[]       = "terminus:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* scratchpads */
typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spcm",   "-g", "120x34","-e", "cmus", NULL };
const char *spcmd3[] = {"st", "-n", "spbc",   "-g", "80x20", "-e", "bc", "-lq",  NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spcmus",      spcmd2},
        {"spbcal",      spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const int taglayouts[] = { 1, 0, 0, 0, 0, 0, 0, 0, 0 };

static const Rule rules[] = {
	/* class              instance   title    tags mask   isfloating   monitor */
	{  NULL,              "spterm",   NULL,   SPTAG(0),	 1,	    -1 },
	{  NULL,              "spcm",     NULL,   SPTAG(1),	 1,	    -1 },
        {  NULL,              "spbc",     NULL,   SPTAG(2),      1,         -1 },
        { "Brave-browser",     NULL,      NULL,   1 << 0,        0,         -1 },
        { "Spotify",           NULL,      NULL,   1 << 2,        0,         -1 }, 
        { "transmission-gtk",  NULL,      NULL,   1 << 8,        1,         -1 },
        { "Bleachbit",         NULL,      NULL,   0,             1,         -1 },
        { "Lxappearance",      NULL,      NULL,   0,             1,         -1 },
        { "easyeffects",       NULL,      NULL,   0,             1,         -1 },
        { "ft",                NULL,      NULL,   0,             1,         -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange   function */
	{ "[]",       tile },   /* first entry is default */
        { "[M]",   monocle },
	{ "><",       NULL },   /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2]            = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]      = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *volup[]         = { "wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@", "5%+",    NULL };
static const char *voldown[]       = { "wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@", "5%-",    NULL };
static const char *mutevol[]       = { "wpctl", "set-mute",   "@DEFAULT_AUDIO_SINK@", "toggle", NULL };
static const char *brighter[]      = { "brightnessctl", "set", "10%+", NULL };
static const char *dimmer[]        = { "brightnessctl", "set", "10%-", NULL };
static const char *medpausecmd[]   = { "playerctl", "play-pause", NULL };
static const char *mednextcmd[]    = { "playerctl", "next", NULL };
static const char *medprevcmd[]    = { "playerctl", "previous", NULL };
static const char *lockcmd[]       = { "slock", NULL };
static const char *termcmd[]       = { "st", NULL };

static const Key keys[] = {
	/* modifier                     key                              function        argument */
	{ 0,                            XF86XK_MonBrightnessDown,        spawn,          {.v = dimmer } },
        { 0,                            XF86XK_MonBrightnessUp,          spawn,          {.v = brighter } },
	{ 0,                            XF86XK_AudioRaiseVolume,         spawn,          {.v = volup } },
	{ 0,                            XF86XK_AudioLowerVolume,         spawn,          {.v = voldown } },
	{ 0,                            XF86XK_AudioMute,                spawn,          {.v = mutevol } },
	{ 0,                            XF86XK_AudioPlay,                spawn,          {.v = medpausecmd } },
        { 0,                            XF86XK_AudioNext,                spawn,          {.v = mednextcmd } },
        { 0,                            XF86XK_AudioPrev,                spawn,          {.v = medprevcmd } },
	{ 0,                            XK_Print,                        spawn,          SHCMD("~/.scripts/screenshot.sh") },
        { MODKEY,                       XK_comma,                        spawn,          SHCMD("transmission-gtk") },
        { MODKEY|ShiftMask,             XK_n,                            spawn,          SHCMD("st -c ft -e nmtui") },
        { MODKEY,                       XK_r,                            spawn,          SHCMD("st -e ranger") },
        { MODKEY,                       XK_period,                       spawn,          SHCMD("bleachbit") },
        { MODKEY,                       XK_s,                            spawn,          SHCMD("spotify") },
	{ MODKEY,                       XK_b,                            spawn,          SHCMD("brave") },
	{ MODKEY,                       XK_p,                            spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,                       spawn,          {.v = termcmd } },
        { MODKEY|ShiftMask,             XK_l,                            spawn,          {.v = lockcmd } },
        { MODKEY,                       XK_e,                            exitdwm,        {0} },
	{ MODKEY,                       XK_c,                            togglebar,      {0} },
	{ MODKEY,                       XK_j,                            focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                            focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,                            incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,                            incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                            setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                            setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Tab,                          view,           {0} },
	{ MODKEY,                       XK_q,                            killclient,     {0} },
	{ MODKEY,                       XK_t,                            setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,                            setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,                            setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                        setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,                        togglefloating, {0} },
	{ MODKEY,                       XK_0,                            view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                            tag,            {.ui = ~0 } },
       	{ MODKEY,                       XK_z,                            focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_v,                            focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_z,                            tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_v,                            tagmon,         {.i = +1 } },
        { MODKEY,            		XK_y,  	                         togglescratch,  {.ui = 0 } },
	{ MODKEY,            		XK_u,	                         togglescratch,  {.ui = 1 } },
	{ MODKEY,            		XK_x,	                         togglescratch,  {.ui = 2 } },
        { MODKEY,                       XK_w,                            togglesticky,   {0} },
        { MODKEY|ShiftMask,             XK_e,                            quit,           {0} },
        { MODKEY|ShiftMask,             XK_q,                            quit,           {1} }, 
        TAGKEYS(                        XK_1,                                            0)
	TAGKEYS(                        XK_2,                                            1)
	TAGKEYS(                        XK_3,                                            2)
	TAGKEYS(                        XK_4,                                            3)
	TAGKEYS(                        XK_5,                                            4)
	TAGKEYS(                        XK_6,                                            5)
	TAGKEYS(                        XK_7,                                            6)
	TAGKEYS(                        XK_8,                                            7)
	TAGKEYS(                        XK_9,                                            8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
 	{ ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

