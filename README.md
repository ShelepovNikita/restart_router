# Розетка для перезагрузки роутера при отсутствии сети.
## Описание

### Зачем:
Далеко от города в деревне стоит роутер, в который подключен древний нестабильный 4Г модем. Электричество часто отрубается, и после перезапуска модем не всегда успешно стартует, при условии запущеного роутера и сети Wi-Fi.

### Решение:
Перезапуск роутера до тех пор, пока не стартанет модем. Перезапуск физическим отключением от сети без участия человека.

### Реализация:
Скетч предназначен для прошивки ESP8266(ESP-01S). Был куплен заранее в комплекте с релейным модулем.
<div id="header" align="center">
  <img src="https://static.procontact74.ru/media/product/12620fac-6719-11e9-80c0-e0d55e81e32a_12620fae-6719-11e9-80c0-e0d55e81e32a.jpeg" width="400"/>
</div>

### Логика работы:
Пинг сервера Яндекса или Гугла, в случае неудачи реле перезапускает питание на роутер. При подключении в сеть загорается светодиод и тухнет после успешного подключения по Wi-Fi. После каждого успешного пинга светодиод мигает. Идея не моя, за основу был взят скетч с [сайта](https://mysku.club/blog/china-stores/57097.html) и адаптирован под покупной релейный модуль.
