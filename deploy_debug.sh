#!/bin/bash

# interruzione in caso di errore
set -e

# Directory del progetto
PROJECT_DIR=$(dirname "$(realpath "$0")")

# Imposta la directory di destinazione sul Raspberry Pi 3
DEST_DIR="/home/display"

# Definisci la variabile per l'eseguibile
EXECUTABLE="DinamometerQt"
DEST_IP="169.254.0.2"
DEST_USER="display"
GDBSERVER_PORT=2345

# Verifica che le variabili d'ambiente siano impostate
if [ -z "$DEST_DIR" ] || [ -z "$EXECUTABLE" ] || [ -z "$PROJECT_DIR" ]; then
  echo "DEST_DIR or EXECUTABLE is not set. Please ensure they are set in the task configuration."
  exit 1
fi

# Funzione per terminare il processo attivo
terminate_process() {
  echo "Terminating the existing process on Raspberry Pi 3..."
  ssh $DEST_USER@$DEST_IP "pkill -f '${EXECUTABLE}'" 
}

# Termina il processo attivo, se esiste
terminate_process  || true

cd "${PROJECT_DIR}"

# Funzione per cambiare i permessi della cartella build e dei suoi contenuti
change_permissions() {
    if [ -d "build" ]; then
        sudo chmod -R u+w build
    fi
}

# Cambia i permessi della cartella build se esiste
#change_permissions
# Pulizia e creazione della cartella build
if [ -d "build" ]; then
    rm -rf build 
fi

mkdir build && cd build

# Recupera l'UID e il GID dell'utente corrente
USER_ID=$(id -u)
GROUP_ID=$(id -g)


echo "Building the project in Docker..."
# Compilazione con Docker
docker run --rm -u $USER_ID:$GROUP_ID -v $(pwd)/..:/project -w /project/build qt5-cross-compiler:v2 cmake -DCMAKE_TOOLCHAIN_FILE=/project/toolchain-armhf.cmake -DCMAKE_BUILD_TYPE=Debug /project

# make
docker run --rm -u $USER_ID:$GROUP_ID -v $(pwd)/..:/project -w /project/build qt5-cross-compiler:v2 make

if [ $? -eq 0 ]; then
    echo "Build successful. Copying the binary to Raspberry Pi 3..."
    # Copia su Raspberry Pi 3
    scp "${PROJECT_DIR}/build/${EXECUTABLE}" $DEST_USER@$DEST_IP:${DEST_DIR}

    if [ $? -eq 0 ]; then

        # Esecuzione su Raspberry Pi 3
        ssh -T $DEST_USER@$DEST_IP << EOF
        pkill gdbserver
        export QT_QPA_PLATFORM=eglfs
        export LD_LIBRARY_PATH=${DEST_DIR}
        cd ${DEST_DIR}

        echo "Checking for outputHistory directory..."
        #verifico presenza cartella outputHistory
        if [ ! -d "outputHistory" ]; then
            mkdir outputHistory 
            chmod 700 outputHistory
        fi

        echo "Archiving old log file..."
        if [ -f "output.log" ]; then
            mv output.log outputHistory/output-\$(date +'%Y%m%d-%H%M%S').log
        fi
        
        echo "Starting gdbserver..."
        gdbserver :${GDBSERVER_PORT} ./${EXECUTABLE} -platform eglfs > output.log 2>&1 &
EOF
#nohup ./${EXECUTABLE} -platform eglfs > output.log 2>&1 &
    else

        echo "Error: Failed to copy the binary to Raspberry Pi 3."

    fi


else 
    echo "Error: Build failed."
fi




echo "Deployment and execution completed on Raspberry Pi 3."
