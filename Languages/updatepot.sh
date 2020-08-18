#!/bin/sh 

LOC=es_AR

xgettext --package-name fxmediainfo  --package-version 1.0 --default-domain fxmediainfo --keyword=_ --output tmp.pot ../*.cxx

# Update catalog by tmp.pot
poedit fxmediainfo.pot 

mv fxmediainfo.mo ./$LOC/

#msginit --no-translator --locale $LOC --output-file fxmediainfo_$LOC.po --input fxmediainfo.pot 

#msgfmt --check --verbose --output-file ./$LOC/fxmediainfo.mo fxmediainfo_$LOC.po

rm tmp.pot

exit
