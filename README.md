# File transmitter

Программа осуществляет передачу данных по сети от клиента к серверу.
Проект собирается с помощью `Cmake`, который собирает цели `server` и `client`.

## Параметры запуска

### Для сервера:
`./server [-p port] [-f folder]`

* `port` — порт, который будет слушать сервер (по умолчанию — 5679)
* `folder` — папка, в которую будут сохраняться файлы (по умолчанию это директория, из которой сервер был запущен)

### Для клиента:

`./client [-h host] [-p port] [-f filename] file`

* `host` — IP-адрес сервера (по умолчанию — 127.0.0.1)
* `port` — порт, на котором будет слушать сервер (по умолчанию — 5679)
* `filename` — имя, с которым надо сохранить файл на сервере (если не указано, то имя файла на сервере будет такое же, как локально)
* `file` — путь до файла, который надо передать