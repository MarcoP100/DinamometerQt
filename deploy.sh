#!/bin/bash

# interruzione in caso di errore
set -e

# Directory del progetto
PROJECT_DIR=$(dirname "$(realpath "$0")")

# Imposta la directory di destinazione sul Raspberry Pi 3
DEST_DIR="/home/display"

# Definisci la variabile per l'eseguibile
EXECUTABLE="DinamometerQt"

# Verifica che le variabili d'ambiente siano impostate
if [ -z "$DEST_DIR" ] || [ -z "$EXECUTABLE" ] || [ -z "$PROJECT_DIR" ]; then
  echo "DEST_DIR or EXECUTABLE is not set. Please ensure they are set in the task configuration."
  exit 1
fi

# Funzione per terminare il processo attivo
terminate_process() {
  echo "Terminating the existing process on Raspberry Pi 3..."
  ssh display@169.254.0.2 'pkill -f "${EXECUTABLE}"'
}

# Termina il processo attivo, se esiste
terminate_process  # || true

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

# Compilazione con Docker
docker run --rm -u $USER_ID:$GROUP_ID -v $(pwd)/..:/project -w /project/build qt5-cross-compiler cmake -DCMAKE_TOOLCHAIN_FILE=/project/toolchain-armhf.cmake /project

# make
docker run --rm -u $USER_ID:$GROUP_ID -v $(pwd)/..:/project -w /project/build qt5-cross-compiler make

# Copia su Raspberry Pi 3
scp "${PROJECT_DIR}/build/${EXECUTABLE}" display@169.254.0.2:${DEST_DIR}

# Esecuzione su Raspberry Pi 3
#ssh -tt display@169.254.0.2 << 'EOF'
ssh display@169.254.0.2 << 'EOF'
export QT_QPA_PLATFORM=eglfs
export LD_LIBRARY_PATH="${DEST_DIR}"
cd "${DEST_DIR}"

#verifico presenza cartella outputHistory
if [ ! -d "outputHistory" ]; then
    mkdir outputHistory 
    chmod 700 outputHistory
fi

if [ -f "output.log" ]; then
    mv output.log outputHistory/output-$(date +'%Y%m%d-%H%M%S').log
fi
nohup "${EXECUTABLE}" -platform eglfs > output.log 2>&1 &

EOF

echo "Deployment and execution completed on Raspberry Pi 3."
