#!/bin/sh -e
SYSVER=`uname -s -r`
FREESPACE=`/usr/bin/freespace`

#espeak -vde "Echtzeit-Datenrekoder-Applikation konnte nicht gestartet werden." 2> /dev/null
ZEIT="Es ist jetzt `date '+%H Uhr %M'` und der `date '+%ete%mte%Y'`"
espeak -vde -s 140 -p 10 "$ZEIT" 2> /dev/null 

# Print the IP address
_IP=$(hostname -I) || true
if [ "$_IP" ]; then
  HIP=`echo $_IP |sed -e 's/\./ punkt /g'`
  printf "Meine Eipiadresse ist %s\n" "$HIP" | espeak -vde -s140 2> /dev/null
fi

espeak -vde "System $SYSVER wurde gestartet." 2> /dev/null
espeak -vde "Freier Speicherplatz auf Wurzel-Dateisystem ist $FREESPACE Gigabeit" 2> /dev/null 
espeak -vde "`/usr/games/fortune /usr/share/games/fortunes/de/murphy`" -s 140 2> /dev/null
exit 0