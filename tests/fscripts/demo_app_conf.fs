
assert(object_set(app_conf(), "timeout", 100))
assert(object_set(app_conf(), "serial.device", "/dev/tty01"))
assert(object_set(app_conf(), "serial.baudrate", 115200))
assert(app_conf_save())

assert(object_get(app_conf(), "timeout") == 100)
assert(object_get(app_conf(), "serial.device") == "/dev/tty01")
assert(object_get(app_conf(), "serial.baudrate") == 115200)

assert(object_remove(app_conf(), "timeout"))
assert(object_remove(app_conf(), "serial"))

assert(object_get(app_conf(), "timeout", 10) == 10)
assert(object_get(app_conf(), "serial.device", "a") == "a")
assert(object_get(app_conf(), "serial.baudrate", 1000) == 1000) 
assert(object_set(app_conf(), "timeout", 100))

assert(app_conf_save())
