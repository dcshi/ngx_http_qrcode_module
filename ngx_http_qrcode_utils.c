/*===============================================================
*   Copyright (C) 2013 All rights reserved.
*   
*   Filename：ngx_http_qrcode_utils.c
*   Author ：dcshi
*   Created：2013-02-02
================================================================*/

#include "ngx_http_qrcode_utils.h"

ngx_int_t 
ngx_http_qrcode_set_fg_color(ngx_http_request_t *r, ngx_int_t *color, ngx_array_t *compiled_args);

ngx_int_t 
ngx_http_qrcode_set_bg_color(ngx_http_request_t *r, ngx_int_t *color, ngx_array_t *compiled_args);

ngx_int_t 
ngx_http_qrcode_set_level(ngx_http_request_t *r, ngx_int_t *val, ngx_array_t *compiled_args);

ngx_int_t 
ngx_http_qrcode_set_hint(ngx_http_request_t *r, ngx_int_t *val, ngx_array_t *compiled_args);

ngx_int_t 
ngx_http_qrcode_set_size(ngx_http_request_t *r, ngx_int_t *val, ngx_array_t *compiled_args);

ngx_int_t 
ngx_http_qrcode_set_margin(ngx_http_request_t *r, ngx_int_t *val, ngx_array_t *compiled_args);

ngx_int_t 
ngx_http_qrcode_set_version(ngx_http_request_t *r, ngx_int_t *val, ngx_array_t *compiled_args);

ngx_int_t 
ngx_http_qrcode_set_casesensitive(ngx_http_request_t *r, ngx_int_t *val, ngx_array_t *compiled_args);

ngx_int_t 
ngx_http_qrcode_set_content(ngx_http_request_t *r, ngx_str_t *val, ngx_array_t *compiled_args, ngx_int_t urlencode);

ngx_int_t 
ngx_http_qrcode_eval_cmd_args(ngx_http_request_t *r,
		ngx_http_qrcode_cmd_t *cmd, ngx_array_t *compiled_args);


ngx_int_t 
ngx_http_qrcode_set_fg_color(ngx_http_request_t *r, ngx_int_t *color, ngx_array_t *compiled_args)
{
	ngx_str_t *arg;
	ngx_int_t i;

	arg = compiled_args->elts;

	if (arg[0].len != 6) {
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, 
				"fg_color format error, it should be like FF00FF. error value %V", arg);	
		return NGX_ERROR;
	}

	for (i = 0; i < 3; i++) {
		*(color + i) = ngx_hextoi(arg[0].data + i*2, 2);
	}

	return NGX_OK;
}

ngx_int_t 
ngx_http_qrcode_set_bg_color(ngx_http_request_t *r, ngx_int_t *color, ngx_array_t *compiled_args)
{
	ngx_str_t *arg;
	ngx_int_t i;

	arg = compiled_args->elts;

	if (arg[0].len != 6) {
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, 
				"bg_color format error, it should be like FF00FF. error value %V", arg);	
		return NGX_ERROR;
	}

	for (i = 0; i < 3; i++) {
		*(color + i) = ngx_hextoi(arg[0].data + i*2, 2);
	}

	return NGX_OK;
}

ngx_int_t 
ngx_http_qrcode_set_level(ngx_http_request_t *r, ngx_int_t *val, ngx_array_t *compiled_args)
{
	ngx_str_t *arg;
	ngx_int_t rc;

	arg = compiled_args->elts;

	rc = ngx_atoi(arg[0].data, arg[0].len);
	if (rc < 0 || rc > 3) { 
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, 
				"error level value %V, level should be 0, 1, 2 or 3.", arg);	
		return rc;
	}

	*val = rc;

	return NGX_OK;
}

ngx_int_t 
ngx_http_qrcode_set_hint(ngx_http_request_t *r, ngx_int_t *val, ngx_array_t *compiled_args)
{
	ngx_str_t *arg;
	ngx_int_t rc;

	arg = compiled_args->elts;

	rc = ngx_atoi(arg[0].data, arg[0].len);
	if (rc == NGX_ERROR) { 
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, 
				"error hint value %V", arg);	
		return rc;
	}

	*val = rc;

	return NGX_OK;
}

ngx_int_t 
ngx_http_qrcode_set_size(ngx_http_request_t *r, ngx_int_t *val, ngx_array_t *compiled_args)
{
	ngx_str_t *arg;
	ngx_int_t rc;

	arg = compiled_args->elts;

	rc = ngx_atoi(arg[0].data, arg[0].len);
	if (rc == NGX_ERROR) { 
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, 
				"error size value %V", arg);	
		return rc;
	}

	*val = rc;

	return NGX_OK;
}

ngx_int_t 
ngx_http_qrcode_set_margin(ngx_http_request_t *r, ngx_int_t *val, ngx_array_t *compiled_args)
{
	ngx_str_t *arg;
	ngx_int_t rc;

	arg = compiled_args->elts;

	rc = ngx_atoi(arg[0].data, arg[0].len);
	if (rc == NGX_ERROR) { 
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, 
				"error margin value %V.", arg);	
		return rc;
	}

	*val = rc;

	return NGX_OK;
}

ngx_int_t 
ngx_http_qrcode_set_version(ngx_http_request_t *r, ngx_int_t *val, ngx_array_t *compiled_args)
{
	ngx_str_t *arg;
	ngx_int_t rc;

	arg = compiled_args->elts;

	rc = ngx_atoi(arg[0].data, arg[0].len);
	if (rc < 0 || rc > 10) { 
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, 
				"error version value %V, it should be [0,10].", arg);	
		return rc;
	}

	*val = rc;

	return NGX_OK;
}

ngx_int_t 
ngx_http_qrcode_set_casesensitive(ngx_http_request_t *r, ngx_int_t *val, ngx_array_t *compiled_args)
{
	ngx_str_t *arg;
	ngx_int_t rc;

	arg = compiled_args->elts;

	rc = ngx_atoi(arg[0].data, arg[0].len);
	if (rc < 0 || rc > 1) { 
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, 
				"error casesensitive value %V, it should be 0 or 1.", arg);	
		return rc;
	}

	*val = rc;

	return NGX_OK;
}

ngx_int_t 
ngx_http_qrcode_set_content(ngx_http_request_t *r, ngx_str_t *val, ngx_array_t *compiled_args, ngx_int_t urlencode)
{
	ngx_str_t *arg;
	u_char *p;

	arg = compiled_args->elts;
	*val = arg[0];

	if (urlencode == 1) {
		p = ngx_pcalloc(r->pool, arg->len);
		val->data = p;

		ngx_unescape_uri(&p, &arg->data, arg->len, 0);
		val->len = p - val->data;
	}

	return NGX_OK;
}
  
ngx_int_t 
ngx_http_qrcode_eval_cmd_args(ngx_http_request_t *r,
		ngx_http_qrcode_cmd_t *cmd, ngx_array_t *compiled_args)
{
	ngx_http_qrcode_arg_template_t *value;
	ngx_str_t *arg;
	ngx_uint_t i;

	value = cmd->args->elts;

	for (i = 0; i < cmd->args->nelts; i++) 
	{
		arg = ngx_array_push(compiled_args);

		if (arg == NULL) {
			return NGX_HTTP_INTERNAL_SERVER_ERROR;
		}

		if (value[i].lengths == NULL) { /* does not contain vars */
			*arg = value[i].raw_value;
		}
	   	else {
			if (ngx_http_script_run(r, arg, value[i].lengths->elts,
						0, value[i].values->elts) == NULL) {
				return NGX_HTTP_INTERNAL_SERVER_ERROR;
			}
		}
	}

	return NGX_OK;
}

ngx_int_t 
ngx_http_qrcode_compile_args(ngx_http_request_t *r, ngx_http_qrcode_loc_conf_t *qlcf)
{
	ngx_array_t 			*cmds;
	ngx_http_qrcode_cmd_t	*cmd;
	ngx_array_t 			*compiled_args;
	ngx_int_t 				rc;
	ngx_uint_t				i;

	compiled_args	= NULL;
	rc 				= NGX_OK;

	cmds = qlcf->cmds;

	if (cmds == NULL)
		return NGX_DECLINED;
	
	cmd = cmds->elts;
	for (i = 0; i < cmds->nelts; i++) 
	{
		if (cmd[i].args) 
		{
			compiled_args = ngx_array_create(r->pool, 
					cmd[i].args->nelts, sizeof(ngx_str_t));

			if (compiled_args == NULL)
				return NGX_HTTP_INTERNAL_SERVER_ERROR;

			rc = ngx_http_qrcode_eval_cmd_args(r, &cmd[i], compiled_args);
			
			if (rc != NGX_OK) {
				ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
						"Failed to evaluate arguments for the directive.");
				return rc;
			}
		}

		/* switch cmd*/
		switch (cmd[i].cfg_code) 
		{
			case qrcode_cfg_fg_color:
				rc = ngx_http_qrcode_set_fg_color(r, qlcf->fg_color, compiled_args);
				break;
			case qrcode_cfg_bg_color:
				rc = ngx_http_qrcode_set_bg_color(r, qlcf->bg_color, compiled_args);
				break;
			case qrcode_cfg_level:
				rc = ngx_http_qrcode_set_level(r, &qlcf->level, compiled_args);
				break;
			case qrcode_cfg_hint:
				rc = ngx_http_qrcode_set_hint(r, &qlcf->hint, compiled_args);
				break;
			case qrcode_cfg_size:
				rc = ngx_http_qrcode_set_size(r, &qlcf->size, compiled_args);
				break;
			case qrcode_cfg_margin:
				rc = ngx_http_qrcode_set_margin(r, &qlcf->margin, compiled_args);
				break;
			case qrcode_cfg_version:
				rc = ngx_http_qrcode_set_version(r, &qlcf->version, compiled_args);
				break;
			case qrcode_cfg_txt:
				rc = ngx_http_qrcode_set_content(r, &qlcf->txt, compiled_args, 0);
				break;
			case qrcode_cfg_urlencode_txt:
				rc = ngx_http_qrcode_set_content(r, &qlcf->txt, compiled_args, 1);
				break;
			case qrcode_cfg_casesensitive:
				rc = ngx_http_qrcode_set_casesensitive(r, &qlcf->casesensitive, compiled_args);
				break;
			default:
				ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, 
						"Unknown cfg_code: %d", cmd[i].cfg_code);
				return NGX_HTTP_INTERNAL_SERVER_ERROR;
		}

		if (rc != NGX_OK)
			return rc;
	}

	ngx_log_error(NGX_LOG_DEBUG, r->connection->log, 0,
			"fg_color %xd%xd%xd%xd, "
			"bg_color %xd%xd%xd%xd, " 
			"level %d, "				
			"hint %d, "				
			"size %d, "				
			"margin %d, "				
			"version %d, "				
			"casesensitive %d, "			
			"txt %V",
			qlcf->fg_color[0], qlcf->fg_color[1], qlcf->fg_color[2], qlcf->fg_color[3],
			qlcf->bg_color[0], qlcf->bg_color[1], qlcf->bg_color[2], qlcf->bg_color[3],
			qlcf->level, qlcf->hint, qlcf->size, qlcf->margin, qlcf->version,
			qlcf->casesensitive, 
			&qlcf->txt);

	return NGX_OK;
}
