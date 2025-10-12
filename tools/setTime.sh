#!/bin/bash

##################################################
# Set the time to a WorcClock 
##################################################
# The computer has to be conected 
# to the access point 
# (IP address: 4.3.2.1)
#
# Parameters:
#   hh hout to set
#   mm minute to set
#
# Without parameters the interactive mode starts at 
# 00:00 and stops at 23:55 in 5min steps.
################################################### 

# Basis-URL wenn verbunden mit AP der WordClock
BASE_URL="http://4.3.2.1/json/state"

# Parameter hh und mm einlesen
hh=$1
mm=$2

# Überprüfen ob Parameter leer sind, sonst Standardwerte setzen
if [ -z "$hh" ]; then
    hh=$(date +%H)  # Aktuelle Stunde
fi



# Schleife von 00:00 bis 23:55
for i in {0..287}
do
    # Überprüfen ob Parameter leer sind, sonst Standardwerte setzen
    if [ -z "$2" ]; then
        #alle 5 minuten 
        mm=$((i * 5))
    fi
    if [ -z "$1" ]; then
        hh=$((($mm / 60 % 24)))
    fi

    #einen neuen Unix Timestamp berechnen
    unixTS=$(date -j -f "%Y-%m-%d %H:%M:%S" "$(date +%Y-%m-%d) $hh:$((mm % 60)):00" +%s)

    echo "Zeit setzen $((hh % 24)):$((mm %60)) = $unixTS"

    #Zeit auf WordClock setzen
    curl -X POST "${BASE_URL}" -d '{"time":'$unixTS'}' -H "Content-Type: application/json"

    # bei parameter kein loop
    if [ -n "$1" ]; then
        break
    fi
    
    # Optional: kurze Pause zwischen Downloads
    #sleep 5

    # Warten auf Tastendruck
    read -p "Drücke Enter für den nächsten Zeit..." -r

    # Prüfe ob Antwort nicht leer ist (z.B. 'q' eingegeben)
    if [ -n "$REPLY" ]; then
        echo "Loop beendet."
        break
    fi
done
