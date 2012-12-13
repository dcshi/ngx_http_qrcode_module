ngx_http_qrcode_module
======================

## Description
                                                                                                                                                              
ngx_http_qrcode_module is a an addon for Nginx to generate and serve QR code.

###Examples:

    server {
      listen 80; 
      server_name  localhost; 
    
      location /qr {
        qrcode_fg_color FF0000;
        qrcode_bg_color FFFFFF;    
        qrcode_level 2;
        qrcode_hint 2;
        qrcode_size 8;
        qrcode_margin 2;
        qrcode_version 2;
        qrcode_txt "http://wwww.dcshi.com";
        qrcode_casesensitive on; 
        qrcode_gen;
    }   
}

## Dependencies
    ngx_http_qrcode_module depends upon libqrencode and libgd. Please install these first.
manual of libqrencode : http://fukuchi.org/works/qrencode/manual/

##Installation
    Clone this responsitory git clone git@github.com:dcshi/ngx_http_qrcode_module.git
    Download the Nginx source, extract and change the current working directory to the Nginx tree.
    Run configure with --add-module=/path/to/source/ngx_http_qrcode_module 
    Run make and make install. make install will probably need root permissions.
    
Refer to http://wiki.nginx.org/3rdPartyModules.

##Directives

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
**Description**: size of qrcode image.
<br/>

**Syntax**: ***qrcode_margin*** margin  
**Default**: qrcode_margin 4  
**Context**: http, server, location    
**Description**: margin of qrcode image.
<br/>

**Syntax**: ***qrcode_version*** version   
**Default**: qrcode_version 1    
**Context**: http, server, location    
**Description**: version of the symbol.
<br/>

**Syntax**: ***qrcode_casesensitive*** [on | off]     
**Default**: qrcode_casesensitive off  
**Context**: http, server, location    
**Description**: case-sensitive(on) or not(off)
<br/>

**Syntax**: ***qrcode_txt*** txt           
**Default**: qrcode_txt ""    
**Context**: http, server, location        
**Description**: the txt you want to encode. the max size is 1M
<br/>

**Syntax**: ***qrcode_gen***      
**Default**: none  
**Context**: http, server, location      
**Description**: generate QRcode.
<br/>
