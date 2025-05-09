/* See LICENSE file for copyright and license details. */

/* patches */
/*
	1. https://dwm.suckless.org/patches/movestack/
	2. https://dwm.suckless.org/patches/resizehere/
	3. https://dwm.suckless.org/patches/statusallmons/
	4. https://dwm.suckless.org/patches/cool_autostart/dwm-cool-autostart-20240312-9f88553.diff
	5. https://dwm.suckless.org/patches/alpha/dwm-fixborders-6.2.diff
	6. https://dwm.suckless.org/patches/systray/dwm-systray-20230922-9f88553.diff
	7. https://dwm.suckless.org/patches/swallow/dwm-swallow-6.3.diff
*/

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 1;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = {
	"DroidSansMNerdFontMono:size=12",
	"monospace:size=14"
};
static const char dmenufont[]       = "DroidSansMNerdFontMono:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_yellow[]      = "#d79921";
static const char col_orange[]      = "#d65d0e";
static const char *colors[][3]      = {
	/*               fg         bg          border    */
	[SchemeNorm] = { col_gray3, col_gray1,  col_gray2  },
	[SchemeSel]  = { col_gray4, col_orange, col_orange },
};

static const char *const autostart[] = {
	"slstatus", NULL,
	"xrandr", "--output", "HDMI-A-0", "--mode", "1920x1080", "--rate", "75", NULL, // TODO somehow resolve for different hosts
	"hsetroot", "-fill", "/home/a/NixOS/pictures/ARR02.png", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "kitty",   NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "feh",     NULL,     NULL,           0,         0,          0,           1,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define PrintScreen 0x0000ff61
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *rebuildcmd[] = { "kitty", "--hold", "bash", "/home/a/NixOS/scripts/error-wrap.sh", NULL };
static const char *v2rayAcmd[] = { "kitty", "sudo", "v2rayA", NULL };
static const char *slockcmd[] = { "slock", NULL };
static const char *rangercmd[] = { "kitty", "--hold", "ranger", NULL };
static const char *savescreenshotcmd[] = { "bash", "/home/a/NixOS/scripts/savescreenshot.sh", NULL };
static const char *copyscreenshotcmd[] = { "bash", "/home/a/NixOS/scripts/copyscreenshot.sh", NULL };
static const char *alsamixercmd[] = { "kitty", "alsamixer", NULL };

#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key          function        argument */
	{ MODKEY,                       XK_p,        spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,   spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,        togglebar,      {0} },
	{ MODKEY,                       XK_j,        focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,        focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,        incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,        incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,        setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,        setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,        movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,        movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Return,   zoom,           {0} },
	{ MODKEY,                       XK_Tab,      view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,        killclient,     {0} },
	{ MODKEY,                       XK_t,        setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,        setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,        setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,    setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,    togglefloating, {0} },
	{ MODKEY,                       XK_0,        view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,        tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,    focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,   focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,    tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,   tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                        0)
	TAGKEYS(                        XK_2,                        1)
	TAGKEYS(                        XK_3,                        2)
	TAGKEYS(                        XK_4,                        3)
	TAGKEYS(                        XK_5,                        4)
	TAGKEYS(                        XK_6,                        5)
	TAGKEYS(                        XK_7,                        6)
	TAGKEYS(                        XK_8,                        7)
	TAGKEYS(                        XK_9,                        8)
	{ MODKEY|ShiftMask,             XK_q,        quit,           {0} },
	{ MODKEY,                       XK_r,        spawn,          {.v = rebuildcmd } },
	{ MODKEY,                       XK_v,        spawn,          {.v = v2rayAcmd } },
	{ MODKEY,                       XK_s,        spawn,          {.v = slockcmd } },
	{ MODKEY,                       XK_e,        spawn,          {.v = rangercmd } },
	{ MODKEY|ShiftMask,             PrintScreen, spawn,          {.v = savescreenshotcmd } },
	{ MODKEY,                       PrintScreen, spawn,          {.v = copyscreenshotcmd } },
	{ MODKEY,                       XK_a,        spawn,          {.v = alsamixercmd } },
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
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

