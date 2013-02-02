/*===============================================================
*   Copyright (C) 2013 All rights reserved.
*   
*   Filename：ngx_http_qrcode_utils.h
*   Author ：dcshi
*   Created：2013-02-02
================================================================*/

#ifndef NGX_HTTP_QRCODE_UTILS_H
#define NGX_HTTP_QRCODE_UTILS_H

#include "ngx_http_qrcode_module.h"

ngx_int_t ngx_http_qrcode_compile_args(ngx_http_request_t *r, 
		ngx_http_qrcode_loc_conf_t *qlcf);

#endif
