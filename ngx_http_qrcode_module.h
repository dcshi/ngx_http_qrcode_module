/*===============================================================
*   Copyright (C) 2013 All rights reserved.
*   
*   Filename：ngx_http_qrcode_module.h
*   Author ：dcshi
*   Created：2013-02-02
================================================================*/

#ifndef NGX_HTTP_QRCODE_MODULE_H
#define NGX_HTTP_QRCODE_MODULE_H

#include <ngx_core.h>
#include <ngx_http.h>
#include <nginx.h>

typedef enum {
	qrcode_cfg_fg_color = 0,
	qrcode_cfg_bg_color,
	qrcode_cfg_level,
	qrcode_cfg_hint,
	qrcode_cfg_size,
	qrcode_cfg_margin,
	qrcode_cfg_version,
	qrcode_cfg_txt,
	qrcode_cfg_urlencode_txt,
	qrcode_cfg_casesensitive,
} ngx_http_qrcode_cfg_t;

/* represent a config directive (or command) like "qrcode_fg_color #FFOOFF". */
typedef struct {
	ngx_http_qrcode_cfg_t 	cfg_code;
	ngx_array_t 			*args;
} ngx_http_qrcode_cmd_t;

typedef struct {
	/* holds the raw string of the argument value */
	ngx_str_t       raw_value;

	/* fields "lengths" and "values" are set by
	 * the function ngx_http_script_compile,
	 * if the argument value indeed contains
	 * nginx variables like "$foo" */
	ngx_array_t     *lengths;
	ngx_array_t     *values;
} ngx_http_qrcode_arg_template_t;

typedef struct {
	ngx_int_t	bg_color[4];			/* color of qrcode */
	ngx_int_t	fg_color[4];			/* color of background, default white */
	ngx_int_t	level;					/* level of error correction, [0:3] */
	ngx_int_t	hint;					/* encoding mode */
	ngx_int_t	size;					/* size of qrcode image */
	ngx_int_t	margin;					/* margin of qrcode image */
	ngx_int_t	version;				/* version of the symbol */
	ngx_int_t	casesensitive;			/* case-sensitive(1) or not(0) */
	ngx_str_t	txt;					/* just txt */
	ngx_array_t *cmds;
} ngx_http_qrcode_loc_conf_t;

#endif
