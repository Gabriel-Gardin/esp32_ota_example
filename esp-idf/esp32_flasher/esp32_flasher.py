import os
import secrets
import names
import requests
import sys

sys.path.append('/home/sat/Documents/dev/esp32_ota_example/esp-idf/esp32_flasher/esptool')

#import esptool.espsecure.main as espsecure_main

def create_token():
    token = secrets.token_hex(32)
    with open('main/certs/thing_cert.pem', "w+") as f:
        f.write(token)

    return token

def registrate_device(token):

    url = 'https://gabriel3sat.pythonanywhere.com/api/device/'

    name = names.get_full_name()

    header = {"Authorization":"Token 4c5e1e86ad51373ed33fad50a111c72c5adc6c41"}

    payload = {
        "unique_id":token,
        "nome":name,
        "observacoes":None,
        "firmware":None
    }

    err = requests.post(url, data=payload, headers=header)
    
    print(err)


def main():

    os.path.join('https_ota/', 'esptool')

    os.chdir('../https_ota/')

    #os.system('. $HOME/esp/esp-idf/export.sh')

    token = create_token()

    os.chdir('../esp32_flasher/')
    
    os.system('idf.py build')

    #os.system('python3 esptool/espefuse.py --port /dev/ttyUSB0 burn_key flash_encryption key.bin') #Gravando a chave da flash

    #os.system('python3 esptool/espefuse.py --port /dev/ttyUSB0 burn_key BLOCK3 key.bin') #Gravando o ID do equipamento no BLOCK3

    #os.system('python espefuse.py --port /dev/ttyUSB0 write_protect_efuse FLASH_CRYPT_CNT')  #Desabilita FLASH_CRYPT_CNT contra escrita. Apenas firmware criptografados serão aceitos


    os.system('python esptool/espsecure.py encrypt_flash_data --keyfile key.bin --address 0x1000 -o encrypted/bootloader.bin ../https_ota/build/bootloader/bootloader.bin') #Criptografando o firmware original..


    os.system('python esptool/espsecure.py encrypt_flash_data --keyfile key.bin --address 0x9000 -o encrypted/partition_table.bin ../https_ota/build/partition_table/partition-table.bin')
        

    os.system('python esptool/espsecure.py encrypt_flash_data --keyfile key.bin --address 0xe000 -o encrypted/ota_data_initial.bin ../https_ota/build/ota_data_initial.bin')
    

    os.system('python esptool/espsecure.py encrypt_flash_data --keyfile key.bin --address 0x20000 -o encrypted/firmware.bin ../https_ota/build/https_ota.bin')
    

    #Fazendo upload do firmware criptografado
    os.system('python esptool/esptool.py -p /dev/ttyUSB0 -b 460800 --before default_reset --after no_reset --chip esp32 write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 encrypted/bootloader.bin 0x9000 encrypted/partition_table.bin 0xe000 encrypted/ota_data_initial.bin 0x20000 encrypted/firmware.bin')

    #registrate_device(token) #Faz o post no endpoin

    os.system('python esptool/espefuse.py --port /dev/ttyUSB0 summary')


if __name__ == "__main__":
    main()
