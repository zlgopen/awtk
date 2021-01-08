dt = date_time_create()
print(dt.year, "-", dt.month, "-", dt.day, " ", dt.hour, ":", dt.minute, ":", dt.second, "(", dt.wday, ")")

dt.year = 2022
dt.month = 10
dt.day = 1
dt.hour = 2
dt.minute = 3
dt.second = 4
assert(dt.year == 2022)
assert(dt.month == 10)
assert(dt.day == 1)
assert(dt.hour == 2)
assert(dt.minute == 3)
assert(dt.second == 4)
print(dt.year, "-", dt.month, "-", dt.day, " ", dt.hour, ":", dt.minute, ":", dt.second, "(", dt.wday, ")")

assert(date_time_to_time(dt) == 1664560984)
assert(date_time_from_time(dt, 1664560985))

assert(dt.year == 2022)
assert(dt.month == 10)
assert(dt.day == 1)
assert(dt.hour == 2)
assert(dt.minute == 3)
assert(dt.second == 5)

print(time_now_us())
print(time_now_ms())
print(time_now_s())


