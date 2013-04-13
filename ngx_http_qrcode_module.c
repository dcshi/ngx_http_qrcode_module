/*===============================================================
*   Copyright (C) 2012 All rights reserved.
*   
*   Filename：ngx_http_qrcode_module.c
*   Author ：dcshi
*   Created：2012-12-09
================================================================*/

#include <ngx_config.h>
#include <ngx_http.h>
#include <gd.h>
#include <qrencode.h>
#include "ngx_http_qrcode_utils.h"

static char *
ngx_http_qrcode_fg_color(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static char *
ngx_http_qrcode_bg_color(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static char *
ngx_http_qrcode_level(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static char *
ngx_http_qrcode_hint(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static char *
ngx_http_qrcode_size(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static char *
ngx_http_qrcode_margin(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static char *
ngx_http_qrcode_version(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static char *
ngx_http_qrcode_casesensitive(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static char *
ngx_http_qrcode_txt(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static char *
ngx_http_qrcode_gen(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_int_t 
ngx_http_qrcode_handler(ngx_http_request_t* r);

static void *
ngx_http_qrcode_create_loc_conf(ngx_conf_t *cf);

static char * 
ngx_http_qrcode_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child);

static char *
ngx_http_qrcode_cmder(ngx_http_qrcode_cfg_t cfg_code, 
		ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_command_t  ngx_http_qrcode_commands[] = {
	{ 
		ngx_string("qrcode_fg_color"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_http_qrcode_fg_color,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, fg_color),
		NULL },
	{ 
		ngx_string("qrcode_bg_color"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_http_qrcode_bg_color,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, bg_color),
		NULL },
	{ 
		ngx_string("qrcode_level"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_http_qrcode_level,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, level),
		NULL },
	{ 
		ngx_string("qrcode_hint"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_http_qrcode_hint,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, hint),
		NULL },
	{ 
		ngx_string("qrcode_size"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_http_qrcode_size,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, size),
		NULL },
	{ 
		ngx_string("qrcode_margin"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_http_qrcode_margin,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, margin),
		NULL },
	{ 
		ngx_string("qrcode_version"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_http_qrcode_version,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, version),
		NULL },
	{ 
		ngx_string("qrcode_casesensitive"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_http_qrcode_casesensitive,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_qrcode_loc_conf_t, casesensitive),
		NULL },
	{ 
		ngx_string("qrcode_txt"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_http_qrcode_txt,
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
	ngx_http_qrcode_loc_conf_t  *qlcf;

	qlcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_qrcode_loc_conf_t));
	if (qlcf == NULL) {
		ngx_log_error(NGX_LOG_ERR, cf->log, 0, 
				"Mem calloc ngx_http_qrcode_loc_conf_t fail");
		return NGX_CONF_ERROR;
	}

	/* set by ngx_pcalloc
	qlcf->fg_color[0] 	= NGX_CONF_UNSET_UINT;
	qlcf->fg_color[1] 	= NGX_CONF_UNSET_UINT;
	qlcf->fg_color[2] 	= NGX_CONF_UNSET_UINT;
	qlcf->fg_color[3] 	= NGX_CONF_UNSET_UINT;
	qlcf->bg_color[0] 	= NGX_CONF_UNSET_UINT;
	qlcf->bg_color[1] 	= NGX_CONF_UNSET_UINT;
	qlcf->bg_color[2] 	= NGX_CONF_UNSET_UINT;
	qlcf->bg_color[3] 	= NGX_CONF_UNSET_UINT;
	qlcf->level			= NGX_CONF_UNSET_UINT;
	qlcf->hint			= NGX_CONF_UNSET_UINT;
	qlcf->size			= NGX_CONF_UNSET_UINT;
	qlcf->margin		= NGX_CONF_UNSET_UINT;
	qlcf->version 		= NGX_CONF_UNSET_UINT;
	qlcf->casesensitive = NGX_CONF_UNSET_UINT;
	cmds				= NGX_CONF_OK;
	*/

	return qlcf;
}

static char * 
ngx_http_qrcode_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child) {
	ngx_http_qrcode_loc_conf_t *pre = parent;
	ngx_http_qrcode_loc_conf_t *qlcf = child;

	ngx_conf_merge_value(qlcf->fg_color[0], pre->fg_color[0], 0);
	ngx_conf_merge_value(qlcf->fg_color[1], pre->fg_color[0], 0);
	ngx_conf_merge_value(qlcf->fg_color[2], pre->fg_color[0], 0);
	ngx_conf_merge_value(qlcf->fg_color[3], pre->fg_color[0], 0);
	ngx_conf_merge_value(qlcf->bg_color[0], pre->bg_color[0], 255);
	ngx_conf_merge_value(qlcf->bg_color[1], pre->bg_color[0], 255);
	ngx_conf_merge_value(qlcf->bg_color[2], pre->bg_color[0], 255);
	ngx_conf_merge_value(qlcf->bg_color[3], pre->bg_color[0], 255);
	ngx_conf_merge_value(qlcf->level, pre->level, QR_ECLEVEL_L);
	ngx_conf_merge_value(qlcf->hint, pre->hint, QR_MODE_8);
	ngx_conf_merge_value(qlcf->size, pre->size, 4);
	ngx_conf_merge_value(qlcf->margin, pre->margin, 4);
	ngx_conf_merge_value(qlcf->version, pre->version, 1);
	ngx_conf_merge_value(qlcf->casesensitive, pre->casesensitive, 0);
	ngx_conf_merge_str_value(qlcf->txt, pre->txt, "");

	if (qlcf->cmds == NULL) {
		qlcf->cmds = pre->cmds;
	}

	return NGX_CONF_OK;
}

static char *
ngx_http_qrcode_fg_color(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	return ngx_http_qrcode_cmder(qrcode_cfg_fg_color, cf, cmd, conf);
}

static char *
ngx_http_qrcode_bg_color(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	return ngx_http_qrcode_cmder(qrcode_cfg_bg_color, cf, cmd, conf);
}

static char *
ngx_http_qrcode_level(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	return ngx_http_qrcode_cmder(qrcode_cfg_level, cf, cmd, conf);
}

static char *
ngx_http_qrcode_hint(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	return ngx_http_qrcode_cmder(qrcode_cfg_hint, cf, cmd, conf);
}

static char *
ngx_http_qrcode_size(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	return ngx_http_qrcode_cmder(qrcode_cfg_size, cf, cmd, conf);
}

static char *
ngx_http_qrcode_margin(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	return ngx_http_qrcode_cmder(qrcode_cfg_margin, cf, cmd, conf);
}

static char *
ngx_http_qrcode_version(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	return ngx_http_qrcode_cmder(qrcode_cfg_version, cf, cmd, conf);
}

static char *
ngx_http_qrcode_casesensitive(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	return ngx_http_qrcode_cmder(qrcode_cfg_casesensitive, cf, cmd, conf);
}

static char *
ngx_http_qrcode_txt(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	return ngx_http_qrcode_cmder(qrcode_cfg_txt, cf, cmd, conf);
}

static char *
ngx_http_qrcode_gen(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	ngx_http_core_loc_conf_t *core_conf = 
		ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);

	core_conf->handler = ngx_http_qrcode_handler;
	
	return NGX_CONF_OK;
}

static ngx_int_t 
ngx_http_qrcode_handler(ngx_http_request_t* r) 
{
	ngx_http_qrcode_loc_conf_t *qlcf;
	ngx_int_t 	code_size;
	ngx_int_t 	img_margin, img_width;
	ngx_int_t 	fg_color, bg_color;
	ngx_int_t 	x, y, posx, posy;
	int 		img_stream_len;
	u_char		*encoded_txt;
	ngx_int_t	rc;
	
	qlcf = ngx_http_get_module_loc_conf(r, ngx_http_qrcode_module);

	/* compile args */
	rc = ngx_http_qrcode_compile_args(r, qlcf);
	if (rc != NGX_OK)  {
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Failed to compile args.");
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	encoded_txt = ngx_pcalloc(r->pool, qlcf->txt.len + 1);
	ngx_sprintf(encoded_txt, "%V", &qlcf->txt); 	

	QRcode *code;
	code = QRcode_encodeString((char*)encoded_txt, 
			qlcf->version, qlcf->level, qlcf->hint, qlcf->casesensitive);

	if(code == NULL) {
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, 
				"Failed to encode content.exception raised by libqrencode: %s", strerror(errno));
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	code_size	= qlcf->size / code->width;
	code_size   = (code_size == 0) ? 1 : code_size;
	img_margin	= qlcf->margin;
	img_width	= code->width * code_size + 2 * img_margin;

	ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "img_width,%d, code_size%d,code->width:%d", 
			img_width, code_size, code->width);

	gdImagePtr img;
	img	= gdImageCreate(img_width, img_width);

	fg_color = gdImageColorAllocate(img, 
			qlcf->fg_color[0], qlcf->fg_color[1], qlcf->fg_color[2]);

	bg_color = gdImageColorAllocate(img,
		   	qlcf->bg_color[0], qlcf->bg_color[1], qlcf->bg_color[2]);

	gdImageFill(img, 0, 0, bg_color);

	u_char *p = code->data;
	for (y = 0; y < code->width; y++) 
	{
		for (x = 0; x < code->width; x++) 
		{
			if (*p & 1) {
				posx = x * code_size + img_margin;
				posy = y * code_size + img_margin;

				gdImageFilledRectangle(img, posx, posy, 
						posx + code_size, posy + code_size, fg_color);
			}
			p++; 
		}
	}

	u_char *img_stream;
    img_stream = gdImagePngPtr(img, &img_stream_len);

	gdImageDestroy(img);
	QRcode_free(code);

	r->headers_out.status = NGX_HTTP_OK;
	r->headers_out.content_length_n = img_stream_len;

	ngx_str_set(&r->headers_out.content_type, "image/png");
	ngx_http_send_header(r);

	ngx_buf_t* buffer;
	buffer = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

	if (buffer == NULL) {
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, 
				"Failed to allocate response buffer");
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

	return ngx_http_output_filter(r, &out);
}

static char *
ngx_http_qrcode_cmder(ngx_http_qrcode_cfg_t cfg_code, 
		ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
	ngx_http_qrcode_loc_conf_t	*qlcf;
    ngx_http_qrcode_cmd_t 		*qr_cmd;
	ngx_str_t	*raw_args;
	ngx_array_t **cmds_ptr; 
	ngx_array_t **args_ptr;
	ngx_http_qrcode_arg_template_t	*arg;
	ngx_http_script_compile_t 		sc;
	ngx_uint_t i, n;

	qlcf = (ngx_http_qrcode_loc_conf_t *)conf;
	cmds_ptr = &qlcf->cmds;
	
	if (*cmds_ptr == NULL) 
	{
		*cmds_ptr = ngx_array_create(cf->pool, 1, sizeof(ngx_http_qrcode_cmd_t));

		if (*cmds_ptr == NULL) {
			return NGX_CONF_ERROR;
		}
	}

	qr_cmd = ngx_array_push(*cmds_ptr);

	if (qr_cmd == NULL) {
		return NGX_CONF_ERROR;
	}

	qr_cmd->cfg_code = cfg_code;

	args_ptr = &qr_cmd->args;
	*args_ptr = ngx_array_create(cf->pool, 1, sizeof(ngx_http_qrcode_arg_template_t));

	if (*args_ptr == NULL) {
		return NGX_CONF_ERROR;
	}

	raw_args = cf->args->elts;

	// we skip the first arg and start from the second
	for (i = 1 ; i < cf->args->nelts; i++) 
	{
		arg = ngx_array_push(*args_ptr);

		if (arg == NULL)
			return NGX_CONF_ERROR;

		arg->raw_value 	= raw_args[i];
		arg->lengths 	= NULL;
		arg->values 	= NULL;

		n = ngx_http_script_variables_count(&arg->raw_value);

		if (n > 0) {
			ngx_memzero(&sc, sizeof(ngx_http_script_compile_t));

			sc.cf = cf;
			sc.source  = &arg->raw_value;
			sc.lengths = &arg->lengths;
			sc.values  = &arg->values;
			sc.variables = n;
			sc.complete_lengths = 1;
			sc.complete_values  = 1;

			if (ngx_http_script_compile(&sc) != NGX_OK) {
				return NGX_CONF_ERROR;
			}
		}
	} /* end for */

	return NGX_CONF_OK;
}
