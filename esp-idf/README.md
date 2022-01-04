# Provisionamento de credenciais via SOFT AP e HTTPS OTA

Este exemplo de código utiliza um ESP32 e o SDK ESP-IDF para prover um exemplo de atualização remota via HTTPS e provisionamento de credenciais de Wi-Fi via o aplicativo da espressif. 

Aplicatiovo Android: [playstore](https://play.google.com/store/apps/details?id=com.espressif.provsoftap) e [github](https://github.com/espressif/esp-idf-provisioning-android)

Aplicatiovo iOS: [app store](https://apps.apple.com/in/app/esp-softap-provisioning/id1474040630) e [github](https://github.com/espressif/esp-idf-provisioning-ios)

Este exemplo está configurado para trabalhar com imagens criptografadas, isto é, apenas app's que tenham sido encriptografados gravados com a mesma chave serão bootados, caso não tenha sido a esp reiniciará sem parar. Isto foi feito seguindo a documentação oficial da espressif e também [este](https://www.embarcados.com.br/protecao-da-flash-no-esp32/) artigo.

**Para desabilitar esta função, entre no menuconfig e desabilite as opções:** 

_Security features -> Enable flash encryption on boot._

__Note__: É importante desabilitar está opção se não for trabalhar com imagens criptografadas.


__Observação__: Ainda é possível gravar e bootar imagens não criptografadas pela atualização OTA, mas, pela UART e JTAG apenas firmware criptografados.

ESP-IDF version: v4.3.2


## Installação

Primeiro limpe o projeto para atualizar com seus diretórios

```bash
idf.py fullclean
```

## Gravando a versão criptografada

```bash
idf.py encrypted-flash monitor
```

## Gerando uma chave

```bash
python espsecure.py generate_flash_encryption_key key.bin
```

## Gravando a chave encriptografada

```bash
python espefuse.py --port /dev/ttyUSB0 burn_key flash_encryption key.bin
```
