compile CURL with SFTP support .

1, install openssl use exe or, install straw-perl and compile openssl :

使用vs命令行工具进入openssl源码路径openssl-3.0.5 下，执行命令:

mkdir build

perl Configure VC-WIN64A no-asm --prefix=D:\download\sftp_project\openssl-3.0.5\build

nmake

nmake install

and set those lib and include into below cmake config.

2, compile zlib , cmake and vs2019 OK, use Administrator privilege to build INSTALL
3, compile libssh2, cmake and vs2019, same to zlib , but carefully set openssl libs

4, compile CURL use vs2019. 

test worked.