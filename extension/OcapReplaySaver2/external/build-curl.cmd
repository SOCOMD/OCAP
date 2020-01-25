pushd curl
call buildconf.bat
cd winbuild\
nmake /f Makefile.vc mode=static ENABLE_UNICODE=yes DEBUG=no GEN_PDB=no MACHINE=x64 WITH_PREFIX=..\build
popd