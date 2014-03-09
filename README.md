ngx_http_qrcode_module
======================

Description
===========

ngx_http_qrcode_module is a an addon for Nginx to generate and serve QR code.

Examples
========

    server {
      listen 80; 
      server_name  localhost; 
    
      #set with constant
      location /qr_with_con {
        qrcode_fg_color FF0000;
        qrcode_bg_color FFFFFF;    
        qrcode_level 2;
        qrcode_hint 2;
        qrcode_size 8;
        qrcode_margin 2;
        qrcode_version 2;
        qrcode_txt "http://wwww.dcshi.com";
        qrcode_casesensitive 1; 
        qrcode_gen;                                                                                                                                                        
      }

      #set with variables
	  location /qr_with_var {
		qrcode_fg_color $arg_fg_color;
		qrcode_bg_color $arg_bg_color;
		qrcode_level $arg_level;
		qrcode_hint $arg_hint;
		qrcode_size $arg_size;
		qrcode_margin $arg_margin;
		qrcode_version $arg_ver;
		qrcode_casesensitive $arg_case;
		qrcode_txt $arg_txt;
		#qrcode_urlencode_txt $arg_txt;

		qrcode_gen;
	  }  
    }
   
curl "http://localhost/qr?size=6&fg_color=00FF00&bg_color=fff700&case=1&txt=12a&margin=2&level=0&hint=2&ver=2"
    
Dependencies
============
ngx_http_qrcode_module depends upon libqrencode and libgd. Please install these first.
<br/>
manual of libqrencode : http://fukuchi.org/works/qrencode/manual/

Installation
============
Clone this responsitory git clone git@github.com:dcshi/ngx_http_qrcode_module.git.
<br/>
Download the Nginx source, extract and change the current working directory to the Nginx tree.
<br/>
Run configure with --add-module=/path/to/source/ngx_http_qrcode_module 
<br/>
Run make and make install. make install will probably need root permissions.
<br/>
    
Refer to http://wiki.nginx.org/3rdPartyModules.

Directives
==========
**Syntax**: ***qrcode_fg_color*** color  
**Default**: qrcode_fg_color 000000  
**Context**: http, server, location  
**Description**: set the color of QRcode.
<br/>

**Syntax**: ***qrcode_bg_color*** color  
**Default**: qrcode_bg_color FFFFFF  
**Context**: http, server, location  
**Description**: set the background color of QRcode.
<br/>

**Syntax**: ***qrcode_level*** level  
**Default**: qrcode_level 0  
**Context**: http, server, location    
**Description**: level of error correction, [0:3]. Refer to http://fukuchi.org/works/qrencode/manual/qrencode_8h.html#a35d70229ba985c61bbdab27b0f65740e
<br/>

**Syntax**: ***qrcode_hint*** hint    
**Default**: qrcode_hint 2  
**Context**: http, server, location    
**Description**: encoding mode. Refer to http://fukuchi.org/works/qrencode/manual/qrencode_8h.html#ab7ec78b96e63c8f019bb328a8d4f55db
<br/>

**Syntax**: ***qrcode_size*** size    
**Default**: qrcode_size 4      
**Context**: http, server, location    
**Description**: size of qrcode image.(pixel)
<br/>

**Syntax**: ***qrcode_margin*** margin  
**Default**: qrcode_margin 4  
**Context**: http, server, location    
**Description**: margin of qrcode image.(pixel)
<br/>

**Syntax**: ***qrcode_version*** version   
**Default**: qrcode_version 1    
**Context**: http, server, location    
**Description**: version of the symbol.it should less 10.
<br/>

**Syntax**: ***qrcode_casesensitive*** [1 | 0]
**Default**: qrcode_casesensitive off  
**Context**: http, server, location    
**Description**: case-sensitive(1) or not(0)
<br/>

**Syntax**: ***qrcode_txt*** txt           
**Default**: none    
**Context**: http, server, location        
**Description**: the txt you want to encode. 
<br/>

**Syntax**: ***qrcode_urlencode_txt*** txt           
**Default**: none    
**Context**: http, server, location        
**Description**: the txt you want to encode. Different from qrcode_txt, the directive will urldecode first. Just http%3a%2f%2fdcshi.com%3fa%3db%26c%3dd => http://dcshi.com?a=b&c=d
<br/>
  
**Syntax**: ***qrcode_gen***      
**Default**: none  
**Context**: http, server, location      
**Description**: generate QRcode.
<br/>
 
Author
======
dcshi(施俊伟) <dcshi@qq.com>

Copyright and License
=====================
This module is licensed under the BSD license.
Copyright (C) 2013, by dcshi(施俊伟). <dcshi@qq.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and 
      the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
