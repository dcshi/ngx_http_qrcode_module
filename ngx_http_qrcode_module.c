/*===============================================================
*   Copyright (C) 2012 All rights reserved.
*   
*   Filename：ngx_http_qrcode_module.c
*   Author ：dcshi
*   Created：2012-12-09
================================================================*/

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <gd.h>
#include <qrencode.h>

//1M
#define MAX_TXT_LEN 1024*1024 

typedef struct {
	ngx_int_t bg_color[4];			/* color of qrcode */
	ngx_int_t fg_color[4];			/* color of background, default white */
	ngx_int_t level;				/* level of error correction, [0:3] */
	ngx_int_t hint;					/* encoding mode */
	ngx_int_t size;					/* size of qrcode image */
	ngx_int_t margin;				/* margin of qrcode image */
	ngx_int_t version;				/* version of the symbol */
	ngx_flag_t casesensitive;		/* case-sensitive(1) or not(0) */
	ngx_str_t  txt;					/* just txt */
} ngx_http_qrcode_loc_conf_t;

static char *
ngx_http_qrcode_set_color(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static char *
ngx_http_qrcode_gen(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_int_t 
ngx_http_qrcode_handler(ngx_http_request_t* request);

static void *
ngx_http_qrcode_create_loc_conf(ngx_conf_t *cf);

static char * 
ngx_http_qrcode_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child);

static ngx_command_t  ngx_http_qrcode_commands[] = {
	{ 
		ngx_string("qrcode_fg_color"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_http_qrcode_set_color,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, fg_color),
		NULL },
	{ 
		ngx_string("qrcode_bg_color"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_http_qrcode_set_color,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, bg_color),
		NULL },
	{ 
		ngx_string("qrcode_level"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_conf_set_num_slot,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, level),
		NULL },
	{ 
		ngx_string("qrcode_hint"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_conf_set_num_slot,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, hint),
		NULL },
	{ 
		ngx_string("qrcode_size"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_conf_set_num_slot,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, size),
		NULL },
	{ 
		ngx_string("qrcode_margin"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_conf_set_num_slot,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, margin),
		NULL },
	{ 
		ngx_string("qrcode_version"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_conf_set_num_slot,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, version),
		NULL },
	{ 
		ngx_string("qrcode_casesensitive"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_conf_set_flag_slot,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, casesensitive),
		NULL },
	{ 
		ngx_string("qrcode_txt"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_conf_set_str_slot,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, txt),
		NULL },
	{ 
		ngx_string("qrcode_gen"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1|NGX_CONF_NOARGS,
		ngx_http_qrcode_gen,
		NGX_HTTP_LOC_CONF_OFFSET,
		0,
		NULL },
	ngx_null_command
};

static ngx_http_module_t  ngx_http_qrcode_module_ctx = {
	NULL,                                  /* preconfiguration */
	NULL,                                  /* postconfiguration */
	NULL,                                  /* create main configuration */
	NULL,                                  /* init main configuration */
	NULL,                                  /* create server configuration */
	NULL,                                  /* merge server configuration */
	ngx_http_qrcode_create_loc_conf,       /* create location configration */
	ngx_http_qrcode_merge_loc_conf         /* merge location configration */
};

ngx_module_t  ngx_http_qrcode_module = {
	NGX_MODULE_V1,
	&ngx_http_qrcode_module_ctx,       	   	/* module context */
	ngx_http_qrcode_commands,             	/* module directives */
	NGX_HTTP_MODULE,                        /* module type */
	NULL,                                   /* init master */
	NULL,                                   /* init module */
	NULL,                                   /* init process */
	NULL,                                   /* init thread */
	NULL,                                   /* exit thread */
	NULL,                                   /* exit process */
	NULL,                                   /* exit master */
	NGX_MODULE_V1_PADDING
};

static void *
ngx_http_qrcode_create_loc_conf(ngx_conf_t *cf)
{
	ngx_http_qrcode_loc_conf_t  *qr_cf;

	qr_cf = ngx_pcalloc(cf->pool, sizeof(ngx_http_qrcode_loc_conf_t));
	if (qr_cf == NULL) {
		ngx_log_error(NGX_LOG_ERR, cf->log, 0, "Mem calloc ngx_http_qrcode_loc_conf_t fail");
		return NGX_CONF_ERROR;
	}

	qr_cf->fg_color[0] = NGX_CONF_UNSET_UINT;
	qr_cf->fg_color[1] = NGX_CONF_UNSET_UINT;
	qr_cf->fg_color[2] = NGX_CONF_UNSET_UINT;
	qr_cf->fg_color[3] = NGX_CONF_UNSET_UINT;
	qr_cf->bg_color[0] = NGX_CONF_UNSET_UINT;
	qr_cf->bg_color[1] = NGX_CONF_UNSET_UINT;
	qr_cf->bg_color[2] = NGX_CONF_UNSET_UINT;
	qr_cf->bg_color[3] = NGX_CONF_UNSET_UINT;
	qr_cf->level = NGX_CONF_UNSET_UINT;
	qr_cf->hint = NGX_CONF_UNSET_UINT;
	qr_cf->size = NGX_CONF_UNSET_UINT;
	qr_cf->margin = NGX_CONF_UNSET_UINT;
	qr_cf->version = NGX_CONF_UNSET_UINT;
	qr_cf->casesensitive = NGX_CONF_UNSET_UINT;

	return qr_cf;
}

static char * 
ngx_http_qrcode_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child) {
	ngx_http_qrcode_loc_conf_t *pre = parent;
	ngx_http_qrcode_loc_conf_t *qr_cf = child;

	ngx_conf_merge_value(qr_cf->fg_color[0], pre->fg_color[0], 0);
	ngx_conf_merge_value(qr_cf->fg_color[1], pre->fg_color[0], 0);
	ngx_conf_merge_value(qr_cf->fg_color[2], pre->fg_color[0], 0);
	ngx_conf_merge_value(qr_cf->fg_color[3], pre->fg_color[0], 0);
	ngx_conf_merge_value(qr_cf->bg_color[0], pre->bg_color[0], 255);
	ngx_conf_merge_value(qr_cf->bg_color[1], pre->bg_color[0], 255);
	ngx_conf_merge_value(qr_cf->bg_color[2], pre->bg_color[0], 255);
	ngx_conf_merge_value(qr_cf->bg_color[3], pre->bg_color[0], 255);
	ngx_conf_merge_value(qr_cf->level, pre->level, QR_ECLEVEL_L);
	ngx_conf_merge_value(qr_cf->hint, pre->hint, QR_MODE_8);
	ngx_conf_merge_value(qr_cf->size, pre->size, 4);
	ngx_conf_merge_value(qr_cf->margin, pre->margin, 4);
	ngx_conf_merge_value(qr_cf->version, pre->version, 1);
	ngx_conf_merge_value(qr_cf->casesensitive, pre->casesensitive, 0);
	ngx_conf_merge_str_value(qr_cf->txt, pre->txt, "");

	return NGX_CONF_OK;
}

static char *
ngx_http_qrcode_set_color(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	char *qr_cf = conf;
	ngx_int_t *color;
	ngx_str_t *args;
   
	color = (ngx_int_t *)(qr_cf + cmd->offset);
	args	= cf->args->elts;

	//RGB red:FF0000
	if (args[1].len != 6) {
		ngx_log_error(NGX_LOG_ERR, cf->log, 0, "Invalid color format.Set RGB(FF00FF)");
		return NGX_CONF_ERROR;
	}

	u_char color_buf[8];
	ngx_snprintf(color_buf, args[1].len, "%V", args + 1);
	
	int i;
	for (i = 0; i < 3; i++) {
		*(color + i) = ngx_hextoi(color_buf + i*2, 2);
	}
	
	return NGX_CONF_OK;
}

static char *
ngx_http_qrcode_gen(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	ngx_http_core_loc_conf_t *core_conf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
	core_conf->handler = ngx_http_qrcode_handler;
	
	return NGX_CONF_OK;
}

static ngx_int_t 
ngx_http_qrcode_handler(ngx_http_request_t* request) 
{
	ngx_http_qrcode_loc_conf_t* qr_cf;
	qr_cf = ngx_http_get_module_loc_conf(request, ngx_http_qrcode_module);

	static u_char txt[MAX_TXT_LEN];
	ngx_snprintf(txt, qr_cf->txt.len, "%V", &qr_cf->txt); 	

	QRcode *code;
	code = QRcode_encodeString((char*)txt, qr_cf->version, qr_cf->level, qr_cf->hint, qr_cf->casesensitive);

	if(code == NULL) {
		ngx_log_error(NGX_LOG_ERR, request->connection->log, 0, "Failed to encode address.");
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	int code_size = qr_cf->size;
	int img_margin = qr_cf->margin;
	int img_width = code->width * code_size + 2 * img_margin;

	gdImagePtr img = gdImageCreate(img_width, img_width);
	int fg_color = gdImageColorAllocate(img, qr_cf->fg_color[0], qr_cf->fg_color[1], qr_cf->fg_color[2]);
	int bg_color = gdImageColorAllocate(img, qr_cf->bg_color[0], qr_cf->bg_color[1], qr_cf->bg_color[2]);

	gdImageFill(img, 0, 0, bg_color);

	u_char *p = code->data;
	int x, y, posx, posy;
	for (y = 0; y < code->width; y++) 
	{
		for (x = 0; x < code->width; x++) 
		{
			if (*p & 1) {
				posx = x * code_size + img_margin;
				posy = y * code_size + img_margin;
				gdImageFilledRectangle(img, posx, posy, posx + code_size, posy + code_size, fg_color);
			}
			p++; 
		}
	}

	int img_stream_len;
	u_char *img_stream = gdImagePngPtr(img, &img_stream_len);

	gdImageDestroy(img);
	QRcode_free(code);

	request->headers_out.status = NGX_HTTP_OK;
	request->headers_out.content_length_n = img_stream_len;
	ngx_str_set(&request->headers_out.content_type, "image/png");
	ngx_http_send_header(request);

	ngx_buf_t* buffer;
	buffer = ngx_pcalloc(request->pool, sizeof(ngx_buf_t));

	if (buffer == NULL) {
		ngx_log_error(NGX_LOG_ERR, request->connection->log, 0, "Failed to allocate response buffer");
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	//set up the buffer chain
	ngx_chain_t out;
	buffer->pos = img_stream;
	buffer->last = img_stream + img_stream_len; 
	buffer->memory = 1;
	buffer->last_buf = 1;
	out.buf = buffer;
	out.next = NULL;

	return ngx_http_output_filter(request, &out);
}
